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
	void init();        // ��ʼ��ջ
	bool empty();       // �ж�ջ�Ƿ�Ϊ��
	T getTop();         // ��ȡջ��Ԫ��(����ջ)
	void push(T x);     // ��ջ
	T pop();            // ��ջ
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
		cout << "ջΪ�գ�\n";
		exit(1);
	}
	return this->data[this->top - 1];
}

template<class T>
void MyStack<T>::push(T x)
{
	if (this->top == MAX)
	{
		cout << "ջ������\n";
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
		cout << "ջΪ��! \n";
		exit(1);
	}

	T e = this->data[this->top - 1];
	this->top--;
	return e;
}

