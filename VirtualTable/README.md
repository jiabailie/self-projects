Reference: http://blog.csdn.net/haoel/article/details/1948051/

These files are some examples to see the virtual table's structure under 5 different situations.

1)

		class Base
		{
		public:
			virtual void f() { cout << "Base::f" << endl; }
			virtual void g() { cout << "Base::g" << endl; }
			virtual void h() { cout << "Base::h" << endl; }
		};


2)

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

3)

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

4)

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
		
5)

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
			virtual void f() { cout << "Derived::f" << endl; }
			virtual void g1() { cout << "Derived::g1" << endl; }
		};
		
