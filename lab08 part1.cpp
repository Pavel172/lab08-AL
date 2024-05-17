#include <iostream>
using namespace std;

template<class T>
class MyUnique //Шаблонный класс "умного" указателя
{
private:
	T* ptr;
public:
	MyUnique() //Пустой конструктор
	{
		ptr = nullptr;
	}
	MyUnique(T*ptr) //Конструктор с начальным значением
	{
		this->ptr = ptr;
	}
	MyUnique(const MyUnique& Unique) = delete; //Конструктор копирования (его не может быть, тк запрещено создание копий)
	MyUnique(MyUnique&& Unique) //Конструктор перемещения
	{
		this->ptr = Unique.ptr;
		Unique.ptr = nullptr;
		delete Unique.ptr;
	}
	T& operator=(const MyUnique& Unique) = delete; //Оператор присваивания копирования (его не может быть, тк запрещено создание копий)
	T& operator=(MyUnique&& Unique) //Оператор перемещения
	{
		this->ptr = Unique.ptr;
		Unique.ptr = nullptr;
		delete Unique.ptr;
		return *this;
	}
	T * operator->() //Перегрузка оператора ->
	{
		return ptr;
	}
	T& operator* () //Перегрузка оператора *
	{
		return *ptr;
	}
	T * get() const //Метод get, который возвращает "сырой указатель"
	{
		return ptr;
	}
	~MyUnique() //Деструктор
	{
		delete ptr;
	}
};

template<class T1>
class MyShared //Шаблонный класс "умного" указателя с возможностью копирования
{
private:
	T1* ptr;
public:
	int* count; //Счётчик для ссылок
	MyShared() //Пустой конструктор
	{
		ptr = nullptr;
		count = nullptr;
	}
	MyShared(T1* ptr) //Конструктор с начальным значением
	{
		this->ptr = ptr;
		count = new int(1);
	}
	MyShared(const MyShared& Shared) //Конструктор копирования
	{
		this->ptr = Shared.ptr;
		++(*n);
	};
	MyShared(MyShared&& Shared) //Конструктор перемещения
	{
		this->ptr = Shared.ptr;
		Shared.ptr = nullptr;
		Shared.count = new int(0);
	}
	T1& operator=(const MyShared& Shared) //Оператор присваивания копирования
	{
		if (this != &Shared) {
			if ((*count) - 1 == 0) {
				delete ptr;
				delete count;
			}
			ptr = Shared.ptr;
			count = Shared.count;
			(*count)++;
		}
		return *this;
	}
	};
	T1& operator=(MyShared&& Shared) //Оператор перемещения
	{
		if (this != &Shared) {
			if ((*count) - 1 == 0) {
				delete ptr;
				delete count;
			}
			ptr = Shared.ptr;
			count = Shared.count;
			Shared.ptr = nullptr;
			Shared.count = new int(0);
		}
		return *this;
	}
	T1* operator->() //Перегрузка оператора ->
	{
		return ptr;
	}
	T1& operator* () //Перегрузка оператора *
	{
		return *ptr;
	}
	T1* get() const //Метод get, который возвращает "сырой указатель"
	{
		return ptr;
	}
	~MyShared() //Деструктор
	{
		if((*n)-1 ==0)
		{
			delete ptr;
			delete count;
		}
	}
};


template<typename T, typename...Args> 
MyUnique<T> Make_MyUnique(Args&&... args) { //Шаблонная функция с переменным числом параметров
	return MyUnique<T>(new T(forward<Args>(args)...));
}

class MyPoint //Класс - точка на плоскости
{
public:
	int x, y;
	MyPoint() //Пустой конструктор
	{
		x = 0;
		y = 0;
	}
	MyPoint(int x, int y) //Конструктор с начальным значением
	{
		this->x = x;
		this->y = y;
	}
	void print() //Метод печати координат точки на плоскости
	{
		cout << "MyPoint(int x, int y): x=" << x << " y=" << y << endl;
	}
	~MyPoint() {}; //Деструктор
};

template <class T1, class... Args>
MyShared<T1> Make_MyShared(Args&&... args) {
	return MyShared<T1>(new T1(forward<Args>(args)...));
}

int main()
{
	setlocale(LC_ALL, "Rus");
	cout << "MyUnique:" << endl;
	MyUnique<MyPoint> mu1 = Make_MyUnique<MyPoint>(3, 4); //Создание указателя на объект своего класса, имеющего конструктор с параметрами с помощью функции Make_MyUnique, получающей параметры как конструктора своего класс (MyPoint)
	mu1->print(); //Печать координат точки, на которую указывает умный указатель
	MyUnique<MyPoint> mu2 = move(mu1); //Демонстрация оператора перемещения
	mu2->print();
	cout << "MyShared:" << endl;
	MyShared<MyPoint> ms1 = Make_MyShared<MyPoint>(5, 6);
	ms1->print();
	MyShared<MyPoint> ms2 = ms1;
	ms2->print();
	return 0;
}
