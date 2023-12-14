#pragma once
#include <thread>
class SpinLock
{
public:
#pragma region Memo
	/*
	CAS�Լ� - (compare and swap)

	@ CAS �ǻ�  �ڵ� ���� - ���� �Լ�
	_locked.compare_exchange_strong(expected, desired)
	@ CAS �ǻ�  �ڵ� ���� -���� �Լ� Ǯ��
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
	//volatile c++ Ű���� : �����Ϸ����� ����ȭ ���� ���� ��û
	//volatile bool _locked = false;
	atomic<bool> _locked = false;
};

class SpinLock2
{
public:
#pragma region Memo
	/*
	Sleep - ��� ���� ���� �ʰ� �����ٸ�
	*/
#pragma endregion
	void lock()
	{
		bool expected = false;
		bool desired = true;
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			//this_thread::sleep_for(100ms);// 100�и� ��� �� - ����
			//this_thread::sleep_for(std::chrono::microseconds(100));// �����ð����� ���
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