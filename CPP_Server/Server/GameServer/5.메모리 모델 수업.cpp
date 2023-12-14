#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <atomic>	// atomic<자료형> ,   자료.fetch_add(값);
#include <mutex>	// Mutual Exclusive (상호 배타적)
/* 데드락 주의 // 스핀락 : 계속 대기 체크 // 컨텍스트 스위칭 */
#include <Windows.h> // Event 사용
#include <future>


atomic<bool> flag;
int main()
{
	flag = false;
	//flag.is_lock_free();// 원자적으로 보장되는지 여부 확인

	//flag = true; 과정과 동일
	flag.store(true, memory_order_seq_cst);
	//bool val = flag; 과정과 동일
	bool val = flag.load(memory_order_seq_cst);

	//이전 flag 값을 prev에 넣고, flag 값을 수정
	{
		//bool prev = flag;
		//flag = true;
		bool prev = flag.exchange(true);// 읽고 쓰기 동시에

	}
	//CAS (compare-and-swap) 조건부 수정
	{
		{
			bool expected = false;
			bool desired = true;
			//if (flag == expected)
			//{
			//	//expected = flag;
			//	flag = desired;
			//	return true;
			//}
			//else
			//{
			//	expected = flag;
			//	return false;
			//}
			flag.compare_exchange_strong(expected, desired);
		}

		{
			while (true)
			{
				bool expected = false;
				bool desired = true;
				//if (flag == expected)
				//{
				//	//다른 스레드의 interruption을 받아서 실패할 수 있음
				//	if (묘한 상황)
				//	{
				//		return false;
				//	}
				//	//expected = flag;
				//	flag = desired;
				//	return true;
				//}
				//else
				//{
				//	expected = flag;
				//	return false;
				//}
				flag.compare_exchange_weak(expected, desired);
			}
		}
	}
}
//===========================================================================================
//===========================================================================================
//===========================================================================================
//===========================================================================================
//===========================================================================================
//===========================================================================================
#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <atomic>	// atomic<자료형> ,   자료.fetch_add(값);
#include <mutex>	// Mutual Exclusive (상호 배타적)
/* 데드락 주의 // 스핀락 : 계속 대기 체크 // 컨텍스트 스위칭 */
#include <Windows.h> // Event 사용
#include <future>



atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;
	ready.store(true, memory_order::memory_order_seq_cst);
}
void Consumer()
{
	while (ready.load(memory_order::memory_order_seq_cst));
	cout << value << endl;
}

int main()
{
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

	// Memory model(메모리 정책)
	// 1) Sequentially Consistent (seq_cst)
	// 2) Acquire-Release (consume, acquire, release, acq_rel) -> 핵심 (acquire, release)
	// 3) Reiaxed (relaxed)

	// 1) seq_cst (가장 엄격 = 컴파일러 최적화 여지가 적음 = 직관적)
	// 가시성 문제 해겨르 코드 재배치 해결

	// 2) acquire-release
	// 중간 단계
	// release 명령 이전의 명령들이 acquire 이후로 재배치 방지, 반대로 acquire 이후의 코드가 중간에 들어오는 걸 방지
	// acquire로 같은 변수를 읽는 스레드가 있다면 가시성 보장

	// 3) relaxed (자유롭다, 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 가시성 문제, 재배치 문제 그대로
	// 기본조건 (동일 객체에 대한 동일 관전 순서만 보장)
}
//===========================================================================================
//===========================================================================================
//===========================================================================================
//===========================================================================================
//===========================================================================================
//===========================================================================================

#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// 리눅스와 윈도우 환경 공용적 스레드 생성 (스레드 표준)
#include <atomic>	// atomic<자료형> ,   자료.fetch_add(값);
#include <mutex>	// Mutual Exclusive (상호 배타적)
/* 데드락 주의 // 스핀락 : 계속 대기 체크 // 컨텍스트 스위칭 */
#include <Windows.h> // Event 사용
#include <future>

atomic<int32> num;

int main()
{
	//=================================================
	//=================================================
	// atomic 복습
	num.store(1);
	cout << num.load() << endl;

	num.store(3);

	cout << num.load() << endl;

	//int32 a = num.exchange(1);//기존의 값을 리턴하고 파라미터 값을 세팅
	int32 a = 1;
	bool result = num.compare_exchange_strong(a, 5);// arg1값과 비교해서 같으면 arg2를 세팅 true 리턴, 아니면 기존값 그대로 arg1에 num값을 넣기 false 리턴
	cout << result << endl;
	//int32 a = num.exchange(1);

	cout << "a  = " << a << endl;
	cout << num.load() << endl;
	//=================================================
	//=================================================
	// 원자적 연산
	{
		atomic<int64> v;
		cout << v.is_lock_free() << endl; // 1리턴 , true -> cpu가 원자적 연산을 해줌
	}
	{
		struct Knight
		{
			int32 level;
			int32 hp;
			int32 pm;
		};
		atomic<Knight> v;
		cout << v.is_lock_free() << endl; // 0리턴 , false -> cpu가 원자적 연산을 못함
	}
	//=================================================
	//=================================================
	// 매모리 정책
	atomic<int64> v;
	v.store(1, memory_order_seq_cst);
	cout << v.load(memory_order_seq_cst) << endl;
}