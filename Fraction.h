/********************************************************************************
 * 文件名：Fraction.h                                                           *
 * 类名：Fraction(分数类，整数可作为特殊的分数)                                 *
 * 作者：刘证源                                                                 *
 * 学号：1300012430                                                             *
 ********************************************************************************/

#ifndef FRACTION_H
#define FRACTION_H

#include<cmath>
using namespace std;

class Fraction
{
private:
	int numerator;    //分子
	int denominator;  //分母
	static int get_gcd(int a, int b)  //获取两个数的最大公约数
	{
		int max = a, min = b;
		if (b > a)
		{
			max = b;
			min = a;
		}
		int temp = max - min;
		while (temp)
		{		
			if (temp >= min)
				max = temp;
			else
			{
				max = min;
				min = temp;
			}
			temp = max - min;
		}
		return min;
	}
public:
	Fraction()  //默认构造函数，构造0/1
	{
		numerator = 0;
		denominator = 1;
	}
	Fraction(int up, int down)  //根据分子和分母构造函数
	{
		if (down==0)  //分母为零
			throw runtime_error("分母不能为0!");
		numerator = up;
		denominator = down;
		simplify();
	}
	Fraction(int n)  //根据整数构造分数，构造n/1
	{
		numerator = n;
		denominator = 1;
	}
	Fraction(const Fraction & copy)  //拷贝构造函数
	{
		numerator = copy.numerator;
		denominator = copy.denominator;
	}

	void simplify()  //分数化简
	{
		if (numerator == 0 || numerator == 1 || denominator == 1)
			return;
		int gcd = get_gcd(abs(denominator), abs(numerator));
		numerator /= gcd;
		denominator /= gcd;
		if (denominator < 0)
		{
			numerator *= -1;
			denominator *= -1;
		}
	}

	operator double()   //转换为double类型
	{
		return double(numerator) / double(denominator);
	}
	Fraction& operator=(const Fraction & that)   //重载赋值运算符
	{
		this->numerator = that.numerator;
		this->denominator = that.denominator;
		return *this;
	}
	Fraction operator+(const Fraction & that)   //重载分数加法
	{
		Fraction f;
		f.denominator = this->denominator * that.denominator;
		f.numerator = this->numerator * that.denominator + that.numerator * this->denominator;
		f.simplify();
		return f;
	}
	Fraction operator-(const Fraction & that)   //重载分数减法
	{
		Fraction f;
		f.denominator = this->denominator * that.denominator;
		f.numerator = this->numerator*that.denominator - that.numerator*this->denominator;
		f.simplify();
		return f;
	}
	Fraction operator*(const int & factor)   //重载分数和整数的乘法
	{
		Fraction f;
		f.numerator = this->numerator * factor;
		f.denominator = this->denominator;
		f.simplify();
		return f;
	}
	Fraction operator*(const Fraction & that)   //重载分数和分数的乘法
	{
		Fraction f;
		f.denominator = this->denominator * that.denominator;
		f.numerator = this->numerator * that.numerator;
		f.simplify();
		return f;
	}
	Fraction operator/(const Fraction & that)   //重载分数除法
	{
		Fraction f;
		f.denominator = this->denominator * that.numerator;
		f.numerator = this->numerator * that.denominator;
		f.simplify();
		return f;
	}
	Fraction& operator+=(const Fraction & that)   //重载+=运算符
	{
		*this = *this + that;
		return *this;
	}
	Fraction& operator-=(const Fraction & that)   //重载-=运算符
	{
		*this = *this - that;
		return *this;
	}
	Fraction& operator*=(const Fraction & that)   //重载*=运算符
	{
		*this = *this * that;
		return *this;
	}
	Fraction& operator/=(const Fraction & that)   //重载/=运算符
	{
		*this = *this / that;
		return *this;
	}
	
	friend ostream& operator<<(ostream& os, const Fraction& f)   //重载输出运算符
	{
		if (f.numerator == 0)
			os << "0";
		else if (f.denominator == 1)
			os << f.numerator;
		else
			os << f.numerator << "/" << f.denominator;
		return os;
	}

};

#endif