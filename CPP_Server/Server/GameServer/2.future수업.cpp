#include "pch.h"
#include <iostream>
#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
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
	//동기(synchronous) 실행
	int64 sum = Calculate();
	cout << sum << endl;

	//std::future
	{
		// 1) deferred -> lazy ebaluation 지연해서 실행하세요
		// 2) async -> 별도의 쓰레드를 만들어서 실행하세요
		// 3) deferred | async -> 둘 중 알아서 골라주세요.
		std::future<int64> future = std::async(std::launch::async, Calculate);

		std::future_status status = future.wait_for(1ms);// 끝났는지 기다려본다.
		if (status == future_status::ready)
		{// 준비완료
			//처리
		}
		int64 sum = future.get();//get은 2번 호출하면 안된다.

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
	//일회성으로 결과를 받아오는 작업에 유용
}