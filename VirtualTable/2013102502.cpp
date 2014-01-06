#include <cstdio>
#include <iostream>

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
	virtual void f1() { cout << "Derived::f1" << endl; }
	virtual void g1() { cout << "Derived::g1" << endl; }
	virtual void h1() { cout << "Derived::h1" << endl; }
};

// Derived class's virtual table : Base::f, Base::g, Base::h, Derived::f1, Derived::g1, Derived::h1
int main()
{
	Derived d;

	const int n = 6;

	Fun pFun[6] = {0, 0, 0, 0, 0, 0};

	cout << "d's virtual table address:" << (int*)(&d) << endl;

	cout << "d's virtual table's first entry address:" << (int*)*(int*)(&d) << endl;

	for(int i = 0; i < n; i++)
	{
		pFun[i] = (Fun)*((int*)*(int*)(&d) + i);
		pFun[i]();
	}

	return 0;
}
