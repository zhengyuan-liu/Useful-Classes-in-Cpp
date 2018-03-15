/********************************************************************************
 * 文件名：Matrix.h                                                             *
 * 类名：Matrix(矩阵类)                                                         *
 * 作者：刘证源                                                                 *
 * 学号：1300012430                                                             *
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
	int row;        //矩阵的行数
	int column;     //矩阵的列数
	T** pElement;   //矩阵元素指针
	bool isSquare;  //是否是方阵(即行数与列数是否相等)
	static T determinant(const Matrix<T> & m, const int & order)  //递归计算n阶行列的值
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
	Matrix()  //默认构造函数，构造一个0行0列的矩阵
	{
		this->row = 0;
		this->column = 0;
		isSquare = true;
		this->pElement = nullptr;
	}
	Matrix(int n)  //构造一个n阶的单位矩阵
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
	Matrix(int row, int column)  //构造一个row行colunm列的零矩阵
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
	Matrix(const Matrix & copy)  //拷贝构造函数
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
	~Matrix()  //析构函数
	{
		for (int i = 0; i < row; i++)
			delete this->pElement[i];
		delete this->pElement;
		this->pElement = nullptr;
	}

	int* size()  //获取矩阵的大小，返回一个长度为2的数组，数组[0]为矩阵的行数，数组[1]为矩阵的列数
	{
		int* pSize = new int[2];
		pSize[0] = row;
		pSize[1] = column;
		return pSize;
	}
	T max()  //求矩阵最大元素
	{
		T max = this->pElement[0][0];
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				if (this->pElement[i][j] > max)
					max = this->pElement[i][j];
		return max;
	}
	int* max_index()  //求矩阵最大元素坐标，返回一个长度为2的数组，数组[0]为最大元素的行坐标，数组[1]为最大元素的列坐标
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
	T* diag()  //返回对角元素，返回一个长度为row的一维数组，数组[i]为Mii的值
	{
		if (!this->isSquare)
			throw runtime_error("Matrix must be a square!");
		T* diag = new T[row];
		for (int i = 0; i < row; i++)
			diag[i] = this->pElement[i][i];
		return diag;
	}
	T det()  //求矩阵行列式
	{
		if (!this->isSquare)
			throw runtime_error("Matrix must be a square!");
		return determinant(*this, row);
	}
	double norm()  //求矩阵的范数（2范数）
	{
		double norm = 0;
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				norm += this->pElement[i][j] * this->pElement[i][j];
		return sqrt(norm);
	}
	void swaprows(int i1, int i2)  //交换i1行和i2行，行数从0开始计
	{
		if (i1 >= this->row || i2 >= this->row)
			throw runtime_error("i1和i2必须小于矩阵行数!");
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
	void swapcols(int j1, int j2)  //交换j1列和j2列，列数从0开始计
	{
		if (j1 >= this->column || j2 >= this->column)
			throw runtime_error("j1和j2必须小于矩阵列数!");
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
	void transpose()  //矩阵的转置
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
	void resize(int sizeRow, int sizeCol)  //矩阵形状转换
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
	Matrix<Fraction> inv()  //求矩阵的逆矩阵，返回一个分数矩阵，如果矩阵不可逆则返回一个0行0列的默认矩阵并给出信息
	{
		if (!isSquare)
			throw runtime_error("Matrix must be a square!");

		Matrix<Fraction> m(this->row);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				m.pElement[i][j] = this->pElement[i][j];
		Matrix<Fraction> mInv(this->row);
		for (int k = 0; k < mInv.row - 1; k++)  //从上往下消元
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
			if (isInv == false)  //全为0
			{
				Matrix<Fraction> zero;
				cout << "该矩阵不可逆" << endl;
				return zero;
			}
			Fraction temp = m.pElement[k][k];
			for (int j = 0; j < mInv.row; j++)  //对于第k行的每列的元素
			{			
				m.pElement[k][j] /= temp;
				mInv.pElement[k][j] /= temp;
			}
			for (int i = k + 1; i < mInv.row; i++)  //对于第k+1行以后的每行
			{
				Fraction temp = m.pElement[i][k];
				for (int j = 0; j < mInv.row; j++)  //对于第i行的每列的元素
				{
					m.pElement[i][j] -= m.pElement[k][j] * temp;
					mInv.pElement[i][j] -= mInv.pElement[k][j] * temp;
				}
			}
		}
		for (int k = mInv.row - 1; k > 0; k--)  //从下往上回代
		{
			if (m.pElement[k][k] == 0)  //主元为0
			{
				Matrix<Fraction> zero;
				cout << "该矩阵不可逆" << endl;
				return zero;
			}
			Fraction temp = m.pElement[k][k];
			for (int j = 0; j < mInv.row; j++)  //对于第k行的每列的元素
			{
				m.pElement[k][j] /= temp;
				mInv.pElement[k][j] /= temp;
			}
			for (int i = k - 1; i >= 0; i--)  //对于第k-1行以上的每行
			{
				Fraction temp = m.pElement[i][k];
				for (int j = 0; j < mInv.row; j++)  //对于第i行的每列的元素
				{
					m.pElement[i][j] -= m.pElement[k][j] * temp;
					mInv.pElement[i][j] -= mInv.pElement[k][j] * temp;
				}
			}
		}	
		return mInv;
	}
	Matrix power(const int & k)  //矩阵的幂次
	{
		if (!this->isSquare)
			throw runtime_error("Matrix must be a square!");

		Matrix mResult;
		for (int i = 0; i < k; i++)
			mResult = *this * *this;
		return mResult;
	}

	T * & operator[](const int & index)  //重载下标运算符
	{
		return this->pElement[index];
	}
	Matrix& operator=(const Matrix & that)  //重载赋值运算符
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
	Matrix operator+(const Matrix & that)  //重载加法运算符
	{
		if (this->row != that.row || this->column != that.column)
			throw runtime_error("做加法的两个矩阵大小必须一致");
		Matrix mResult(this->row, this->column);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				mResult.pElement[i][j] = this->pElement[i][j] + that.pElement[i][j];
		return mResult;
	}
	Matrix operator-(const Matrix & that)  //重载减法运算符
	{
		if (this->row != that.row || this->column != that.column)
			throw runtime_error("做减法的两个矩阵大小必须一致");
		Matrix mResult(this->row, this->column);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				mResult.pElement[i][j] = this->pElement[i][j] - that.pElement[i][j];
		return mResult;
	}
	Matrix operator*(const T & k)  //重载乘法运算符，数量乘法
	{
		Matrix mResult(this->row, this->column);
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				mResult.pElement[i][j] = this->pElement[i][j] * k;
		return mResult;
	}
	Matrix operator*(const Matrix & that)  //重载乘法运算符，矩阵乘法
	{
		if (this->column != that.row)
			throw runtime_error("做矩阵乘法时，前矩阵的列数必须等于后矩阵的行数");

		int n = this->column;
		Matrix mResult(this->row, that.column);
		for (int j = 0; j < mResult.column; j++)  //对后矩阵的每一列
		{
			for (int i = 0; i < mResult.row; i++)  //对前矩阵的每一行
			{
				T sum = 0;
				for (int k = 0; k < n; k++)
					sum += this->pElement[i][k] * that.pElement[k][j];
				mResult.pElement[i][j] = sum;
			}
		}
		return mResult;
	}
	bool operator==(const Matrix & that)  //重载等于运算符
	{
		if (this->row != that.row || this->column != that.column)
			return false;
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				if (this->pElement[i][j] != that.pElement[i][j])
					return false;
		return true;
	}
	bool operator!=(const Matrix & that)  //重载不等于运算符
	{
		if (this->row != that.row || this->column != that.column)
			return true;
		for (int i = 0; i < this->row; i++)
			for (int j = 0; j < this->column; j++)
				if (this->pElement[i][j] != that.pElement[i][j])
					return true;
		return false;
	}

	friend ostream& operator<<(ostream &os, const Matrix<T> &m)  //重载<<运算符，实现矩阵输出
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
