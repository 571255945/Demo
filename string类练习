#if 0

#define   _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<istream>
#include<ostream>
#include<stdio.h>
using namespace std;

class MyString
{
public:
	MyString(const char *str=NULL);
	MyString(const MyString &str);
	~MyString();

	MyString operator +(const MyString &str) const;
	MyString& operator=(const MyString &str);
	MyString/*&*/ operator+=(const MyString &str);
	bool operator ==(const MyString &str) const;
	char& operator[](size_t n) const;

	size_t size() const;
	const char* c_str() const;
	friend istream & operator>>(istream &is, MyString &str);
	friend ostream& operator<<(ostream &os, MyString &str);

private:
	char *data;
	size_t length;

};
MyString::MyString(const char *str/* = NULL*/)
{
	if (NULL==str)
	{
		length = 0;
		data = new char[1];
		*data = '\0';
	}
	else
	{
		length = strlen(str);
		data = new char[length + 1];
		strcpy(data, str);
	}
}
MyString::MyString(const MyString &str)
{
	this->length = str.size();
	this->data = new char[length + 1];
	strcpy(this->data, str.c_str());
}
MyString::~MyString()
{
	if (NULL!=this->data)
	{
		delete[] this->data;
		this->length = 0;
	}
}

MyString MyString::operator +(const MyString &str) const
{
	//MyString a = "jjj";
	//a = "hhh";
	//this->length = 1000;
	MyString newString;
	newString.length = this->length + str.size();
	newString.data = new char[newString.length + 1];
	strcpy(newString.data, this->data);
	strcat(newString.data, str.data);
	return newString;
	
}
MyString& MyString::operator=(const MyString &str)
{
	if (this==&str)
	{
		return *this;
	}
	//this->length = 1000;
	//异常安全的方法
	char *pOrig = this->data;
	this->data = new char[length + 1];
	strcpy(this->data, str.c_str());
	delete[] pOrig;
	return *this;
}
MyString/*&*/ MyString::operator+=(const MyString &str)
{
	this->length += str.length;
	char* newData = new char[length + 1];
	strcpy(newData, this->data);
	strcat(newData, str.data);
	delete[] this->data;
	this->data = newData;
	return *this;
}
bool MyString::operator ==(const MyString &str) const
{
	
	if (this->length != str.length)
	{
		return false;
	}
	return strcmp(this->data, str.data) ? false : true;
}
char& MyString::operator[](size_t n) const
{
	if (n>=length)
	{
		return data[length - 1];
	}
	else
	{
		return data[n];
	}
}

inline size_t MyString::size() const
{
	return length;
}
const char* MyString::c_str() const
{
	return this->data;
}
istream & operator>>(istream &is, MyString &str)
{
	char temp[1000];
	is >> temp;
	str.length = strlen(temp);
	str.data = new char[str.length + 1];
	strcpy(str.data, temp);
	return is;
}
ostream& operator<<(ostream &os, MyString &str)
{
	os << str.data;
	return os;
}
int main()
{
	MyString str1(" hello ");//构造函数
	MyString str2(" nihao ");//构造函数
	MyString str3 = str1 + str2;//加法运算符+拷贝构造
	MyString str7 = str3;//拷贝构造
	MyString str8 = "ququququ";//构造函数
	str8 = str1;  //赋值运算符
	MyString str4("hahahhahahahah += ");
	str2+=str4+= str3;
	cout << "str1: " << str1 << "; " << "str2: " << str2 << " ; " << "str3: " << str3 << " str4: " << str4 << endl;

	cout << "测试输入"<<endl;
	MyString str5, str6;
	cin >> str5>>str6;
	cout << "str5 = " << str5 << "  str6 = " << str6 << endl;

	system("pause");

	return 0;
}


#endif
