#include <cstdio>
#include <iostream>
#include <cstring>

using namespace std;

typedef void(*Fun)(void);

class Base
{
public:
	virtual void f() { cout << "Base::f" << endl; }
	virtual void g() { cout << "Base::g" << endl; }
	virtual void h() { cout << "Base::h" << endl; }
};

class Derived : Base
{
public:
	virtual void f() { cout << "Derived::f" << endl; }
	virtual void g1() { cout << "Derived::g1" << endl; }
	virtual void h1() { cout << "Derived::h1" << endl; }
};

// Derived class's virtual table : Derived::f, Base::g, Base::h, Derived::g1, Derived::h1
int main()
{
	int i = 0;
	const int n = 6;

	Derived d;

	Fun pFun[n];
	for(i = 0; i < n; i++) { pFun[i] = 0; }

	cout << "d's virtual table's address:" << (int*)(&d) << endl;
	cout << "d's virtual table's first entry's address:" << (int*)*(int*)(&d) << endl;

	for(i = 0; i < n; i++)
	{
		pFun[i] = (Fun)*((int*)*(int*)(&d) + i);
		if(!pFun[i])
		{
			break;
		}
		pFun[i]();
	}

	return 0;
}
