#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>	// �������� ������ ȯ�� ������ ������ ���� (������ ǥ��)
#include <atomic>	// atomic<�ڷ���> ,   �ڷ�.fetch_add(��);
#include <mutex>	// Mutual Exclusive (��ȣ ��Ÿ��)
/* ����� ���� // ���ɶ� : ��� ��� üũ // ���ؽ�Ʈ ����Ī */
#include <Windows.h> // Event ���
#include <future>


atomic<bool> flag;
int main()
{
	flag = false;
	//flag.is_lock_free();// ���������� ����Ǵ��� ���� Ȯ��

	//flag = true; ������ ����
	flag.store(true, memory_order_seq_cst);
	//bool val = flag; ������ ����
	bool val = flag.load(memory_order_seq_cst);

	//���� flag ���� prev�� �ְ�, flag ���� ����
	{
		//bool prev = flag;
		//flag = true;
		bool prev = flag.exchange(true);// �а� ���� ���ÿ�

	}
	//CAS (compare-and-swap) ���Ǻ� ����
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
				//	//�ٸ� �������� interruption�� �޾Ƽ� ������ �� ����
				//	if (���� ��Ȳ)
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

#include <thread>	// �������� ������ ȯ�� ������ ������ ���� (������ ǥ��)
#include <atomic>	// atomic<�ڷ���> ,   �ڷ�.fetch_add(��);
#include <mutex>	// Mutual Exclusive (��ȣ ��Ÿ��)
/* ����� ���� // ���ɶ� : ��� ��� üũ // ���ؽ�Ʈ ����Ī */
#include <Windows.h> // Event ���
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

	// Memory model(�޸� ��å)
	// 1) Sequentially Consistent (seq_cst)
	// 2) Acquire-Release (consume, acquire, release, acq_rel) -> �ٽ� (acquire, release)
	// 3) Reiaxed (relaxed)

	// 1) seq_cst (���� ���� = �����Ϸ� ����ȭ ������ ���� = ������)
	// ���ü� ���� �ذܸ� �ڵ� ���ġ �ذ�

	// 2) acquire-release
	// �߰� �ܰ�
	// release ��� ������ ��ɵ��� acquire ���ķ� ���ġ ����, �ݴ�� acquire ������ �ڵ尡 �߰��� ������ �� ����
	// acquire�� ���� ������ �д� �����尡 �ִٸ� ���ü� ����

	// 3) relaxed (�����Ӵ�, �����Ϸ� ����ȭ ���� ���� = ���������� ����)
	// ���ü� ����, ���ġ ���� �״��
	// �⺻���� (���� ��ü�� ���� ���� ���� ������ ����)
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

#include <thread>	// �������� ������ ȯ�� ������ ������ ���� (������ ǥ��)
#include <atomic>	// atomic<�ڷ���> ,   �ڷ�.fetch_add(��);
#include <mutex>	// Mutual Exclusive (��ȣ ��Ÿ��)
/* ����� ���� // ���ɶ� : ��� ��� üũ // ���ؽ�Ʈ ����Ī */
#include <Windows.h> // Event ���
#include <future>

atomic<int32> num;

int main()
{
	//=================================================
	//=================================================
	// atomic ����
	num.store(1);
	cout << num.load() << endl;

	num.store(3);

	cout << num.load() << endl;

	//int32 a = num.exchange(1);//������ ���� �����ϰ� �Ķ���� ���� ����
	int32 a = 1;
	bool result = num.compare_exchange_strong(a, 5);// arg1���� ���ؼ� ������ arg2�� ���� true ����, �ƴϸ� ������ �״�� arg1�� num���� �ֱ� false ����
	cout << result << endl;
	//int32 a = num.exchange(1);

	cout << "a  = " << a << endl;
	cout << num.load() << endl;
	//=================================================
	//=================================================
	// ������ ����
	{
		atomic<int64> v;
		cout << v.is_lock_free() << endl; // 1���� , true -> cpu�� ������ ������ ����
	}
	{
		struct Knight
		{
			int32 level;
			int32 hp;
			int32 pm;
		};
		atomic<Knight> v;
		cout << v.is_lock_free() << endl; // 0���� , false -> cpu�� ������ ������ ����
	}
	//=================================================
	//=================================================
	// �Ÿ� ��å
	atomic<int64> v;
	v.store(1, memory_order_seq_cst);
	cout << v.load(memory_order_seq_cst) << endl;
}