#include "pch.h"
#include <iostream>
#include <thread>	// �������� ������ ȯ�� ������ ������ ���� (������ ǥ��)
#include <future>
int64 result;
int64 Calculate()
{
	int64 sum = 0;
	for (int32 i = 0; i < 100'000; i++)
	{
		sum += i;
	}
	result = sum;
	return sum;
}

void PromisWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)//Rvalue
{
	task();
}

int main()
{
	//����(synchronous) ����
	int64 sum = Calculate();
	cout << sum << endl;

	//std::future
	{
		// 1) deferred -> lazy ebaluation �����ؼ� �����ϼ���
		// 2) async -> ������ �����带 ���� �����ϼ���
		// 3) deferred | async -> �� �� �˾Ƽ� ����ּ���.
		std::future<int64> future = std::async(std::launch::async, Calculate);

		std::future_status status = future.wait_for(1ms);// �������� ��ٷ�����.
		if (status == future_status::ready)
		{// �غ�Ϸ�
			//ó��
		}
		int64 sum = future.get();//get�� 2�� ȣ���ϸ� �ȵȴ�.

		class Knight
		{
		public:
			int64 GetHp() { return 100; }
		};

		Knight knight;
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHp, knight);
	}
	//std::promise
	{
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromisWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;
		t.join();
	}
	//std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}
	//��ȸ������ ����� �޾ƿ��� �۾��� ����
}