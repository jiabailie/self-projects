#include <cstdio>
#include <iostream>

using namespace std;

typedef void(*Fun)(void);

class Base1
{
public:
	virtual void f() { cout << "Base1::f" << endl; }
	virtual void g() { cout << "Base1::g" << endl; }
	virtual void h() { cout << "Base1::h" << endl; }
};

class Base2
{
public:
	virtual void f() { cout << "Base2::f" << endl; }
	virtual void g() { cout << "Base2::g" << endl; }
	virtual void h() { cout << "Base2::h" << endl; }
};

class Base3
{
public:
	virtual void f() { cout << "Base3::f" << endl; }
	virtual void g() { cout << "Base3::g" << endl; }
	virtual void h() { cout << "Base3::h" << endl; }
};

class Derived : Base1, Base2, Base3
{
public:
	virtual void f1() { cout << "Derived::f1" << endl; }
	virtual void g1() { cout << "Derived::g1" << endl; }
};

/* Derived class's virtual table : 
(int*)(&Derived) + 0: Base1::f,Base1::g,Base1::h,Derived::f1,Derived::g1
(int*)(&Derived) + 1: Base2::f,Base2::g,Base2::h
(int*)(&Derived) + 2: Base3::f,Base3::g,Base3::h
*/
int main()
{
	Derived d;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			Fun pFun = (Fun)*((int*)*((int*)(&d) + i) + j);
			if(!pFun) { break; }
			pFun();
		}
		cout << endl;
	}
	return 0;
}
