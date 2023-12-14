#pragma once
#include <thread>
class SpinLock
{
public:
#pragma region Memo
	/*
	CAS함수 - (compare and swap)

	@ CAS 의사  코드 설명 - 원본 함수
	_locked.compare_exchange_strong(expected, desired)
	@ CAS 의사  코드 설명 -원본 함수 풀이
	if (_locked == expected)
	{
		expected = _locked;
		_locked = desired;
		return true;
	}
	else
	{
		expected = _locked;
		return false;
	}
	*/
#pragma endregion
	void lock()
	{
		bool expected = false;
		bool desired = true;
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}
	}
	void unlock()
	{
		_locked.store(false);
	}
private:
	//volatile c++ 키워드 : 컴파일러에게 최적화 하지 거절 요청
	//volatile bool _locked = false;
	atomic<bool> _locked = false;
};

class SpinLock2
{
public:
#pragma region Memo
	/*
	Sleep - 계속 루프 돌지 않고 스케줄링
	*/
#pragma endregion
	void lock()
	{
		bool expected = false;
		bool desired = true;
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			//this_thread::sleep_for(100ms);// 100밀리 대기 후 - 같음
			//this_thread::sleep_for(std::chrono::microseconds(100));// 일정시간동안 대기
			this_thread::sleep_for(0ms); // == this_thread::yield();			
		}
	}
	void unlock()
	{
		_locked.store(false);
	}
private:
	atomic<bool> _locked = false;
};