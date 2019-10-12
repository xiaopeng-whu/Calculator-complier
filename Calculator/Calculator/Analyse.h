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
const double ACCURACY = 0.01; //�Ƚ�����double�������Ĵ�Сʱ�����ֵʹ�õľ���

using namespace std;
bool isoperator(char op);                                       // �ж��Ƿ�Ϊ�����
int priority(char op);                                          // ����������ȼ�
double postfix(string pre, map<string, int> mymap1, map<string, float> mymap2, int* i);     // ����׺���ʽת��Ϊ��׺���ʽ
double read_number(char str[], int* i, map<string, int> mymap1, map<string, float> mymap2); // �������ַ���ת�����Ӧ�����֣���ʶ��ת��Ϊ����
double postfix_value(char post[], map<string, int> mymap1, map<string, float> mymap2, int errl);      //�Ժ�׺���ʽ��ֵ

map<string, int> Identities_int;//��ű�ʶ���������Ͷ���
map<string, float> Identities_float;
map<string, int>::iterator iter;

vector<string> Uninitialize;  //�洢δ��ʼ���ı���
//�ж��Ƿ�Ϊ������
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

//������������ȼ�
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

//����׺���ʽת��Ϊ��׺���ʽ�����غ�׺���ʽ�ĳ��ȣ������ո�
double postfix(string pre, map<string, int> mymap1, map<string, float> mymap2, int* i, int errl)
{
	char post[MAX];      //��׺���ʽ����
	int j = 0;
	MyStack<char> stack;
	stack.init();        // ��ʼ���洢��������ջ
	stack.push('#');     // ���Ȱѽ�����־��;������ջ��

	while (pre[*i] != ';')
	{
		if ((pre[*i] >= '0' && pre[*i] <= '9') || pre[*i] == '.' || (pre[*i] >= 'a' && pre[*i] <= 'z') || (pre[*i] >= 'A' && pre[*i] <= 'Z')) // �������֡���ĸ��С����ֱ��д���׺���ʽ
		{
			post[j++] = pre[*i];
		}
		else if (pre[*i] == '(')        // �������������ñȽ�ֱ����ջ
			stack.push(pre[*i]);
		else if (pre[*i] == ')')        // ���������Ž����Ӧ�����ź�Ĳ�������������ջ�еģ�ȫ��д���׺���ʽ
		{
			while (stack.getTop() != '(')
			{
				post[j++] = stack.pop();
			}
			stack.pop();                // ����������ջ����׺���ʽ�в���С����
		}
		else if (isoperator(pre[*i]))
		{
			post[j++] = ' ';            // �ÿո�ֿ�������

			while (priority(pre[*i]) <= priority(stack.getTop()))
			{
				// ��ǰ�Ĳ�����С�ڵ���ջ�������������ȼ�ʱ����ջ��������д�뵽��׺���ʽ���ظ��˹���
				post[j++] = stack.pop();
			}
			stack.push(pre[*i]);        // ��ǰ���������ȼ�����ջ�������������ȼ������ò�������ջ
		}

		(*i)++;
	}
	while (stack.top) // �����еĲ����������׺���ʽ
	{
		post[j++] = stack.pop();
	}
	double result = postfix_value(post, mymap1, mymap2, errl);
	return result;
}
// �������ַ���ת�����Ӧ�����֣���ʶ��ת��Ϊ����
double read_number(char str[], int* i, map<string, int> mymap1, map<string, float> mymap2, int errl)
{
	double x = 0.0; //һ���з���ֵ���޷�������ֵ����δ��ʼ��������δ������������
	int k = 0;
	bool f = false;
	while (str[*i] >= '0' && str[*i] <= '9')  // ������������
	{
		x = x * 10 + (str[*i] - '0');
		(*i)++;
	}

	if (str[*i] == '.') // ����С������
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
	while ((str[*i] >= 'a' && str[*i] <= 'z') || (str[*i] >= 'A' && str[*i] <= 'Z'))//�����ʶ��
	{
		int len = *i;
		string token = "";
		do {
			token = token + str[*i];
			(*i)++;
		} while ((str[*i] >= 'a' && str[*i] <= 'z') || (str[*i] >= 'A' && str[*i] <= 'Z') || (str[*i] >= '0' && str[*i] <= '9'));
		int is_uninitialized = std::count(Uninitialize.begin(), Uninitialize.end(), token); //�Ƿ���δ��ʼ��vector��
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
				cout << "Error:line " << errl << " undefined identifier" << endl;  //��ֵ
				return UDF;
			}
		}

	}
	//cout << x << endl;
	return x;
}
//�Ժ�׺���ʽ��ֵ
double postfix_value(char post[], map<string, int> mymap1, map<string, float> mymap2, int errl)
{
	MyStack<double> stack;    // ������ջ
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
			//cout << (read_number(post, &i, mymap1, mymap2, errl) == UDF) << endl;  //doubleֵ�����ȣ���������ȡ��ֵ��
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
				stack.push(value);  //read_number��ʹ��value���ɣ�����᷵��0
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

//��������
void scanner(string s, int flag, int errl)
{
	//cout << "scanner_in" << endl;
	//cout << flag << endl;
	//int i = 1; flag�������������͵�
	if (flag == 1)
	{
		//���ҵ��ֺŵ�λ�ã�Ȼ����ȡ����ע��substr�����е㲻һ��
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
			cout << "Error:line " << errl << " ����" << var << "�ظ����壡" << endl;
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
			cout << "Error:line " << errl << " ����" << var << "�ظ����壡" << endl;
		}
		else
		{
			Identities_float.insert(pair<string, float>(var, 0.0));
			Uninitialize.push_back(var);
		}
	}
	//cout << "scanner_out" << endl;
}
//������ֵ
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
		int value = value1; //int��Ѵ������ͱ��0
		if (fabs(value1 - UIN) < ACCURACY || fabs(value1 - UDF) < ACCURACY) //�ж�������ֵ�Ƿ���Ч
			return;
		else {
			//Identities_int.insert(pair<string, int>(var, value));
			Identities_int[var] = value;
			//cout << "eval_value_in" << endl;
			for (vector<string>::iterator it = Uninitialize.begin(); it != Uninitialize.end(); )
			{
				if (*it == var)
				{
					it = Uninitialize.erase(it); //����д��arr.erase(it);
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
					it = Uninitialize.erase(it); //����д��arr.erase(it);
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
		cout << "Error:line " << errl << " undefined identifier" << endl;   //��ֵ
	}
	//cout << "eval_out" << endl;
}
//��ӡ�������ļ���ÿһ��
void analyse(string fPath) {
	int flag_is_int;
	string path = fPath;
	ifstream in2(path, ios::in);
	string line = "";
	int errl = 0, errPsn = 0;//�������ڵ��к�λ��
	//�Ȱ�Դ�����ӡһ��
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
	//���ж�ȡ�ļ�
	while (!in.eof()) {
		getline(in, line);
		errl++;
		if (line != "") {
			//cout << line << endl;
			//��ע��ȥ��
			if (line.find("#")) //ֻ��һ��'/'���ң�why�����������г��ŵ��л������ʱ��'#'���档
			{
				line = line.substr(0, line.find_first_of("#")); //���븳ֵ������substr������ı�line
				//cout << line << endl;
			}
			//�жϽ�β�Ƿ���";"
			if (line[line.length() - 1] != ';') {
				//errPsn = line.length();
				//cout << "Error(line " << errl << ",position " << errPsn << "): Ӧ����';'" << endl;
				cout << "Error:line " << errl << " ĩβȱ��';'" << endl;
				//system("pause");
				if (line[line.length() - 1] == '.') {
					//��������������copyһ�ݣ���ʱû�뵽���õ��߼���ϵ���..��Ŀ��Ӧ�����ж�ĩβΪ'.'�����ж�ĩβΪ';'��
					if (line.substr(0, 3) == "int")
					{
						if (line.find('=') != -1)
						{
							//˵���ڸ�ֵ
							cout << "Error:line " << errl << " ��ʱ��֧������������ʱ��ֵ" << endl;
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
							cout << "Error:line " << errl << " ��ʱ��֧������������ʱ��ֵ" << endl;
						}
						else
						{
							//flag_is_int=2��������,Ȼ��ʼ�������ʽ
							flag_is_int = 2;
							scanner(line, flag_is_int, errl);
						}
					}
					else if (line.substr(0, 6) == "double")
					{
						cout << "Error:line " << errl << " ��֧��double���͵�����" << endl;
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
						if (is_uninitialized) //���ж�  ע��˳��
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
					//˵���ڸ�ֵ
					cout << "Error:line " << errl << " ��ʱ��֧������������ʱ��ֵ" << endl;
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
					cout << "Error:line " << errl << " ��ʱ��֧������������ʱ��ֵ" << endl;
				}
				else
				{
					//flag_is_int=2��������,Ȼ��ʼ�������ʽ
					flag_is_int = 2;
					scanner(line, flag_is_int, errl);
				}
			}
			else if (line.substr(0, 6) == "double")
			{
				cout << "Error:line " << errl << " ��֧��double���͵�����" << endl;
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
				if (is_uninitialized) //���ж�  ע��˳��
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
