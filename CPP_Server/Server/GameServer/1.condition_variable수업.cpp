#include "pch.h"
#include <iostream>
#include <mutex>	// Mutual Exclusive (��ȣ ��Ÿ��)
#include <thread>	// �������� ������ ȯ�� ������ ������ ���� (������ ǥ��)
#include <Windows.h> // Event ���
mutex m;
queue<int32> q;
HANDLE handle;

// ��������_������Ʈ (Ŀ�� ������Ʈ X)
condition_variable cv;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);//1) lock ��� 
			q.push(100);//2) ���� ���� ���� ����
		}//3) lock Ǯ��

		//::SetEvent(handle);//4) ���� ���� ���� �ٸ� �����忡�� ����
		cv.notify_one();// wait ���� ������ ��1���� �����. //4) ���� ���� ���� �ٸ� �����忡�� ����

		//this_thread::sleep_for(100ms);
	}
}

void Consumer()
{
	while (true)
	{
		//::WaitForSingleObject(handle, INFINITE);// �̺�Ʈ ���

		unique_lock<mutex> lock(m);
		
		cv.wait(lock, []() {return q.empty() == false; });
		//1) lock ���
		//2) ������ Ȯ��
		// - ����O -> �������� ����
		//- ����X -> lock�� Ǯ�� ��� ����

		//while (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	/* Ŀ�� ������Ʈ (Ŀ��, �ü������ �����ϴ� ������Ʈ) �� �Ӽ�
	@ Usage Count : �󸶳� ����ϴ���
	@ Signal / Non-Signal << bool
	@ Auto / Manual << bool �ڵ����� ��������
	*/
	handle = ::CreateEvent(NULL/*���ȼӼ�*/, FALSE/*����TRUE �ڵ�FALSE ����*/
		, FALSE/*�ʱ� ����*/, NULL /*�̸�*/);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}