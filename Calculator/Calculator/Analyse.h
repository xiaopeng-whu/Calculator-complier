#pragma once
#include <fstream>
#include "MyStack.h"
#include <map> 
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

const double UIN = 0.001; //uninitialized identifier tag 
const double UDF = 0.002; //undefined identifier tag
const double ACCURACY = 0.01; //比较两个double类型数的大小时计算差值使用的精度

using namespace std;
bool isoperator(char op);                                       // 判断是否为运算符
int priority(char op);                                          // 求运算符优先级
double postfix(string pre, map<string, int> mymap1, map<string, float> mymap2, int* i);     // 把中缀表达式转换为后缀表达式
double read_number(char str[], int* i, map<string, int> mymap1, map<string, float> mymap2); // 将数字字符串转变成相应的数字，标识符转换为数字
double postfix_value(char post[], map<string, int> mymap1, map<string, float> mymap2, int errl);      //对后缀表达式求值

map<string, int> Identities_int;//存放标识符的声明和定义
map<string, float> Identities_float;
map<string, int>::iterator iter;

vector<string> Uninitialize;  //存储未初始化的变量
//判断是否为操作符
bool isoperator(char op)
{
	switch (op)
	{
	case '+':
	case '-':
	case '*':
	case '/':
		return 1;
	default:
		return 0;
	}
}

//设置运算符优先级
int priority(char op)
{
	switch (op)
	{
	case '#':
		return -1;
	case '(':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		return -1;
	}
}

//把中缀表达式转换为后缀表达式，返回后缀表达式的长度（包括空格）
double postfix(string pre, map<string, int> mymap1, map<string, float> mymap2, int* i, int errl)
{
	char post[MAX];      //后缀表达式数组
	int j = 0;
	MyStack<char> stack;
	stack.init();        // 初始化存储操作符的栈
	stack.push('#');     // 首先把结束标志‘;’放入栈底

	while (pre[*i] != ';')
	{
		if ((pre[*i] >= '0' && pre[*i] <= '9') || pre[*i] == '.' || (pre[*i] >= 'a' && pre[*i] <= 'z') || (pre[*i] >= 'A' && pre[*i] <= 'Z')) // 遇到数字、字母和小数点直接写入后缀表达式
		{
			post[j++] = pre[*i];
		}
		else if (pre[*i] == '(')        // 遇到“（”不用比较直接入栈
			stack.push(pre[*i]);
		else if (pre[*i] == ')')        // 遇到右括号将其对应左括号后的操作符（操作符栈中的）全部写入后缀表达式
		{
			while (stack.getTop() != '(')
			{
				post[j++] = stack.pop();
			}
			stack.pop();                // 将“（”出栈，后缀表达式中不含小括号
		}
		else if (isoperator(pre[*i]))
		{
			post[j++] = ' ';            // 用空格分开操作数

			while (priority(pre[*i]) <= priority(stack.getTop()))
			{
				// 当前的操作符小于等于栈顶操作符的优先级时，将栈顶操作符写入到后缀表达式，重复此过程
				post[j++] = stack.pop();
			}
			stack.push(pre[*i]);        // 当前操作符优先级大于栈顶操作符的优先级，将该操作符入栈
		}

		(*i)++;
	}
	while (stack.top) // 将所有的操作符加入后缀表达式
	{
		post[j++] = stack.pop();
	}
	double result = postfix_value(post, mymap1, mymap2, errl);
	return result;
}
// 将数字字符串转变成相应的数字，标识符转换为数字
double read_number(char str[], int* i, map<string, int> mymap1, map<string, float> mymap2, int errl)
{
	double x = 0.0; //一定有返回值，无法处理右值中有未初始化变量或未定义变量的情况
	int k = 0;
	bool f = false;
	while (str[*i] >= '0' && str[*i] <= '9')  // 处理整数部分
	{
		x = x * 10 + (str[*i] - '0');
		(*i)++;
	}

	if (str[*i] == '.') // 处理小数部分
	{
		(*i)++;
		while (str[*i] >= '0' && str[*i] <= '9')
		{
			x = x * 10 + (str[*i] - '0');
			(*i)++;
			k++;
		}
	}
	while (k != 0)
	{
		x /= 10.0;
		k--;
	}
	while ((str[*i] >= 'a' && str[*i] <= 'z') || (str[*i] >= 'A' && str[*i] <= 'Z'))//处理标识符
	{
		int len = *i;
		string token = "";
		do {
			token = token + str[*i];
			(*i)++;
		} while ((str[*i] >= 'a' && str[*i] <= 'z') || (str[*i] >= 'A' && str[*i] <= 'Z') || (str[*i] >= '0' && str[*i] <= '9'));
		int is_uninitialized = std::count(Uninitialize.begin(), Uninitialize.end(), token); //是否在未初始化vector中
		//cout << token << is_uninitialized << endl;
		if (is_uninitialized)
		{
			cout << "Error:line " << errl << " uninitialized identifier" << endl;
			return UIN;
		}
		else
		{
			if (Identities_int.count(token))
			{
				x = mymap1[token];
			}
			else if (Identities_float.count(token))
			{
				x = mymap2[token];
			}
			else
			{
				cout << "Error:line " << errl << " undefined identifier" << endl;  //右值
				return UDF;
			}
		}

	}
	//cout << x << endl;
	return x;
}
//对后缀表达式求值
double postfix_value(char post[], map<string, int> mymap1, map<string, float> mymap2, int errl)
{
	MyStack<double> stack;    // 操作数栈
	stack.init();
	int i = 0;
	double x1, x2;

	while (post[i] != '#')
	{
		//cout << post[i] << endl;
		if (post[i] >= '0' && post[i] <= '9' || (post[i] >= 'a' && post[i] <= 'z') || (post[i] >= 'A' && post[i] <= 'Z'))
		{
			double value = read_number(post, &i, mymap1, mymap2, errl);
			//cout << value << endl;
			//cout << UDF << endl;
			//cout << (read_number(post, &i, mymap1, mymap2, errl) == UDF) << endl;  //double值如何相比？给定精度取差值。
			//cout << (value - UIN) << endl;
			if (fabs(value - UIN) < ACCURACY)
			{
				//cout << "here1" << endl;
				return UIN;
			}
			else if (fabs(value - UDF) < ACCURACY)
			{
				//cout << "here2" << endl;
				return UDF;
			}
			else
			{
				//cout << "here3" << endl;
				//cout << value << endl;
				stack.push(value);  //read_number后使用value即可，否则会返回0
			}
		}
		else if (post[i] == ' ')
			i++;
		else if (post[i] == '+')
		{
			x2 = stack.pop();
			x1 = stack.pop(); 
			//cout << x1 << endl;
			//cout << x2 << endl;
			stack.push(x1 + x2);
			//cout << (x1+x2) << endl;
			i++;
		}
		else if (post[i] == '-')
		{
			x2 = stack.pop();
			x1 = stack.pop();
			stack.push(x1 - x2);
			i++;
		}
		else if (post[i] == '*')
		{
			x2 = stack.pop();
			x1 = stack.pop();
			stack.push(x1 * x2);
			i++;
		}
		else if (post[i] == '/')
		{
			x2 = stack.pop();
			x1 = stack.pop();
			if (x2 == 0)
			{
				cout << "Error:line " << errl << " divided by zero!" << endl;
				//cout << "divided by zero!" << endl;
			}
			stack.push(x1 / x2);
			i++;
		}
	}
	double result = stack.getTop();
	//cout << result << endl;
	return result;
}

//声明变量
void scanner(string s, int flag, int errl)
{
	//cout << "scanner_in" << endl;
	//cout << flag << endl;
	//int i = 1; flag代表是哪种类型的
	if (flag == 1)
	{
		//先找到分号的位置，然后提取出来注意substr函数有点不一样
		int end_sign;
		if (s.find(';'))
			end_sign = s.find_first_of(';');
		else if (s.find('.'))
			end_sign = s.find_last_of('.');
		//int fenhao = s.find_first_of(';');
		string var = s.substr(4, end_sign - strlen("int") - 1); //Attention!
		//cout << var << endl;
		if (Identities_int.count(var))
		{
			cout << "Error:line " << errl << " 变量" << var << "重复定义！" << endl;
		}
		else
		{
			//cout << "int_insert" << endl;
			Identities_int.insert(pair<string, int>(var, 0));
			Uninitialize.push_back(var);
			//cout << Identities_int.count(var) << endl;
		}
	}
	if (flag == 2)
	{
		int end_sign;
		if (s.find(';'))
			end_sign = s.find_first_of(';');
		else if (s.find('.'))
			end_sign = s.find_last_of('.');
		//int fenhao = s.find_last_of(';');
		string var = s.substr(6, end_sign - strlen("float") - 1);
		if (Identities_float.count(var))
		{
			cout << "Error:line " << errl << " 变量" << var << "重复定义！" << endl;
		}
		else
		{
			Identities_float.insert(pair<string, float>(var, 0.0));
			Uninitialize.push_back(var);
		}
	}
	//cout << "scanner_out" << endl;
}
//变量赋值
void eval(string line, int errl)
{
	//line = line.substr(line.find('=') + 1).substr(0, line.find_last_of(';'));
	//cout << "eval_in" << endl;
	string var = line.substr(0, line.find('=')); 

	map<string, int>::iterator iter;

	//cout << var << endl;
	//cout << Identities_int.count(var) << endl;
	if (Identities_int.count(var))
	{
		int p = line.find('=') + 1;
		double value1 = postfix(line, Identities_int, Identities_float, &p, errl);
		//cout << value1 << endl;
		int value = value1; //int会把错误类型变成0
		if (fabs(value1 - UIN) < ACCURACY || fabs(value1 - UDF) < ACCURACY) //判断所赋右值是否有效
			return;
		else {
			//Identities_int.insert(pair<string, int>(var, value));
			Identities_int[var] = value;
			//cout << "eval_value_in" << endl;
			for (vector<string>::iterator it = Uninitialize.begin(); it != Uninitialize.end(); )
			{
				if (*it == var)
				{
					it = Uninitialize.erase(it); //不能写成arr.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		//cout << "done1" << endl;
	}
	else if (Identities_float.count(var))
	{
		int p = line.find('=') + 1;
		double value1 = postfix(line, Identities_int, Identities_float, &p, errl);
		//cout << value1 << endl;
		float value = value1;
		if (value1 == UIN || value1 == UDF)
			return;
		else {
			//Identities_int.insert(pair<string, float>(var, value));
			Identities_float[var] = value;
			for (vector<string>::iterator it = Uninitialize.begin(); it != Uninitialize.end(); )
			{
				if (*it == var)
				{
					it = Uninitialize.erase(it); //不能写成arr.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		//cout << "done2" << endl;
	}
	else
	{
		cout << "Error:line " << errl << " undefined identifier" << endl;   //左值
	}
	//cout << "eval_out" << endl;
}
//打印并处理文件的每一行
void analyse(string fPath) {
	int flag_is_int;
	string path = fPath;
	ifstream in2(path, ios::in);
	string line = "";
	int errl = 0, errPsn = 0;//错误所在的行和位置
	//先把源代码打印一遍
	int line_num = 0;
	while (!in2.eof()) {
		getline(in2, line);
		line_num++;
		if (line != "")
			cout << line_num << "  " << line << endl;
	}
	in2.close();
	ifstream in(path, ios::in);
	cout << "-----------------------------------------" << endl;
	//按行读取文件
	while (!in.eof()) {
		getline(in, line);
		errl++;
		if (line != "") {
			//cout << line << endl;
			//将注释去除
			if (line.find("#")) //只按一个'/'查找（why？），导致有除号的行会出错，暂时用'#'代替。
			{
				line = line.substr(0, line.find_first_of("#")); //必须赋值，单独substr并不会改变line
				//cout << line << endl;
			}
			//判断结尾是否是";"
			if (line[line.length() - 1] != ';') {
				//errPsn = line.length();
				//cout << "Error(line " << errl << ",position " << errPsn << "): 应输入';'" << endl;
				cout << "Error:line " << errl << " 末尾缺少';'" << endl;
				//system("pause");
				if (line[line.length() - 1] == '.') {
					//将处理部分在这里copy一份，暂时没想到更好的逻辑关系解决..（目测应该先判断末尾为'.'，再判断末尾为';'）
					if (line.substr(0, 3) == "int")
					{
						if (line.find('=') != -1)
						{
							//说明在赋值
							cout << "Error:line " << errl << " 暂时不支持声明变量的时候赋值" << endl;
						}
						else
						{
							flag_is_int = 1;
							scanner(line, flag_is_int, errl);
						}
					}
					else if (line.substr(0, 5) == "float") {
						if (line.find('=') != -1)
						{
							cout << "Error:line " << errl << " 暂时不支持声明变量的时候赋值" << endl;
						}
						else
						{
							//flag_is_int=2代表浮点数,然后开始解析表达式
							flag_is_int = 2;
							scanner(line, flag_is_int, errl);
						}
					}
					else if (line.substr(0, 6) == "double")
					{
						cout << "Error:line " << errl << " 不支持double类型的数据" << endl;
					}
					else if (line.find('=') != -1)
					{
						eval(line, errl);
					}
					else if (line.substr(0, 5) == "write")
					{
						string var = line.substr(line.find("(") + 1, line.find_last_of(')') - line.find('(') - 1);
						int is_uninitialized = std::count(Uninitialize.begin(), Uninitialize.end(), var);
						//cout << is_uninitialized << endl;
						//cout << var << endl;
						if (is_uninitialized) //先判断  注意顺序！
						{
							cout << "Error:line " << errl << " uninitialized identifier" << endl;
						}
						else if (Identities_int.count(var))
						{
							cout << Identities_int[var] << endl;
						}
						else if (Identities_float.count(var))
						{
							cout << Identities_float[var] << endl;
						}
						else
						{
							cout << "Error:line " << errl << " undefined identifier" << endl;
						}
					}
					else
					{
						cout << "Error:line " << errl << " syntax error" << endl;
					}

					cout << "line" << errl << " End of code." << endl;
					break;
				}
			}
			else if (line.substr(0, 3) == "int")
			{
				if (line.find('=') != -1)
				{
					//说明在赋值
					cout << "Error:line " << errl << " 暂时不支持声明变量的时候赋值" << endl;
				}
				else
				{
					flag_is_int = 1;
					scanner(line, flag_is_int, errl);
				}
			}
			else if (line.substr(0, 5) == "float") {
				if (line.find('=') != -1)
				{
					cout << "Error:line " << errl << " 暂时不支持声明变量的时候赋值" << endl;
				}
				else
				{
					//flag_is_int=2代表浮点数,然后开始解析表达式
					flag_is_int = 2;
					scanner(line, flag_is_int, errl);
				}
			}
			else if (line.substr(0, 6) == "double")
			{
				cout << "Error:line " << errl << " 不支持double类型的数据" << endl;
			}
			else if (line.find('=') != -1)
			{
				eval(line, errl);
			}
			else if (line.substr(0, 5) == "write")
			{
				string var = line.substr(line.find("(") + 1, line.find_last_of(')') - line.find('(') - 1);
				int is_uninitialized = std::count(Uninitialize.begin(), Uninitialize.end(), var);
				//cout << is_uninitialized << endl;
				//cout << var << endl;
				if (is_uninitialized) //先判断  注意顺序！
				{
					cout << "Error:line " << errl << " uninitialized identifier" << endl;
				}
				else if (Identities_int.count(var))
				{
					cout << Identities_int[var] << endl;
				}
				else if (Identities_float.count(var))
				{
					cout << Identities_float[var] << endl;
				}
				else
				{
					cout << "Error:line " << errl << " undefined identifier" << endl;
				}
			}
			else
			{
				cout << "Error:line " << errl << " syntax error" << endl;
			}
		}
	}
	in.close();
}
