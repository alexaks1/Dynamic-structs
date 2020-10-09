// 1 лаба по алгоритмам.cpp 
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

template <typename T>
class ArrayList
{
private:
	T* ptr;//указатель на 1 эл. массива
	unsigned int size;//размер массива, задаваемый при создании 
	unsigned int capacity;//размер массива с учетом дополнительно выделенного места

public:
	ArrayList(unsigned int size = 0)//конструктор 
	{
		this->size = size;
		capacity = 1;
		while (capacity <= size)
		{
			capacity *= 2;
		}
		ptr = (T*)malloc(capacity * sizeof(T));
	}
	~ArrayList()//деструктор
	{
		size = 0;
		capacity = 0;
		free(ptr);
		ptr = nullptr;
	}
	unsigned int GetSize()//геттер для размера массива
	{
		return size;
	}
	unsigned int GetCapacity()//геттер для размера массива, с учётом доп. места
	{
		return capacity;
	}
	void push_back(T data)//вставка в конец
	{
		++size;
		if (size > capacity)
		{
			capacity *= 2;
			ptr = (T*)realloc(ptr, capacity * sizeof(T));
		}
		*(ptr + size - 1) = data;
	}
	void insert(unsigned int index, T data)//вставка со сдвигом элементов
	{
		if (index < GetSize())
		{
			++size;
			if (size > capacity)
			{
				capacity *= 2;
				ptr = (T*)realloc(ptr, capacity * sizeof(T));
			}
			for (unsigned int i = size - 1; i >= index; --i)//идём обратным циклом до индекса, чтобы не париться с буфферами(тк пишем из предыдущего в следующий)
			{
				if (i > index)
					* (ptr + i) = *(ptr + i - 1);
				if (i == index)
					* (ptr + i) = data;
			}
		}
		else
			push_back(data);
	}
	void removeAt(unsigned int index)//удаление по индексу
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			if (i > index)
			{
				*(ptr + i - 1) = *(ptr + i);//копируем в каждый предыдущий элемент каждый следующий
			}
			if (i == size - 1)
				* (ptr + i) = T();//выкидываем значение последнего элемента, так как оно теперь хранится в предпоследнем
		}
		--size;
		if (size < capacity / 4)
		{
			capacity /= 2;
			ptr = (T*)realloc(ptr, capacity * sizeof(T));
		}
	}
	void pop_back()//удаление последнего элемента массива
	{
		removeAt(size - 1);
	}
	T& operator[](const unsigned int index)//перегрузка оператора "квадратные скобки" для обращения к эл-у массива с индексом index
	{
		if (index < size)
			return (*(ptr + index));
	}
};


template <typename T>
class Node//узел односвязного списка(элемент)
{
public:
	Node* pNextAddress;//указатель на след. элемент
	T data;//данные элемента

	//конструктор
	Node(T data = T(), Node* pNextAddress = nullptr)//по умолчанию: данные по умолчанию и указатель на null
	{
		this->data = data;
		this->pNextAddress = pNextAddress;
	}
};

template <typename T>
class LinkedList
{
private:
	unsigned int size;//кол-во эл-ов в списке
	Node<T>* head;//указатель на первый элемент списка

public:
	T& operator[](const unsigned int index)//перегрузка оператора "квадратные скобки" для обращения к эл-у списка с индексом index
	{
		Node<T>* currentNode = head;
		int i = 0;//счётчик
		while (currentNode != nullptr)
		{
			if (i == index)
			{
				return currentNode->data;
			}
			currentNode = currentNode->pNextAddress;
			++i;
		}
	}
	int GetSize()//геттер для размера списка
	{
		return size;
	}
	LinkedList()//конструктор по умолчанию
	{
		size = 0;
		head = nullptr;
	}
	~LinkedList()//деструктор 
	{
		clear();
	}
	void push_back(T data)//добавить данные в конец списка
	{
		if (head == nullptr)//если первого эл-а нет, то создаём его
		{
			head = new Node<T>(data);
		}
		else
		{
			Node<T>* currentNode = head;

			while (currentNode->pNextAddress != nullptr)
			{
				currentNode = currentNode->pNextAddress;//присваиваем каждый след. адрес текущему элементу, пока не nullptr
			}
			currentNode->pNextAddress = new Node<T>(data);
		}
		++size;
	}
	void push_front(T data)//добавить данные в начало списка
	{
		head = new Node<T>(data, head);
		++size;
	}
	void pop_front()//удаление первого эл-а списка
	{
		Node<T>* buffer = head;//типа буффер, который удаляем сразу после присваивания значения следующего эл-а для того, чтобы точно знать где лежал 1 эл в памяти
		head = head->pNextAddress;
		delete buffer;
		--size;
	}
	void pop_back()//удаление последнего эл-а списка
	{
		removeAt(size - 1);
	}
	void clear()//удаление списка и очистка памяти
	{
		while (size != 0)
			pop_front();
	}
	void insert(T data, int index)//вставка нового элемента
	{
		if (index == 0)//если вставляем в начало списка
		{
			push_front(data);
		}
		else
		{
			Node<T>* previous = head;

			for (int i = 0; i < index - 1; ++i)//логика описывает поиск предыдущего элемента(который д.б. перед новым)
			{
				previous = previous->pNextAddress;
			}

			Node<T>* newNode = new Node<T>(data, previous->pNextAddress);//теперь новый элемент указывает на тот, который шёл после предыдущего
			previous->pNextAddress = newNode;//теперь предыдущий элемент указывает на новый
			++size;
		}
	}
	void removeAt(int index)//удаление элемента по индексу
	{
		if (index == 0)//если удаляем из начала списка
		{
			pop_front();
		}
		else
		{
			Node<T>* previous = head;

			for (int i = 0; i < index - 1; ++i)//логика описывает поиск предыдущего элемента(который д.б. перед удаляемым)
			{
				previous = previous->pNextAddress;
			}
			Node<T>* removable = previous->pNextAddress;
			previous->pNextAddress = removable->pNextAddress;//теперь предыдущий элемент указывает на новый
			delete removable;
			--size;
		}
	}
};

template <typename T>
class Stack
{
private:
	unsigned int size;//кол-во эл-ов в списке
	Node<T>* last;//указатель на последний добавленный элемент в стек
public:
	Stack()//конструктор по умолчанию
	{
		size = 0;
		last = nullptr;
	}
	~Stack()//деструктор 
	{
		clear();
	}
	T get_last()//получаем последний добавленный элемент в стек
	{
		return last->data;
	}
	unsigned int GetSize()//геттер для размера стека
	{
		return size;
	}
	void push_back(T data)//добавить данные в конец стека
	{
		last = new Node<T>(data, last);
		++size;
	}
	void pop_back()//удаление последнего добавленного эл-а стека
	{
		Node<T>* buffer = last;//типа буффер, который удаляем сразу после присваивания значения следующего эл-а для того, чтобы точно знать где лежал предыдущий эл в памяти
		last = last->pNextAddress;//делаем предпоследний элемент последним
		delete buffer;//удаляем последний элемент из памяти
		--size;
	}
	void clear()//удаление стека из памяти
	{
		while (size != 0)
			pop_back();
	}
};

//Функция для определения приоритетности операции
int priority(char Operator)//выше возвращаемое значение - выше приоритет
{
	if ((Operator == '*') || (Operator == '/'))
		return 2;
	if ((Operator == '+') || (Operator == '-'))
		return 1;
	return 0;
	//тут мб ещё дофига операторов
}
ArrayList<char> ReadLine()//Функция для чтения и записи только разрешённых символов
{
	ArrayList<char> input;
	char symbol;
	do
	{
		symbol = _getch();
		if ((symbol >= '0') && (symbol <= '9') || ((symbol >= 'a') && (symbol <= 'z')) || ((symbol >= '(') && (symbol <= '+')) || (symbol == '-') || (symbol == '/') || (symbol == ' '))
		{
			input.push_back(symbol);
			cout << symbol;
		}
		if ((symbol == 8) && (input.GetSize() > 0))
		{
			input.pop_back();
			cout << "\b \b";
		}
	} while (symbol != 13);
	cout << endl;
	input.push_back('\0');
	return input;
}
//В функции main, собственно, обратная польская запись
int main()
{
	setlocale(0, "");

	ArrayList<char> input;//входная строка
	ArrayList<char> output;//выходная строка
	Stack<char> stek;//стек для хранения операторов

	//Дальше просто ввод разрешённых символов и их запись в input
	cout << "Введите входную строку: ";
	input = ReadLine();

	/*
	//Вывод входной строки input
	cout << endl << "Входная строка: ";
	for (int i = 0; i < input.length(); ++i)
		cout << input[i];
	*/

	//Сам алгоритм:
	for (int i = 0; i < input.GetSize(); ++i)// length()+1, чтобы захватить терминирующий ноль
	{
		//Если символ является числом или постфиксной функцией(например, !— факториал), добавляем его к выходной строке.(while, чтобы впихнуть число целиком и сразу)
		if ((input[i] >= '0') && (input[i] <= '9') || (input[i] == '!'))
		{
			while ((input[i] >= '0') && (input[i] <= '9'))
			{
				output.push_back(input[i]);
				++i;
			}
			output.push_back(' ');//добавляем пробел, чтобы разделить числа между собой
		}

		//Если символ является префиксной функцией(например, sin — синус), помещаем его в стек.
		//if... - если надо, то сделаю

		//Если символ является открывающей скобкой, помещаем его в стек.
		if (input[i] == '(')
			stek.push_back(input[i]);

		//Если символ является закрывающей скобкой:
		//До тех пор, пока верхним элементом стека не станет открывающая скобка, выталкиваем элементы из стека в выходную строку.При этом открывающая скобка удаляется из стека, но в выходную строку не добавляется.Если стек закончился раньше, чем мы встретили открывающую скобку, это означает, что в выражении либо неверно поставлен разделитель, либо не согласованы скобки.
		if (input[i] == ')')
		{
			while ((stek.GetSize() > 0) && (stek.get_last() != '('))
			{
				output.push_back(stek.get_last());
				output.push_back(' ');//разделительный пробел
				stek.pop_back();
			}
			stek.pop_back();
		}

		/*
		Если символ является бинарной операцией о1, тогда:
		1) пока на вершине стека префиксная функция…
		… ИЛИ операция на вершине стека приоритетнее o1
			… ИЛИ операция на вершине стека левоассоциативная с приоритетом как у o1 - я понятия не имею, что это значит в нашем случае
			… выталкиваем верхний элемент стека в выходную строку;
		2) помещаем операцию o1 в стек.
		*/
		if ((input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/'))
		{
			while ((stek.GetSize() > 0) && (priority(stek.get_last()) > priority(input[i]) && ((stek.get_last() == '+') || (stek.get_last() == '-') || (stek.get_last() == '*') || (stek.get_last() == '/'))))
			{
				output.push_back(stek.get_last());
				output.push_back(' ');//разделительный пробел
				stek.pop_back();
			}
			stek.push_back(input[i]);
		}

		//Когда входная строка закончилась, выталкиваем все символы из стека в выходную строку.В стеке должны были остаться только символы операций; если это не так, значит в выражении не согласованы скобки.
		if (input[i] == '\0')
		{
			while (stek.GetSize() > 0)
			{
				output.push_back(stek.get_last());
				output.push_back(' ');//разделительный пробел
				stek.pop_back();
			}
		}
	}
	//cout << "Выходная строка в постфиксной форме: " << output;
	for (unsigned int i = 0; i < output.GetSize(); ++i)
		cout << output[i];
	return 0;
}

