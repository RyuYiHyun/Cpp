#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <atomic>	// atomic<자료형> ,   자료.fetch_add(값);
#include <mutex>	// Mutual Exclusive (상호 배타적)
/* 데드락 주의 // 스핀락 : 계속 대기 체크 // 컨텍스트 스위칭 */
#include <Windows.h> // Event 사용
#include <future>



//__declspec(thread) int32 value;//옛날 방법
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
