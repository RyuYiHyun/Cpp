#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// �������� ������ ȯ�� ������ ������ ���� (������ ǥ��)
#include <atomic>	// atomic<�ڷ���> ,   �ڷ�.fetch_add(��);
#include <mutex>	// Mutual Exclusive (��ȣ ��Ÿ��)
/* ����� ���� // ���ɶ� : ��� ��� üũ // ���ؽ�Ʈ ����Ī */
#include <Windows.h> // Event ���
#include <future>



//__declspec(thread) int32 value;//���� ���
thread_local int32 LThreadId = 0;

void ThreadMain(int32 threadid)
{
	LThreadId = threadid;
	while (true)
	{
		cout << "Hi! I am Thread" << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}
int main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; i++)
	{
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain, threadId));
	}

	for (thread& t : threads)
	{
		t.join();
	}
}
