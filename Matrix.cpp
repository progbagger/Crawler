#include <iostream>
#include <fstream>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

class Matrix
{
	int** M; int size;
	void rawClean()
	{
		for (int i = 0; i < this->size; i++)
			delete[] this->M[i];
		delete[] this->M;
	}

	void rawCopy(const Matrix& that)
	{
		this->size = that.size;
		this->M = new int* [this->size];
		for (int i = 0; i < this->size; i++)
			this->M[i] = new int[this->size];
		for (int i = 0; i < this->size; i++)
			for (int j = 0; j < this->size; j++)
				this->M[i][j] = that.M[i][j];
	}

	class Vector // вспомогательный класс для реализации доп. заданий 1 и 2 (взятие строки и столбца)
	{
		void rawClean()
		{
			delete[] this->V;
		}

		void rawCopy(const Vector& that)
		{
			this->size = that.size;
			this->V = new int* [this->size];
			for (int i = 0; i < this->size; i++)
				this->V[i] = that.V[i];
		}

	public:
		int** V, size;
		Vector()
		{
			this->size = 0;
			V = nullptr;
		}

		Vector(int s)
		{
			this->size = s;
			V = new int* [this->size];
			for (int i = 0; i < this->size; i++)
				V[i] = 0;
		}

		Vector(const Vector& that)
		{
			this->rawCopy(that);
		}

		~Vector()
		{
			this->rawClean();
		}

		Vector& operator = (const Vector& that)
		{
			if (this != &that)
			{
				this->rawClean();
				this->rawCopy(that);
			}
			return *this;
		}

		int& operator[](int r) // корректный доступ к элементам строки/столбца
		{
			return *this->V[r - 1];
		}
	};

public:
	Matrix() // конструктор по умолчанию
	{
		size = 0;
		M = nullptr;
	}

	Matrix(const Matrix& that) // конструктор копирования
	{
		this->rawCopy(that);
	}

	~Matrix()
	{
		this->rawClean();
	}

	Matrix(int s) // конструктор, создающий единичную матрицу размера s
	{
		this->size = s;
		M = new int* [this->size];
		for (int i = 0; i < this->size; i++)
			M[i] = new int[size];
		for (int i = 0; i < this->size; i++)
			for (int j = 0; j < this->size; j++)
				if (i == j)
					M[i][j] = 1;
				else
					M[i][j] = 0;
	}

	Matrix(int s, int* digits) // конструктор, создающий матрицу размера s и заполняющий её диагональ элементами из массива digits, а остальные элементы нулями
	{
		this->size = s;
		M = new int* [size];
		for (int i = 0; i < this->size; i++)
			M[i] = new int[size];
		for (int i = 0; i < this->size; i++)
			for (int j = 0; j < this->size; j++)
			{
				if (i == j)
					M[i][j] = digits[i];
				else
					M[i][j] = 0;
			}
	}

	void Print() // функция вывода матрицы
	{
		for (int i = 0; i < this->size; i++)
		{
			for (int j = 0; j < this->size; j++)
				fout << this->M[i][j] << '\t';
			fout << endl;
		}
	}

	void Enter() // функцтя ввода матрицы в программу
	{
		for (int i = 0; i < this->size; i++)
			for (int j = 0; j < this->size; j++)
				fin >> this->M[i][j];
	}

	Matrix& operator =(const Matrix& that)
	{
		if (this != &that)
		{
			this->rawClean();
			this->rawCopy(that);
		}
		return *this;
	}

	Matrix operator +(Matrix a)
	{
		if (this->size != a.size)
		{
			fout << "Error: different sizes";
			exit(0);
		}
		else
		{
			Matrix result(this->size);
			for (int i = 0; i < result.size; i++)
				for (int j = 0; j < result.size; j++)
					result.M[i][j] = this->M[i][j] + a.M[i][j];
			return result;
		}
	}

	Matrix operator -(Matrix a)
	{
		if (this->size != a.size)
		{
			fout << "Error: different sizes";
			exit(0);
		}
		else
		{
			Matrix result(this->size);
			for (int i = 0; i < result.size; i++)
				for (int j = 0; j < result.size; j++)
					result.M[i][j] = this->M[i][j] - a.M[i][j];
			return result;
		}
	}

	Matrix operator *(Matrix a)
	{
		if (this->size != a.size)
		{
			fout << "Error: different sizes";
			exit(0);
		}
		else
		{
			Matrix result(this->size);
			for (int i = 0; i < result.size; i++) // зануление результирующей матрицы
				result.M[i][i] = 0;
			for (int i = 0; i < result.size; i++)
				for (int j = 0; j < result.size; j++)
					for (int k = 0; k < result.size; k++)
						result.M[i][j] += this->M[i][k] * a.M[k][j];
			return result;
		}
	}

	bool operator ==(Matrix a)
	{
		bool result = 1;
		if (this->size != a.size)
			result = 0;
		else
			for (int i = 0; i < this->size; i++)
				for (int j = 0; j < this->size; j++)
					if (this->M[i][j] != a.M[i][j])
					{
						result = 0;
						goto end;
					}
	end:
		return result;
	}

	bool operator !=(Matrix a)
	{
		bool result = 0;
		if (this->size != a.size)
			result = 1;
		else
			for (int i = 0; i < this->size; i++)
				for (int j = 0; j < this->size; j++)
					if (this->M[i][j] == a.M[i][j])
					{
						result = 1;
						break;
					}
		return result;
	}

	Matrix operator !() // оператор транспонирования матрицы
	{
		Matrix result(this->size);
		for (int i = 0; i < this->size; i++)
			for (int j = 0; j < this->size; j++)
				result.M[i][j] = this->M[j][i];
		return result;
	}

	Matrix operator ()(int a, int b) // взятие минора - матрица без строки a и столбца b
	{
		Matrix result(this->size - 1);
		for (int i = 0; i < a - 1; i++)
			for (int j = 0; j < b - 1; j++)
				result.M[i][j] = this->M[i][j];
		for (int i = a - 1; i < result.size; i++)
			for (int j = 0; j < b - 1; j++)
				result.M[i][j] = this->M[i + 1][j];
		for (int i = 0; i < a - 1; i++)
			for (int j = b - 1; j < result.size; j++)
				result.M[i][j] = this->M[i][j + 1];
		for (int i = a - 1; i < result.size; i++)
			for (int j = b - 1; j < result.size; j++)
				result.M[i][j] = this->M[i + 1][j + 1];
		return result;
	}

	Vector operator [](int r) // оператор взятия строки
	{
		Vector result(this->size);
		for (int i = 0; i < this->size; i++)
			result.V[i] = &this->M[r - 1][i];
		return result;
	}

	Vector operator ()(int r) // оператор взятия столбца
	{
		Vector result(this->size);
		for (int i = 0; i < result.size; i++)
			result.V[i] = &this->M[i][r - 1];
		return result;
	}
};

void execute(int n, int k)
{
	int* m = new int[n];
	for (int i = 0; i < n; i++)
		m[i] = k;
	Matrix A(n);
	Matrix B(n);
	Matrix C(n);
	Matrix D(n);
	Matrix K(n, m);
	delete[] m;
	A.Enter();
	B.Enter();
	C.Enter();
	D.Enter();
	((A + B * !C + K) * !D).Print();
}

int main()
{
	int n, k;
	fin >> n >> k;
	execute(n, k);
	fin.close();
	fout.close();
	return 0;
}
