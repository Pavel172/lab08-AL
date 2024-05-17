#include <iostream>
using namespace std;

template<class T>
class MyShared //Шаблонный класс "умного" указателя с возможностью копирования 
{
private:
	T* ptr;
public:
	int* count; //Счётчик для ссылок
	MyShared() //Пустой конструктор
	{
		ptr = nullptr;
		count = new int(0);
	}
	MyShared(T* ptr) //Конструктор с начальным значением
	{
		this->ptr = ptr;
		this->count = new int(1);
	}
	MyShared(const MyShared& Shared) //Конструктор копирования 
	{
		this->ptr = Shared.ptr;
		this->count = Shared.count;
		++(*count);
	}
	MyShared(MyShared&& Shared) //Конструктор перемещения
	{
		this->ptr = Shared.ptr;
		this->count = Shared.count;
		Shared.ptr = nullptr;
		Shared.count = new int(0);
	}
	T& operator=(const MyShared& Shared) //Оператор присваивания копирования 
	{
		~MyShared();
		this->ptr = Shared.ptr;
		this->count = Shared.count;
		if (this->count != nullptr)
		{
			(*this->count)++;
		}
		return *this;
	}
	T& operator=(MyShared&& Shared) //Оператор присваивания перемещения
	{
		~MyShared();
		this->ptr = Shared.ptr;
		this->count = Shared.count;
		Shared.ptr = nullptr;
		Shared.count = new int(0);
		return *this;
	}
	T* operator->() //Перегрузка оператора ->
	{
		return ptr;
	}
	T& operator* () //Перегрузка оператора *
	{
		return *ptr;
	}
	T* get() const //Метод get, который возвращает "сырой указатель"
	{
		return ptr;
	}
	~MyShared() //Деструктор
	{
		--(*count);
		if (*count == 0)
		{
			delete ptr;
			delete count;
			count = nullptr;
		}
	}
};


template<typename T, typename...Args>
MyShared<T> Make_MyShared(Args&&... args) { //Шаблонная функция с переменным числом параметров
	return MyShared<T>(new T(forward<Args>(args)...));
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

int main()
{
	setlocale(LC_ALL, "Rus");
	MyShared<MyPoint> ms1 = Make_MyShared<MyPoint>(5, 6); //Создание указателя на объект своего класса, имеющего конструктор с параметрами с помощью функции Make_MyUnique, получающей параметры как конструктора своего класс (MyPoint)
	ms1->print(); //Печать координат точки, на которую указывает умный указатель
	cout << "Кол-во ссылок в ms1:" << *ms1.count << endl;
	MyShared<MyPoint> ms2 = ms1; 
	ms2->print();
	cout << "Кол-во ссылок в ms2:" << *ms2.count << endl;
	return 0;
}