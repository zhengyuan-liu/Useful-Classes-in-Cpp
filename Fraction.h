/********************************************************************************
 * �ļ�����Fraction.h                                                           *
 * ������Fraction(�����࣬��������Ϊ����ķ���)                                 *
 * ���ߣ���֤Դ                                                                 *
 * ѧ�ţ�1300012430                                                             *
 ********************************************************************************/

#ifndef FRACTION_H
#define FRACTION_H

#include<cmath>
using namespace std;

class Fraction
{
private:
	int numerator;    //����
	int denominator;  //��ĸ
	static int get_gcd(int a, int b)  //��ȡ�����������Լ��
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
	Fraction()  //Ĭ�Ϲ��캯��������0/1
	{
		numerator = 0;
		denominator = 1;
	}
	Fraction(int up, int down)  //���ݷ��Ӻͷ�ĸ���캯��
	{
		if (down==0)  //��ĸΪ��
			throw runtime_error("��ĸ����Ϊ0!");
		numerator = up;
		denominator = down;
		simplify();
	}
	Fraction(int n)  //���������������������n/1
	{
		numerator = n;
		denominator = 1;
	}
	Fraction(const Fraction & copy)  //�������캯��
	{
		numerator = copy.numerator;
		denominator = copy.denominator;
	}

	void simplify()  //��������
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

	operator double()   //ת��Ϊdouble����
	{
		return double(numerator) / double(denominator);
	}
	Fraction& operator=(const Fraction & that)   //���ظ�ֵ�����
	{
		this->numerator = that.numerator;
		this->denominator = that.denominator;
		return *this;
	}
	Fraction operator+(const Fraction & that)   //���ط����ӷ�
	{
		Fraction f;
		f.denominator = this->denominator * that.denominator;
		f.numerator = this->numerator * that.denominator + that.numerator * this->denominator;
		f.simplify();
		return f;
	}
	Fraction operator-(const Fraction & that)   //���ط�������
	{
		Fraction f;
		f.denominator = this->denominator * that.denominator;
		f.numerator = this->numerator*that.denominator - that.numerator*this->denominator;
		f.simplify();
		return f;
	}
	Fraction operator*(const int & factor)   //���ط����������ĳ˷�
	{
		Fraction f;
		f.numerator = this->numerator * factor;
		f.denominator = this->denominator;
		f.simplify();
		return f;
	}
	Fraction operator*(const Fraction & that)   //���ط����ͷ����ĳ˷�
	{
		Fraction f;
		f.denominator = this->denominator * that.denominator;
		f.numerator = this->numerator * that.numerator;
		f.simplify();
		return f;
	}
	Fraction operator/(const Fraction & that)   //���ط�������
	{
		Fraction f;
		f.denominator = this->denominator * that.numerator;
		f.numerator = this->numerator * that.denominator;
		f.simplify();
		return f;
	}
	Fraction& operator+=(const Fraction & that)   //����+=�����
	{
		*this = *this + that;
		return *this;
	}
	Fraction& operator-=(const Fraction & that)   //����-=�����
	{
		*this = *this - that;
		return *this;
	}
	Fraction& operator*=(const Fraction & that)   //����*=�����
	{
		*this = *this * that;
		return *this;
	}
	Fraction& operator/=(const Fraction & that)   //����/=�����
	{
		*this = *this / that;
		return *this;
	}
	
	friend ostream& operator<<(ostream& os, const Fraction& f)   //������������
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