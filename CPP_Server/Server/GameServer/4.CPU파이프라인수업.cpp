#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <atomic>	// atomic<자료형> ,   자료.fetch_add(값);
#include <mutex>	// Mutual Exclusive (상호 배타적)
/* 데드락 주의 // 스핀락 : 계속 대기 체크 // 컨텍스트 스위칭 */
#include <Windows.h> // Event 사용
#include <future>


//가시성, 코드 재배치
int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

volatile bool ready;// 컴파일러 최적화 하지않기 volatile 키워드

void Thread_1()
{
	while (!ready);
	y = 1;// store y
	r1 = x;// load x
}
void Thread_2()
{
	while (!ready);
	x = 1;// store x
	r2 = y;// load y
}
int main()
{
	int32 count = 0;
	while (true)
	{
		ready = false;
		count++;

		x = y = r1 = r2 = 0;

		thread t1(Thread_1);
		thread t2(Thread_2);

		ready = true;

		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
		{
			break;
		}
	}

	cout << count << "번 빠져나옴" << endl;
}