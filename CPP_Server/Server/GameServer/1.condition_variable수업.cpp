#include "pch.h"
#include <iostream>
#include <mutex>	// Mutual Exclusive (상호 배타적)
#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <Windows.h> // Event 사용
mutex m;
queue<int32> q;
HANDLE handle;

// 유저레벨_오브젝트 (커널 오브젝트 X)
condition_variable cv;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);//1) lock 잡기 
			q.push(100);//2) 공유 변수 값을 수정
		}//3) lock 풀기

		//::SetEvent(handle);//4) 조건 변수 통해 다른 쓰레드에게 통지
		cv.notify_one();// wait 중인 쓰레드 딱1개를 깨운다. //4) 조건 변수 통해 다른 쓰레드에게 통지

		//this_thread::sleep_for(100ms);
	}
}

void Consumer()
{
	while (true)
	{
		//::WaitForSingleObject(handle, INFINITE);// 이벤트 대기

		unique_lock<mutex> lock(m);
		
		cv.wait(lock, []() {return q.empty() == false; });
		//1) lock 잡기
		//2) 조건을 확인
		// - 만족O -> 빠져나와 실행
		//- 만족X -> lock을 풀고 대기 상태

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
	/* 커널 오브젝트 (커널, 운영체제에서 관리하는 오브젝트) 및 속성
	@ Usage Count : 얼마나 사용하는지
	@ Signal / Non-Signal << bool
	@ Auto / Manual << bool 자동인지 수동인지
	*/
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*수동TRUE 자동FALSE 리셋*/
		, FALSE/*초기 세팅*/, NULL /*이름*/);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}