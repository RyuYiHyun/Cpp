#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <atomic>	// atomic<자료형> ,   자료.fetch_add(값);
#include <mutex>	// Mutual Exclusive (상호 배타적)
/* 데드락 주의 // 스핀락 : 계속 대기 체크 // 컨텍스트 스위칭 */
#include <Windows.h> // Event 사용
#include <future>




void subfoo1(int a, int b)
{
	cout << a << endl;
	cout << b << endl;
}

template<typename ... Types>
void foo1(Types ... args)
{
	subfoo1(args...);
	return;
}

int main()
{
	foo1(3,1);
}

