#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <atomic>	// atomic<자료형> ,   자료.fetch_add(값);
#include <mutex>	// Mutual Exclusive (상호 배타적)
/* 데드락 주의 // 스핀락 : 계속 대기 체크 // 컨텍스트 스위칭 */
#include <Windows.h> // Event 사용
#include <future>


int32 buffer[10'000][10'000];


int main()
{
	memset(buffer, 0, sizeof(buffer));
	{
		uint64 start = GetTickCount64();

		int64 sum = 0;
		for (int32 i = 0; i < 10'000; i++)
		{
			for (int32 j = 0; j < 10'000; j++)
			{
				sum += buffer[i][j];
			}
		}
		uint64 end = GetTickCount64();

		cout << "Elapsed Tick" << (end - start) << endl;
	}

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;
		for (int32 i = 0; i < 10'000; i++)
		{
			for (int32 j = 0; j < 10'000; j++)
			{
				sum += buffer[j][i];
			}
		}
		uint64 end = GetTickCount64();

		cout << "Elapsed Tick" << (end - start) << endl;
	}
}