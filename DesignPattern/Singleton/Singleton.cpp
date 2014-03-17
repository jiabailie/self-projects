/**
 * Assume there is a server, and it maintains an array, which records the status of n different objects.
 * eg. if array[i], which means the object i has been assigned; else object i is still available.
 * Operations:
 * getVar(): it will return an index of object, and setting it is unavailable.
 * setVar(int): an object is been given back, and setting its status is available.
 * Using the design pattern of Singleton to realize this application.
 */
#include <cstdio>
#include <iostream>
#include <cstring>

using namespace std;

const int n = 100;

class Singleton
{
private:
	Singleton();
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
	static Singleton* instance;
	bool var[n];
public:	
	int getVar();
	void setVar(int);
	static Singleton* GetInstance();
};

Singleton::Singleton()
{
	memset(this->var, false, sizeof(this->var));
	cout << "Singleton constructor." << endl;
}

Singleton* Singleton::instance = 0;

Singleton* Singleton::GetInstance()
{
	if(!instance)
	{
		instance = new Singleton();
	}
	return instance;
}

int Singleton::getVar()
{
	int idx = rand() % n;
	while(var[idx])
	{
		idx = rand() % n;
	}
	var[idx] = true;
	return idx;
}

void Singleton::setVar(int idx)
{
	if(var[idx])
	{
		var[idx] = false;
	}
}

int main()
{
	Singleton *s1 = Singleton::GetInstance();
	Singleton *s2 = Singleton::GetInstance();

	if(s1 == s2)
	{
		cout << "s1 == s2" << endl;
	}
	else
	{
		cout << "s1 != s2" << endl;
	}

	cout << "s1 var = " << s1->getVar() << endl;
	cout << "s2 var = " << s2->getVar() << endl;

	s1->setVar(54);

	cout << "s1 var = " << s1->getVar() << endl;
	cout << "s2 var = " << s2->getVar() << endl;

	delete Singleton::GetInstance();

	s1 = 0;
	s2 = 0;

	return 0;
}