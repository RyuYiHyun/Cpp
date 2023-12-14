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
