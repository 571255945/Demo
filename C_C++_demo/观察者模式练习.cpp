#if 1 //Observer
#include<iostream>
#include<list>
using namespace std;

class Observer
{
public:
	virtual void Update(int) = 0;
};
class Subject
{
public:
	virtual void Attach(Observer *) = 0; //附上
	virtual void Detach(Observer *) = 0; //分离
	virtual void Notify() = 0;           //通知
};
class ConcreteObserver1:public Observer
{
public:
	ConcreteObserver1(Subject *pSubject):m_pSubject(pSubject){}
	
	void Update(int value)
	{
		cout << "ConcreteObserver1 get the update. New State:" << value << endl;
	}

private:
	Subject *m_pSubject;
};

class ConcreteObserver2:public Observer
{
public:
	ConcreteObserver2(Subject *pSubject) :m_pSubject(pSubject){}
	~ConcreteObserver2();

	void Update(int value)
	{
		cout << "ConcreteObserver2 get the update. New State:" << value << endl;
	}
private:
	Subject *m_pSubject;

};
class ConcreteSubject:public Subject
{
public:
	void Attach(Observer *pObserver)
	{
		m_ObserverList.push_back(pObserver);
	}
	void Detach(Observer *pObserver)
	{
		m_ObserverList.remove(pObserver);
	}
	void Notify()
	{
		std::list<Observer *>::iterator it = m_ObserverList.begin();
		while (it!=m_ObserverList.end())
		{
			(*it)->Update(m_iState);
			++it;
		}
	}
	
	void SetState(int state)
	{
		m_iState = state;
	}

private:
	std::list<Observer *> m_ObserverList;
	int m_iState;
};

int main()
{
	//create subject
	ConcreteSubject *pSubject = new ConcreteSubject();
	//create observer
	Observer *pObserver1 = new ConcreteObserver1(pSubject);
	Observer *pObserver2 = new ConcreteObserver2(pSubject);

	pSubject->SetState(12);

	pSubject->Attach(pObserver1);
	pSubject->Attach(pObserver2);

	pSubject->Notify();

	pSubject->Detach(pObserver1);
	pSubject->SetState(30);
	pSubject->Notify();

	delete pSubject; pSubject = nullptr;
	delete pObserver1; pObserver1 = nullptr;
	delete pObserver2; pObserver2 = nullptr;

	system("pause");
	return 0;
}


#endif
