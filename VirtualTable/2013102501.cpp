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

// Base class's virtual table : Base::f, Base::g, Base::h
int main()
{
	Base b;
	const int n = 3;

	Fun pFun[n] = {0, 0, 0};
	
	cout << "virtual table address:" << (int*)(&b) << endl;

	cout << "the first entry of virtual table's address:" << (int*)*(int*)(&b) << endl;

	for(int i = 0; i < n; i++)
	{
		pFun[i] = (Fun)*((int*)*(int*)(&b) + i);
		pFun[i]();
	}

	return 0;
}
