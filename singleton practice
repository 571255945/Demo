单例练习

#if 0//懒汉单例模式
/************************************************************************/
/*      
	要点：1.构造函数私有化，及构造，拷贝构造，赋值运算符私有化
		  2.提供全局静态访问点，供外部调用
		  3.通过锁机制或static初始化，保证多线程访问对象安全

*/
/************************************************************************/
#include <iostream>
#include <vector>
#include <thread>
#include <windows.h>
#include <mutex>
using namespace std;
const int THREAD_NUM = 10;
mutex m_mutex;
int num = 0;

/*
//没有加锁时
this is a Constructor
thread ID 9252 ----------0 //输出了0 说明构造就没有执行完


thread ID 3844 ----------0
this is 1 th
thread ID 3604 ----------0
this is 2 th
thread ID 12832 ----------0
this is 3 th
thread ID 13260 ----------0
this is 4 th
thread ID 8644 ----------0
this is 5 ththis is 0 th
thread ID 3340 ----------0
this is 6 th
thread ID 10916 ----------0
this is 7 th
thread ID 3988 ----------0
this is 8 th请按任意键继续. . .
thread ID 1536 ----------10
this is 9 th


//加锁后
this is a Constructor
请按任意键继续. . .
thread ID 10280 ----------10

thread ID 11940 ----------10
this is 0 th
thread ID 9260 ----------10
this is 1 ththis is 2 th
thread ID 2548 ----------10
this is 3 th
thread ID 7256 ----------10
this is 4 th
thread ID 11344 ----------10
this is 5 th
thread ID 2356 ----------10
this is 6 th
thread ID 6416 ----------10
this is 7 th
thread ID 13628 ----------10
this is 8 th
thread ID 13144 ----------10
this is 9 th
*/


class CSingle
{
private:
	CSingle():num(0){
		//lock_guard<mutex> lock(m_mutex);
		cout << "this is a Constructor" << endl;
		Sleep(5000);
		num = 10;
		//cout << "this is a Constructor => after sleep" << endl;
		
	}
	~CSingle(){};
	CSingle(const CSingle&);
	const CSingle& operator=(const CSingle&);
public:
	//如果返回实例（CSingle）由于拷贝构造私有化了，那么CSingle singleInstance = CSingle::GetInstance();将是非法的
	//返回指针（CSingle*）则可以避免上面的问题，及CSingle* singleInstance = CSingle::GetInstance();是合法的
	static CSingle* GetInstance(){
		//c++11标准下，local static对象在多线程的情况下初始化是线程安全的,vs2013好像是不支持这种特性的，所以最好还是加个锁
		lock_guard<mutex> lock(m_mutex);// z这里如果不加锁，一个线程来构造，如果构造花了很长时间，其他线程在m_instance实例没有完全构造成功的情况下就会返回，具体实验结果见上面的数据
		static CSingle m_instance;      //但是这里加锁，显然也不是一个明智的方案，因为每个线程进来都要锁一次，效率不高！！！
		return &m_instance;
	}
public:
	int num;
};
//验证：c++11标准下，local static对象在多线程的情况下初始化是线程安全的
//可以发现，同时在多个线程执行时，构造函数只执行了一次
void testStatic()
{
	
	CSingle* p=CSingle::GetInstance();
	printf("\n thread ID %d ----------%d\n",GetCurrentThreadId(), p->num);
	//cout << "\n thread id is: " << GetCurrentThreadId() << endl;
	//cout << "this is num " << num++ << endl;
	printf("this is %d th", num++);
}

int main()
{
	thread threadArray[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		threadArray[i] = thread(testStatic);
		threadArray[i].detach();
	}
	
	for (int k = 0; k < THREAD_NUM;++k)
	{
		if (threadArray[k].joinable())
		{
			threadArray[k].join();//主线程等待所有线程都结束
		}
	}
	//while (true)
	//{

	//}
	system("pause");

	return 0;
}
#endif
#if 0//饿汉单例
#include<iostream>
#include <thread>
#include<windows.h>
using namespace std;
const int THREAD_NUM = 10;

int num = 0;
class CSingle1
{
private:
	CSingle1();
	~CSingle1();
	CSingle1(const CSingle1&);
	const CSingle1& operator=(const CSingle1&);
private:
	static CSingle1 *ptr;
public:
	static CSingle1* Instance();

};
//全局变量和static的初始化就是 在main函数之前执行,所以不存在线程安全问题
CSingle1* CSingle1::ptr = new CSingle1();
CSingle1* CSingle1::Instance(){
	return ptr;
}
CSingle1::CSingle1()
{
	cout << " this is constructor  "<< endl;
}

CSingle1::~CSingle1()
{
	OutputDebugString("this is descontructor");
	if (ptr!=nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}
void testSingle()
{
	
	cout << "subthread , thread id is = " << GetCurrentThreadId() << endl;
	CSingle1* testIns = CSingle1::Instance();
	cout << "testSingle : " << ++num << endl;

}
int main()
{
	cout << "this is main func" << endl;
	thread threadArray[10];
	for (int i = 0; i < THREAD_NUM;++i)
	{
		threadArray[i] = thread(testSingle);
		threadArray[i].detach();
	}
	for (int i = 0; i < THREAD_NUM;++i)
	{
		if (threadArray[i].joinable())
		{
			threadArray[i].join();
		}
	}

	system("pause");
	return 0;
}
#endif

#if 0//单例模板,懒汉模式  没有使用C++11 静态变量初始化时是线程安全的这一属性
#include<windows.h>
#include<iostream>
#include<mutex>
#include<thread>
using namespace std;

template<typename T>
class CSingleton
{
private://禁止拷贝和赋值
	CSingleton(const CSingleton &);
	const CSingleton& operator=(const CSingleton&);

protected://允许子类创建实例
	CSingleton(){
		cout << "............this is base constructor" << endl;
	}
	virtual ~CSingleton()//虚析构
	{
		cout << "............this is base disconstructor" << endl;
		if (!m_free)
		{
			m_free = true;
			lock_guard<mutex> lock(m_mutex);
			if (nullptr != m_instance)
			{
				delete m_instance;
				m_instance = nullptr;
			}
		}
	}
public:
	static inline T* Instance()
	{
		if (m_instance==nullptr)//避免多个线程锁等待
		{
			//m_mutex.lock();
			lock_guard<mutex> lock(m_mutex);
			if (m_instance==nullptr)
			{
				m_instance = new T;
			}
			//m_mutex.unlock();
		}
		return m_instance;
	}
	static inline void FreeInstance()
	{
		m_free = true;
		lock_guard<mutex> lock(m_mutex);
		if (nullptr != m_instance)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}
private:
	static T* m_instance;
	static mutex m_mutex;
	static bool m_free;
};
template<typename T>
T* CSingleton<T>::m_instance = nullptr;
template<typename T>
mutex CSingleton<T>::m_mutex;
template<typename T>
bool CSingleton<T>::m_free = false;


class CTestSingle :public CSingleton<CTestSingle>
{
public:
	CTestSingle(){
		cout << "this is subObject constructor" << endl;
	};
	~CTestSingle(){
		cout << "this is subObject Disconstructor" << endl;
	};

	void Init()
	{
		cout << "this is Init func" << endl;
	}
};
int num = 0;
void testThread()
{
	CTestSingle* ins = CTestSingle::Instance();
	cout << "thread id = " << GetCurrentThreadId() << endl;
	cout << "mun is " << ++num << endl;
}

int main()
{
	cout << "this is main func" << endl;
	thread threadArray[10];
	for (int i = 0; i < 10; ++i)
	{
		threadArray[i] = thread(testThread);
		threadArray[i].detach();
	}
	for (int i = 0; i < 10;++i)
	{
		if (threadArray[i].joinable())
		{
			threadArray[i].join();
		}
	}

	system("pause");
	return 0;
}

#endif

#if 0//单例模板 懒汉单例模式
#include <windows.h>
#include <iostream>
#include<thread>
#include<mutex>
using namespace std;
mutex m_mutex;
template<typename T>
class CSingleton2
{
public:
	static inline T* Instance()
	{
		//lock_guard<mutex> lock(m_mutex);//这个锁是不是必须要加呢？
		//cout << "thread id= " << GetCurrentThreadId() << " come to Instance............." << endl;
		printf("\n thread id= %d  come to Instance.............\n", GetCurrentThreadId());
		//Sleep(1000);
		static T instance;//构造函数只会执行一次
		//Sleep(1000);
		//cout << "thread id= " << GetCurrentThreadId() << "...............leave Instance" << endl;
		printf("\n thread id= %d  ...............leave Instance \n", GetCurrentThreadId());
		

		return &instance;
	}

protected:
	CSingleton2(){
		//cout << "this is base constructor" << endl;
		Sleep(10000);
		printf("\n thread id= %d  _ - _ - _ this is base constructor \n", GetCurrentThreadId());
	}
	~CSingleton2(){
		cout << "this is base disconstructor" << endl;
	}

private:
	CSingleton2(CSingleton2&);
	const CSingleton2& operator=(const CSingleton2&);
};
class testSingle:public CSingleton2<testSingle>
{
public:
	testSingle(){
		//cout << "this is sub constructor" << endl;
		printf("\n thread id= %d  _ + _ + _ this is base constructor \n", GetCurrentThreadId());

	}
	~testSingle(){
		cout << "this is sub discontructor" << endl;
	}
	void Init(){
		printf("\n thread id= %d  --------------------------this is init func \n", GetCurrentThreadId());

		//cout <<"thread id= "<<GetCurrentThreadId()<<" --------------------------this is init func" << endl;
	}
private:

};
int num = 0;
void testThread()
{
	//printf("\nthread id is %d\n", GetCurrentThreadId());
	testSingle* ins = testSingle::Instance();
	ins->Init();
	//cout << "thread id = " << GetCurrentThreadId() << endl;
	//cout << "mun is " << ++num << endl;
}

int main()
{
	cout << "this is main func" << endl;
	thread threadArray[20];
	for (int i = 0; i < 20; ++i)
	{
		threadArray[i] = thread(testThread);
		threadArray[i].detach();
	}
	for (int i = 0; i < 20; ++i)
	{
		if (threadArray[i].joinable())
		{
			threadArray[i].join();
		}
	}

	system("pause");
	return 0;
}

#endif

#if 0
#include <iostream>
#include <thread>
#include<windows.h>
using namespace std;


class Cnum
{
public:
	Cnum()
	{
		std::cout << "construct start" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));   //构造函数中休眠5s
		num++;
		std::cout << "construct stop" << std::endl;
	}
	void Test()
	{
		printf(" \n thread id= %d, num=%d \n", GetCurrentThreadId(), num);
		//std::cout << "id:" << std::this_thread::get_id() << ",num=" << num << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	static int num;
};


void func(void* argv)
{
	int i = *(int *)argv;
	//std::cout << i << std::endl;   //每个线程启动时，打印线程次序号
	printf(" \n start num %d \n", i);
	static Cnum a;   //内部静态成员变量的初始化时线程安全的，只有一个线程能执行初始化，其他线程会在此阻塞(实验的情况并不是阻塞的啊)
	a.Test();
	/*
	原博主说：从结果中可以看出：1）当打印“construct stop” 以后，其他线程才陆续执行Test函数  
	2） 所有线程打印的num为1。以上现象表明：当有一个线程执行static类对象的构造函数时，其他欲访问该static类对象的线程都是阻塞的。
	但从实际打印出的结果来看，并不是这样的，说明vs2013中静态变量的初始化并不是线程安全的，机一个创建静态变量时，其他线程并没有阻塞。但是在单例模式里，静态变量确实只会被初始化一次，即使是线程不安全的，会有影响吗？
	*/
}
/*

start num 0
construct start
start num 3


start num 1

thread id= 7692, num=0

thread id= 14108, num=0

start num 2

thread id= 9580, num=0

start num 4

thread id= 13340, num=0

start num 5

thread id= 13628, num=0

start num 6

thread id= 11672, num=0

start num 7

thread id= 2948, num=0

start num 8

start num 9

thread id= 10492, num=0

thread id= 8936, num=0
请按任意键继续. . . construct stop

thread id= 6180, num=1

*/

int Cnum::num = 0;     //初始化静态成员

void main()
{
	for (int i = 0; i < 10; i++)    //开启10个线程，模拟并发访问情形
	{
		std::thread t1(func, (void *)&i);
		t1.detach();
	}
	system("pause ");//暂停一下
}

#endif

