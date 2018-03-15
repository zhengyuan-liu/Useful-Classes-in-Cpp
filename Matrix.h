/********************************************************************************
 * �ļ�����Matrix.h                                                             *
 * ������Matrix(������)                                                         *
 * ���ߣ���֤Դ                                                                 *
 * ѧ�ţ�1300012430                                                             *
 ********************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include<cmath>
#include"Fraction.h"
using namespace std;

template<typename T>
class Matrix
{
	template <typename>
	friend class Matrix;

private:
	int row;        //���������
	int column;     //���������
	T** pElement;   //����Ԫ��ָ��
	bool isSquare;  //�Ƿ��Ƿ���(�������������Ƿ����)
	static T determinant(const Matrix<T> & m, const int & order)  //�ݹ����n�����е�ֵ
	{
		if (order == 1)
			return m.pElement[0][0];
		T result = 0;
		int factor = 1;
		for (int k = 0; k < order; k++)
		{
			int iSub = 0, jSub = 0;
			Matrix<T> mSub(order - 1, order - 1);
			for (int i = 1; i < order; i++)
			{
				jSub = 0;
				for (int j = 0; j < order; j++)
				{
					if (j == k)
						continue;
					mSub.pElement[iSub][jSub] = m.pElement[i][j];
					jSub++;
				}
				iSub++;
			}
			result += m.pElement[0][k] * determinant(mSub, order - 1) * factor;
			factor *= -1;
		}
		return result;
	}

public:
	Matrix()  //Ĭ�Ϲ��캯��������һ��0��0�еľ���
	{
		this->row = 0;
		this->column = 0;
		isSquare = true;
		this->pElement = nullptr;
	}
	Matrix(int n)  //����һ��n�׵ĵ�λ����
	{
		this->row = n;
		this->column = n;
		isSquare = true;

		pElement = new T*[row];
		for (int i = 0; i < this->row; i++)
		{
			this->pElement[i] = new T[column];
			for (int j = 0; j < this->column; j++)
			{
				if (i == j)
					this->pElement[i][j] = 1;
				else
					this->pElement[i][j] = 0;
			}
		}
	}
	Matrix(int row, int column)  //����һ��row��colunm�е������
	{
		this->row = row;
		this->column = column;
		if (row == column)
			isSquare = true;
		else
			isSquare = false;
		pElement = new T*[row];
		for (int i = 0; i < this->row; i++)
		{
			this->pElement[i] = new T[column];
			for (int j = 0; j < this->column; j++)
				this->pElement[i][j] = 0;
		}
	}
	Matrix(const Matrix & copy)  //�������캯��
	{
		this->row = copy.row;
		this->column = copy.column;
		this->isSquare = copy.isSquare;
		this->pElement = new T*[this->row];
		for (int i = 0; i < this->row; i++)
			this->pElement[i] = new T[this->column];
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				this->pElement[i][j] = copy.pElement[i][j];
	}
	~Matrix()  //��������
	{
		for (int i = 0; i < row; i++)
			delete this->pElement[i];
		delete this->pElement;
		this->pElement = nullptr;
	}

	int* size()  //��ȡ����Ĵ�С������һ������Ϊ2�����飬����[0]Ϊ���������������[1]Ϊ���������
	{
		int* pSize = new int[2];
		pSize[0] = row;
		pSize[1] = column;
		return pSize;
	}
	T max()  //��������Ԫ��
	{
		T max = this->pElement[0][0];
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				if (this->pElement[i][j] > max)
					max = this->pElement[i][j];
		return max;
	}
	int* max_index()  //��������Ԫ�����꣬����һ������Ϊ2�����飬����[0]Ϊ���Ԫ�ص������꣬����[1]Ϊ���Ԫ�ص�������
	{
		T max = this->pElement[0][0];
		int rowIndex = 0, colIndex = 0;
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				if (this->pElement[i][j] > max)
				{
					max = this->pElement[i][j];
					rowIndex = i;
					colIndex = j;
				}
		int* maxIndexes = new int[2];
		maxIndexes[0] = rowIndex;
		maxIndexes[1] = colIndex;
		return maxIndexes;
	}
	T* diag()  //���ضԽ�Ԫ�أ�����һ������Ϊrow��һά���飬����[i]ΪMii��ֵ
	{
		if (!this->isSquare)
			throw runtime_error("Matrix must be a square!");
		T* diag = new T[row];
		for (int i = 0; i < row; i++)
			diag[i] = this->pElement[i][i];
		return diag;
	}
	T det()  //���������ʽ
	{
		if (!this->isSquare)
			throw runtime_error("Matrix must be a square!");
		return determinant(*this, row);
	}
	double norm()  //�����ķ�����2������
	{
		double norm = 0;
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				norm += this->pElement[i][j] * this->pElement[i][j];
		return sqrt(norm);
	}
	void swaprows(int i1, int i2)  //����i1�к�i2�У�������0��ʼ��
	{
		if (i1 >= this->row || i2 >= this->row)
			throw runtime_error("i1��i2����С�ھ�������!");
		if (i1 == i2)
			return;

		T temp;
		for (int j = 0; j < this->column; j++)
		{
			temp = this->pElement[i1][j];
			this->pElement[i1][j] = this->pElement[i2][j];
			this->pElement[i2][j] = temp;
		}
	}
	void swapcols(int j1, int j2)  //����j1�к�j2�У�������0��ʼ��
	{
		if (j1 >= this->column || j2 >= this->column)
			throw runtime_error("j1��j2����С�ھ�������!");
		if (j1 == j2)
			return;

		T temp;
		for (int i = 0; i < this->row; i++)
		{
			temp = this->pElement[i][j1];
			this->pElement[i][j1] = this->pElement[i][j2];
			this->pElement[i][j2] = temp;
		}
	}
	void transpose()  //�����ת��
	{
		Matrix mTran(this->column, this->row);
		for (int i = 0; i < this->row; i++)
		{
			for (int j = 0; j < this->column; j++)
				mTran.pElement[j][i] = this->pElement[i][j];
			delete this->pElement[i];
		}
		delete this->pElement;

		this->row = mTran.row;
		this->column = mTran.column;
		this->pElement = new T*[this->row];
		for (int i = 0; i < this->row; i++)
		{
			this->pElement[i] = new T[column];
			for (int j = 0; j < this->column; j++)
				this->pElement[i][j] = mTran.pElement[i][j];
		}
	}
	void resize(int sizeRow, int sizeCol)  //������״ת��
	{
		for (int i = 0; i < row; i++)
			delete this->pElement[i];
		delete this->pElement;

		this->row = sizeRow;
		this->column = sizeCol;
		if (this->row == this->column)
			isSquare = true;
		else
			isSquare = false;
		this->pElement = new T*[this->row];
		for (int i = 0; i < this->row; i++)
		{
			this->pElement[i] = new T[column];
			for (int j = 0; j < this->column; j++)
				this->pElement[i][j] = 0;
		}
	}
	Matrix<Fraction> inv()  //����������󣬷���һ����������������󲻿����򷵻�һ��0��0�е�Ĭ�Ͼ��󲢸�����Ϣ
	{
		if (!isSquare)
			throw runtime_error("Matrix must be a square!");

		Matrix<Fraction> m(this->row);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				m.pElement[i][j] = this->pElement[i][j];
		Matrix<Fraction> mInv(this->row);
		for (int k = 0; k < mInv.row - 1; k++)  //����������Ԫ
		{
			bool isInv = false;
			for (int i = k; i < mInv.row; i++)
			{
				if (m.pElement[i][k] != 0)
				{
					m.swaprows(i, k);
					mInv.swaprows(i, k);
					isInv = true;
					break;
				}
			}
			if (isInv == false)  //ȫΪ0
			{
				Matrix<Fraction> zero;
				cout << "�þ��󲻿���" << endl;
				return zero;
			}
			Fraction temp = m.pElement[k][k];
			for (int j = 0; j < mInv.row; j++)  //���ڵ�k�е�ÿ�е�Ԫ��
			{			
				m.pElement[k][j] /= temp;
				mInv.pElement[k][j] /= temp;
			}
			for (int i = k + 1; i < mInv.row; i++)  //���ڵ�k+1���Ժ��ÿ��
			{
				Fraction temp = m.pElement[i][k];
				for (int j = 0; j < mInv.row; j++)  //���ڵ�i�е�ÿ�е�Ԫ��
				{
					m.pElement[i][j] -= m.pElement[k][j] * temp;
					mInv.pElement[i][j] -= mInv.pElement[k][j] * temp;
				}
			}
		}
		for (int k = mInv.row - 1; k > 0; k--)  //�������ϻش�
		{
			if (m.pElement[k][k] == 0)  //��ԪΪ0
			{
				Matrix<Fraction> zero;
				cout << "�þ��󲻿���" << endl;
				return zero;
			}
			Fraction temp = m.pElement[k][k];
			for (int j = 0; j < mInv.row; j++)  //���ڵ�k�е�ÿ�е�Ԫ��
			{
				m.pElement[k][j] /= temp;
				mInv.pElement[k][j] /= temp;
			}
			for (int i = k - 1; i >= 0; i--)  //���ڵ�k-1�����ϵ�ÿ��
			{
				Fraction temp = m.pElement[i][k];
				for (int j = 0; j < mInv.row; j++)  //���ڵ�i�е�ÿ�е�Ԫ��
				{
					m.pElement[i][j] -= m.pElement[k][j] * temp;
					mInv.pElement[i][j] -= mInv.pElement[k][j] * temp;
				}
			}
		}	
		return mInv;
	}
	Matrix power(const int & k)  //������ݴ�
	{
		if (!this->isSquare)
			throw runtime_error("Matrix must be a square!");

		Matrix mResult;
		for (int i = 0; i < k; i++)
			mResult = *this * *this;
		return mResult;
	}

	T * & operator[](const int & index)  //�����±������
	{
		return this->pElement[index];
	}
	Matrix& operator=(const Matrix & that)  //���ظ�ֵ�����
	{
		for (int i = 0; i < row; i++)
			delete this->pElement[i];
		delete this->pElement;

		this->row = that.row;
		this->column = that.column;
		this->isSquare = that.isSquare;
		this->pElement = new T*[this->row];
		for (int i = 0; i < this->row; i++)
		{
			this->pElement[i] = new T[column];
			for (int j = 0; j < this->column; j++)
				this->pElement[i][j] = that.pElement[i][j];
		}
		return *this;
	}
	Matrix operator+(const Matrix & that)  //���ؼӷ������
	{
		if (this->row != that.row || this->column != that.column)
			throw runtime_error("���ӷ������������С����һ��");
		Matrix mResult(this->row, this->column);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				mResult.pElement[i][j] = this->pElement[i][j] + that.pElement[i][j];
		return mResult;
	}
	Matrix operator-(const Matrix & that)  //���ؼ��������
	{
		if (this->row != that.row || this->column != that.column)
			throw runtime_error("�����������������С����һ��");
		Matrix mResult(this->row, this->column);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				mResult.pElement[i][j] = this->pElement[i][j] - that.pElement[i][j];
		return mResult;
	}
	Matrix operator*(const T & k)  //���س˷�������������˷�
	{
		Matrix mResult(this->row, this->column);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				mResult.pElement[i][j] = this->pElement[i][j] * k;
		return mResult;
	}
	Matrix operator*(const Matrix & that)  //���س˷������������˷�
	{
		if (this->column != that.row)
			throw runtime_error("������˷�ʱ��ǰ���������������ں���������");

		int n = this->column;
		Matrix mResult(this->row, that.column);
		for (int j = 0; j < mResult.column; j++)  //�Ժ�����ÿһ��
		{
			for (int i = 0; i < mResult.row; i++)  //��ǰ�����ÿһ��
			{
				T sum = 0;
				for (int k = 0; k < n; k++)
					sum += this->pElement[i][k] * that.pElement[k][j];
				mResult.pElement[i][j] = sum;
			}
		}
		return mResult;
	}
	bool operator==(const Matrix & that)  //���ص��������
	{
		if (this->row != that.row || this->column != that.column)
			return false;
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				if (this->pElement[i][j] != that.pElement[i][j])
					return false;
		return true;
	}
	bool operator!=(const Matrix & that)  //���ز����������
	{
		if (this->row != that.row || this->column != that.column)
			return true;
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				if (this->pElement[i][j] != that.pElement[i][j])
					return true;
		return false;
	}

	friend ostream& operator<<(ostream &os, const Matrix<T> &m)  //����<<�������ʵ�־������
	{
		for (int i = 0; i < m.row; i++)
		{
			for (int j = 0; j < m.column; j++)
				os << m.pElement[i][j] << '\t';
			os << endl;
		}
		return os;
	}
};

#endif
