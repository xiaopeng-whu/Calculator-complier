#pragma once
#include <iostream>
#define MAX 100
using namespace std;
template <class T>
class MyStack
{
public:
	T data[MAX];
	int top;

public:
	void init();        // 初始化栈
	bool empty();       // 判断栈是否为空
	T getTop();         // 读取栈顶元素(不出栈)
	void push(T x);     // 进栈
	T pop();            // 出栈
};

template<class T>
void MyStack<T>::init()
{
	this->top = 0;
}

template<class T>
bool MyStack<T>::empty()
{
	return this->top == 0 ? true : false;
}

template<class T>
T MyStack<T>::getTop()
{
	if (empty())
	{
		cout << "栈为空！\n";
		exit(1);
	}
	return this->data[this->top - 1];
}

template<class T>
void MyStack<T>::push(T x)
{
	if (this->top == MAX)
	{
		cout << "栈已满！\n";
		exit(1);
	}
	this->data[this->top] = x;
	this->top++;
}

template<class T>
T MyStack<T>::pop()
{

	if (this->empty())
	{
		cout << "栈为空! \n";
		exit(1);
	}

	T e = this->data[this->top - 1];
	this->top--;
	return e;
}

