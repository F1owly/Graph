#pragma once
#include <initializer_list>
#include <iomanip>
#include <math.h>
#include <stdexcept>
#include <typeinfo>
#define EPS 0.00000001


namespace linalg {
	template <typename T>
	class Matrix {
private:
		T* m_ptr;
		int m_rows;
		int m_columns;
		int m_capacity;
public:
		Matrix() {
			m_rows = 0;
			m_columns = 0;
			m_capacity = 0;
			m_ptr = nullptr;
		}

		~Matrix() {
			if (m_ptr != nullptr) {
				delete[] m_ptr;
			}
		}

		Matrix(int rows) {
			if (rows <= 0) {
				throw std::runtime_error("Rows must be more zero!");
			}
			m_capacity = static_cast<int>(std::log2(m_rows)) + 1;
			m_ptr = new T[m_capacity];
			m_rows = rows;
			m_columns = 1;
			

		}

		Matrix(int rows, int cols) {
			if (rows <= 0) {
				throw std::runtime_error("Rows must be more zero!");
			}
			if (cols <= 0) {
				throw std::runtime_error("Cols must be more zero!");
			}
			m_capacity = rows * cols;
			m_ptr = new T[m_capacity];
			m_rows = rows;
			m_columns = cols;
		}

		template<typename T1>
		Matrix(const Matrix<T1>& m1) : Matrix(m1.rows(), m1.columns()) {
			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m_columns; j++) {
					(*this)(i, j) = static_cast<T>(m1(i, j));
				}
			}
		}

		Matrix(Matrix&& moved) noexcept {
			m_ptr = moved.m_ptr;
			m_rows = moved.m_rows;
			m_columns = moved.m_columns;
		}
		
		template<typename T1>
		Matrix(const std::initializer_list<T1>& list) : Matrix(list.size()) {
			int i = 0;
			for (auto el : list) {
				m_ptr[i++] = static_cast<T>(el);
			}
		}
		
		template<typename T1>
		Matrix(const std::initializer_list<const std::initializer_list<T1>>& list) {
			m_rows = list.size();
			m_columns = 1;
			if (list.begin() != list.end()) {
				m_columns = (*list.begin()).size();
				m_capacity = m_rows * m_columns;
				m_ptr = new T[m_capacity];
			}
			else {
				m_capacity = 0;
				delete[] m_ptr;
				throw std::runtime_error("Can`t create matrix! Empty initializer_list!");
			}

			int i = 0;
			int cols = 0;
			for (auto el_list : list) {
				for (T1 el : el_list) {
					if (cols >= m_columns)
						throw std::runtime_error("Can`t create matrix! Invalid initializer_list!");
					m_ptr[i++] = static_cast<T>(el);
					cols++;
				}
				cols = 0;
			}
		}

		int rows() const {
			return m_rows;
		}

		int columns() const {
			return m_columns;
		}

		bool empty() const {
			if (m_rows == 0 || m_columns == 0) {
				return true;
			}
			return false;
		}

		void reshape(int rows, int cols) {
			if (rows * cols == m_rows * m_columns) {
				m_rows = rows;
				m_columns = cols;
			}
			else {
				throw "Can`t reshape matrix!";
			}
		}

		Matrix& operator=(const Matrix& m1) {
			if (m_rows * m_columns > 0) {
				delete[] m_ptr;
			}

			m_rows = m1.m_rows;
			m_columns = m1.m_columns;
			m_ptr = new T[static_cast<long long>(m_rows) * m_columns];

			for (int i = 0; i < m_rows * m_columns; i++) {
				m_ptr[i] = m1.m_ptr[i];
			}

			return *this;
		}

		Matrix& operator=(Matrix&& m1) noexcept {
			if (m_rows * m_columns > 0) {
				delete[] m_ptr;
			}

			m_rows = m1.m_rows;
			m_columns = m1.m_columns;
			m_ptr = m1.m_ptr;

			return *this;
		}

		T& operator()(int i, int j) {
			if (0 <= i && i < m_rows && 0 <= j && j < m_columns) {
				return m_ptr[i * m_columns + j];
			}
			else {
				delete[]m_ptr;
				throw std::runtime_error("double& operator()(int i, int j); //Index out of range.");
			}
		}

		const T& operator()(int i, int j) const {
			if (0 <= i && i < m_rows && 0 <= j && j < m_columns) {
				return m_ptr[i * m_columns + j];
			}
			else {
				delete[]m_ptr;
				throw std::runtime_error("const double& operator()(int i, int j) const; //Index out of range.");
			}
		}

		friend std::ostream& operator << (std::ostream& os, const Matrix& m)
		{
			for (int i = 0; i < m.m_columns * m.m_rows; i++) {
				if (i % m.m_columns == 0) {
					os << "|";
				}
				
				os << std::setprecision(3) << m.m_ptr[i];
				
				if ((i + 1) % m.m_columns == 0) {
					os << "|";
					if (i + 1 < m.m_columns * m.m_rows) {
						os << '\n';
					}
				}
				else os << "\t";

			}
			return os;
		}

		template<typename T1>
		Matrix& operator+=(const Matrix<T1>& m) {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				throw std::runtime_error("The matrices must be of the same size!");
			}

			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] += m.m_ptr[i];
			}
			return *this;
		}

		template<typename T1>
		Matrix& operator-=(const Matrix<T1>& m) {
			if (m_rows != m.m_rows || m_columns != m.m_columns) {
				throw std::runtime_error("The matrices must be of the same size!");
			}

			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] -= m.m_ptr[i];
			}
			return *this;
		}

		template<typename T1>
		Matrix& operator*=(const Matrix<T1>& m) {
			*this = Matrix(*this * m);
			return *this;
		}

		template<typename X>
		Matrix& operator*=(X x) {
			for (int i = 0; i < m_columns * m_rows; i++) {
				m_ptr[i] *= x;
			}
			return *this;
		}

		double trace() const {
			if (m_rows != m_columns) {
				throw std::runtime_error("Matrix is not square!");
			}

			T trace = 0;
			for (int i = 0; i < m_rows; i++) {
				trace += (*this)(i, i);
			}
			return trace;
		}

		double det() const {
			if (m_rows != m_columns) {
				throw std::runtime_error("Matrix is not square!");
			}

			Matrix ans(m_rows, m_columns);
			for (int i = 0; i < m_rows * m_columns; i++) {
				ans.m_ptr[i] = m_ptr[i];
			}
			T det = 1;
			//(i, i_) - ��� ���������� �������������� �������� ��������
			for (int i = 0, i_ = 0; i < m_rows && i_ < m_columns; i++, i_++) {
				int j = i; // j - ������, � ����������� ��������� � ������� i_
				for (int k = i + 1; k < m_rows; k++) {
					if (abs(ans(k, i_)) > abs(ans(j, i_))) {
						j = k;
					}
				}

				//���� ��� �������� � ������� i_ �������, ��������� �� ��������� �������
				if (abs(ans(j, i_)) < EPS) {
					i--;
					continue;
				}

				if (j != i) {
					det *= -1;
				}
				//������ ������ � ������������ ��������� �� ����� ������ i
				for (int k = 0; k < m_columns; k++) {
					T swapper = ans(i, k);
					ans(i, k) = ans(j, k);
					ans(j, k) = swapper;
				}

				//�������� �� ���� �����, ��� ���� ������ i, ��� ������, ���������� �� ��������� ���, ����� ��� �������� i_��� ������� c���� ����� 0
				for (int h = i + 1; h < m_rows; h++) {
					double coef = 1.0 * (ans(h, i_)) / ans(i, i_);
					for (int k = 0; k < m_columns; k++) {
						ans(h, k) -= ans(i, k) * coef;
					}
				}
			}


			for (int i = 0; i < m_rows; i++) {
				det *= ans(i, i);
			}

			return det;
		}

		Matrix& gauss_forward() {
			Matrix ans(std::move(*this));
			//(i, i_) - ��� ���������� �������������� �������� ��������
			for (int i = 0, i_ = 0; i < m_rows && i_ < m_columns; i++, i_++) {
				int j = i; // j - ������, � ����������� ��������� � ������� i_
				for (int k = i + 1; k < m_rows; k++) {
					if (abs(ans(k, i_)) > abs(ans(j, i_))) {
						j = k;
					}
				}
				//���� ��� �������� � ������� i_ �������, ��������� �� ��������� �������
				if (abs(ans(j, i_)) < EPS) {
					i--;
					continue;
				}
				//������ ������ � ������������ ��������� �� ����� ������ i
				for (int k = 0; k < m_columns; k++) {
					T swapper = ans(i, k);
					ans(i, k) = ans(j, k);
					ans(j, k) = swapper;
				}
				//�������� �� ���� �����, ��� ���� ������ i, ��� ������, ���������� �� ��������� ���, ����� ��� �������� i_��� ������� c���� ����� 0
				for (int h = i + 1; h < m_rows; h++) {
					double coef = 1.0 * (ans(h, i_)) / ans(i, i_);
					for (int k = 0; k < m_columns; k++) {
						ans(h, k) -= ans(i, k) * coef;
					}
				}
			}
			return *this;
		}

		Matrix& gauss_back() {
			Matrix ans(std::move(*this));
			{
				//�������� �� �� ��� ������� �����������
				int j_based = -1;
				bool zeros_row = true;

				for (int i = 0; i < m_rows; i++) {
					for (int j = 0; j < m_columns; j++) {
						if (abs(ans(i, j)) > EPS) {
							if (j_based > j) {
								throw std::runtime_error("Matrix is not step view!");
							}
							j_based = j;
							zeros_row = false;
							break;
						}
					}

					if (zeros_row) {
						continue;
					}

					for (int k = i + 1; k < m_rows; k++) {
						if (abs(ans(k, j_based)) > EPS) {
							throw std::runtime_error("Matrix is not step view!");
						}
					}

					zeros_row = true;
				}
			}
			int n = -1;
			for (int i = m_rows - 1; i >= 0; i--) {
				for (int j = 0; j < m_columns; j++) {
					if (abs(ans(i, j)) > EPS) {
						n = i;
						i = -1;
						break;
					}
				}
			}

			if (n == -1) {
				return *this;
			}

			for (int i = n; i >= 0; i--) {
				int j_based = -1;
				for (int j = 0; j < m_columns; j++) {
					if (abs(ans(i, j)) > EPS) {
						j_based = j;
						break;
					}
				}
				if (j_based == -1) {
					continue;
				}

				for (int k = 0; k < i; k++) {
					double coef = 1.0 * (ans(k, j_based)) / ans(i, j_based);
					for (int h = 0; h < m_columns; h++) {
						ans(k, h) -= coef * ans(i, h);
					}
				}
			}
			return *this;
		}

		int rank() const {
			Matrix ans = *this;

			ans.gauss_forward();
			ans.gauss_back();

			int rank = 0;
			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m_columns; j++) {
					if (abs(ans(i, j)) > EPS) {
						rank++;
						break;
					}
				}
			}

			return rank;
		}

		double norm() const {
			T sum2 = 0;
			for (int i = 0; i < static_cast<long long>(m_rows * m_columns); i++) {
				sum2 += m_ptr[i] * m_ptr[i];
			}
			return std::sqrt(sum2);
		}
	};

	template<typename T1, typename T2>
	bool operator==(const Matrix<T1>& m1, const Matrix<T2>& m2) {
		if (m1.rows() != m2.rows() || m1.columns() != m2.columns()) {
			return false;
		}

		for (int i = 0; i < m1.rows(); i++) {
			for (int j = 0; j < m1.columns(); j++) {
				if (abs(m1(i, j) - m2(i, j)) > EPS) {
					return false;
				}
			}
		}
		return true;
	}

	template<typename T1, typename T2>
	bool operator!=(const Matrix<T1>& m1, const Matrix<T2>& m2) {
		return !(m1==m2);
	}

	template<typename T1, typename T2>
	auto operator*(const Matrix<T1>& m1, const Matrix<T2>& m2) ->Matrix<decltype(m1(0, 0)+m2(0, 0))> {
		if (m1.columns() != m2.rows()) {
			throw std::runtime_error("The matrices are not compatible");
		}

		Matrix<decltype(m1(0, 0)+m2(0, 0))> ans(m1.rows(), m2.columns());
		for (int i = 0; i < m1.rows(); i++) {
			for (int j = 0; j < m2.columns(); j++) {
				ans(i, j) = 0;
				for (int k = 0; k < m1.columns(); k++) {
					ans(i, j) += m2(k, j) * m1(i, k);
				}
			}
		}
		return ans;
	}

	template<typename T1, typename T2>
	auto operator+(const Matrix<T1>& m1, const Matrix<T2>& m2) -> Matrix<decltype(m1(0, 0) + m2(0, 0))> {
		if (m1.rows() != m2.rows() || m1.columns() != m2.columns()) {
			throw std::runtime_error("The matrices must be of the same size!");
		}

		Matrix<decltype(m1(0, 0) + m2(0, 0))> ans(m1.rows(), m2.columns());
		for (int i = 0; i < m1.columns() * m1.rows(); i++) {
			ans(i / m1.columns(), i % m1.columns()) = m1(i / m1.columns(), i % m1.columns()) + m2(i / m1.columns(), i % m1.columns());
		}
		return ans;
	}

	template<typename T1, typename T2>
	auto operator-(const Matrix<T1>& m1, const Matrix<T2>& m2) -> Matrix<decltype(m1(0, 0) + m2(0, 0))> {
		if (m1.rows() != m2.rows() || m1.columns() != m2.columns()) {
			throw std::runtime_error("The matrices must be of the same size!");
		}

		Matrix<decltype(m1(0, 0) + m2(0, 0))> ans(m1.rows(), m2.columns());
		for (int i = 0; i < m1.columns() * m1.rows(); i++) {
			ans(i / m1.columns(), i % m1.columns()) = m1(i / m1.columns(), i % m1.columns()) - m2(i / m1.columns(), i % m1.columns());
		}
		return ans;
	}


	template<typename X, typename T1>
	auto operator*(X x, const linalg::Matrix<T1>& m) -> linalg::Matrix<decltype(x* m(0, 0))> {
		linalg::Matrix<decltype(x* m(0, 0))> ans(m.rows(), m.columns());
		for (int i = 0; i < m.columns() * m.rows(); i++) {
			ans(i / m.columns(), i % m.columns()) = m(i / m.columns(), i % m.columns()) * x;
		}
		return ans;
	}

	template<typename X, typename T1>
	auto operator*(const linalg::Matrix<T1>& m, X x) -> linalg::Matrix<decltype(x* m(0, 0))> {
		linalg::Matrix<decltype(x* m(0, 0))> ans(m.rows(), m.columns());
		for (int i = 0; i < m.columns() * m.rows(); i++) {
			ans(i / m.columns(), i % m.columns()) = m(i / m.columns(), i % m.columns()) * x;
		}
		return ans;
	}

	template<typename X, typename T1>
	auto operator+(X x, const linalg::Matrix<T1>& m) -> linalg::Matrix<decltype(x + m(0, 0))> {
		linalg::Matrix<decltype(x + m(0, 0))> ans(m.rows(), m.columns());
		for (int i = 0; i < m.columns() * m.rows(); i++) {
			ans(i / m.columns(), i % m.columns()) = m(i / m.columns(), i % m.columns()) + x;
		}
		return ans;
	}

	template<typename X, typename T1>
	auto operator+(const linalg::Matrix<T1>& m, X x) -> linalg::Matrix<decltype(x + m(0, 0))> {
		linalg::Matrix<decltype(x + m(0, 0))> ans(m.rows(), m.columns());
		for (int i = 0; i < m.columns() * m.rows(); i++) {
			ans(i / m.columns(), i % m.columns()) = m(i / m.columns(), i % m.columns()) + x;
		}
		return ans;
	}

	template<typename X, typename T1>
	auto operator-(X x, const linalg::Matrix<T1>& m) -> linalg::Matrix<decltype(x + m(0, 0))> {
		linalg::Matrix<decltype(x + m(0, 0))> ans(m.rows(), m.columns());
		for (int i = 0; i < m.columns() * m.rows(); i++) {
			ans(i / m.columns(), i % m.columns()) = x - m(i / m.columns(), i % m.columns());
		}
		return ans;
	}

	template<typename X, typename T1>
	auto operator-(const linalg::Matrix<T1>& m, X x) -> linalg::Matrix<decltype(x + m(0, 0))> {
		linalg::Matrix<decltype(m(0, 0) - x)> ans(m.rows(), m.columns());
		for (int i = 0; i < m.columns() * m.rows(); i++) {
			ans(i / m.columns(), i % m.columns()) = m(i / m.columns(), i % m.columns()) - x;
		}
		return ans;
	}
}

template<typename T>
linalg::Matrix<T> concatenate(const linalg::Matrix<T>& m1, const linalg::Matrix<T>& m2) {//c�������� �� �����������
	if (m1.rows() != m2.rows()) {
		throw std::runtime_error("The matrices have a different number of rows!");
	}

	linalg::Matrix<T> ans(m1.rows(), m1.columns() + m2.columns());

	for (int i = 0; i < m1.rows(); i++) {
		for (int j = 0; j < m1.columns(); j++) {
			ans(i, j) = m1(i, j);
		}
		for (int j = 0; j < m2.columns(); j++) {
			ans(i, m1.columns() + j) = m2(i, j);
		}
	}
	return ans;
}

template<typename T>
linalg::Matrix<T> transpose(const linalg::Matrix<T>& m) {
	linalg::Matrix<T> ans(m.columns(), m.rows());

	for (int i = 0; i < ans.rows(); i++) {
		for (int j = 0; j < ans.columns(); j++) {
			ans(i, j) = m(j, i);
		}
	}

	return ans;
}

template<typename T>
T minor(const linalg::Matrix<T>& m, int i_, int j_) {
	if (m.rows() != m.columns()) {
		throw std::runtime_error("Matrix is not square!");
	}
	if (m.rows() < 1 || m.columns() < 1) {
		throw std::runtime_error("Matrix is 1x1 or less!");
	}
	if (m.rows() == 1 || m.columns() == 1) {
		return 1;
	}

	i_--;
	j_--;

	linalg::Matrix<T> minor(m.rows() - 1, m.columns() - 1);
	int counted = 0;
	for (int i = 0; i < m.rows(); i++) {
		if (i == i_) {
			continue;
		}
		for (int j = 0; j < m.columns(); j++) {
			if (j == j_) {
				continue;
			}
			minor(static_cast<int>(counted / minor.columns()), static_cast<int>(counted % minor.columns())) = m(i, j);
			counted++;
		}
	}
	return minor.det();
}

template<typename T>
T cofactor(const linalg::Matrix<T>& m, int i_, int j_) {
	return std::pow(-1, i_ + j_) * minor(m, i_, j_);
}

template<typename T>
linalg::Matrix<T> invert(const linalg::Matrix<T>& m) {
	if (m.rows() != m.columns()) {
		throw std::runtime_error("Matrix is not square!");
	}

	if (abs(m.det()) < EPS) {
		throw std::runtime_error("Determinante is zero!");
	}

	linalg::Matrix<T> cofactors(m.rows(), m.columns());

	for (int i = 0; i < m.rows(); i++) {
		for (int j = 0; j < m.columns(); j++) {
			cofactors(i, j) = cofactor(m, i + 1, j + 1);
		}
	}

	cofactors = transpose(cofactors) * (1 / m.det());
	return cofactors;
}
