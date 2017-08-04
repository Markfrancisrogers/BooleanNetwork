/*
 * Matrix.hpp
 *
 *  Created on: Aug 18, 2009
 *      Author: stewie
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <cassert>
#include <cstddef>
#include <istream>
#include <ostream>
#include <utility>
#include <algorithm>
#include <memory>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/distance.hpp>

#include "printing.hpp"

namespace bn {

namespace util {

/**
 * A simple matrix class to hold some data.
 *
 * Matrix indices are zero-based (just like arrays). Data is laid out in rows,
 * so it is faster to scan a row at a time.
 *
 * Matrix iterators are all RandomAccess (actually are simple pointers).
 */
template<class T> class Matrix {
private:
	/**
	 * Number of rows in the matrix.
	 */
	const size_t m;
	/**
	 * Number of columns in the matrix.
	 */
	const size_t n;
	/**
	 * Data buffer of this matrix.
	 */
	T* data;

	/**
	 * Check index bounds on rows.
	 *
	 * If @a i is not within bounds, an assertion is thrown.
	 * @param i a row index
	 *
	 * @note
	 * This check is disabled in the release.
	 */
	void checkRowBounds(const size_t i) const {
		assert(i < numRows());
	}

	/**
	 * Check index bounds on columns.
	 *
	 * If @a i is not within bounds, an assertion is thrown.
	 * @param i a column index
	 *
	 * @note
	 * This check is disabled in the release.
	 */
	void checkColBounds(const size_t j) const {
		assert(j < numCols());
	}

public:
	/**
	 * RandomAccess Iterator type.
	 */
	typedef T* iterator;
	/**
	 * RandomAccess constant iterator type.
	 */
	typedef const T* const_iterator;
	typedef std::pair<iterator, iterator> row_view;
	typedef std::pair<const_iterator, const_iterator> const_row_view;

	/**
	 * Constructs a matrix filled with copies of provided value.
	 * @param m number of rows
	 * @param n number of columns
	 * @param value a value to initialize the matrix with
	 */
	Matrix(const size_t m, const size_t n, const T value = 0) :
		m(m), n(n), data(std::get_temporary_buffer<T>(m * n).first) {
		std::uninitialized_fill(begin(), end(), value);
	}

	/**
	 * Constructs a matrix given dimensions and a range of elements.
	 *
	 * The range shall contain \f$m \cdot n\f$ elements which are supposed to
	 * be laid out row-wise.
	 * @param m number of rows
	 * @param n number of columns
	 * @param first input iterator to the initial position of the first range
	 * @param last input iterator to the final position of the first range
	 */
	template<class InputIterator> Matrix(const size_t m, const size_t n,
			const InputIterator first, const InputIterator last) :
		m(m), n(n), data(std::get_temporary_buffer<T>(m * n).first) {
		assert(std::distance(first, last) == static_cast<int> (n * m));
		std::uninitialized_copy(first, last, data);
	}

	/**
	 * Constructs a matrix given dimensions and a range of elements.
	 *
	 * This overload works with Boost ranges instead of iterators. Has the same
	 * requirements as Matrix::Matrix(const size_t,const size_t,const InputIterator,const InputIterator).
	 * @param m number of rows
	 * @param n number of columns
	 * @param r a single pass range of size \f$m \cdot n\f$
	 *
	 * @note
	 * If compiled in debug configuration (i.e. assertions enabled) the type
	 * of @a r must conform to <a href="http://www.boost.org/libs/range">
	 * ForwardRange</a> concept.
	 */
	template<class SinglePassRange> Matrix(const size_t m, const size_t n,
			const SinglePassRange& r) :
		m(m), n(n), data(std::get_temporary_buffer<T>(m * n).first) {
		assert(boost::distance(r) == n * m);
		std::uninitialized_copy(boost::begin(r), boost::end(r), data);
	}

	/**
	 * Copy constructor.
	 * @param other other matrix to copy
	 */
	Matrix(const Matrix& other) :
		m(other.m), n(other.n), data(new T[m * n]) {
		memcpy(data, other.data, m * n);
	}

	/**
	 * Destructor.
	 */
	~Matrix() {
		std::return_temporary_buffer(data);
	}

	/**
	 * Returns the number of rows.
	 * @return the number of rows
	 */
	size_t numRows() const {
		return m;
	}

	/**
	 * Returns the number of columns.
	 * @return the number of columns
	 */
	size_t numCols() const {
		return n;
	}

	/**
	 * Implementation of swap function.
	 * @param other a matrix
	 */
	void swap(Matrix& other) {
		std::swap(data, other.data);
		std::swap(m, other.m);
		std::swap(n, other.n);
	}

	/**
	 * Iterator to the beginning of the data buffer.
	 *
	 * The iterator scans one row at a time.
	 * @return an iterator to the initial position in the data buffer
	 */
	iterator begin() {
		return data;
	}

	/**
	 * Iterator to the end of the data buffer.
	 * @return an iterator to the final position in the data buffer
	 */
	iterator end() {
		return data + n * m;
	}

	/**
	 * Constant iterator to the beginning of the data buffer.
	 *
	 * The iterator scans one row at a time.
	 * @return a constant iterator to the initial position in the data buffer
	 */
	const_iterator begin() const {
		return data;
	}

	/**
	 * Constant iterator to the end of the data buffer.
	 * @return a constant iterator to the final position in the data buffer
	 */
	const_iterator end() const {
		return data + n * m;
	}

	/**
	 * Iterator to the beginning of the <em>i</em>-th row.
	 * @param i row index
	 * @return an iterator to the initial position in the row
	 */
	iterator begin(const size_t i) {
		checkRowBounds(i);
		return data + n * i;
	}

	/**
	 * Iterator to the end of the <em>i</em>-th row.
	 * @param i row index
	 * @return an iterator to the final position in the row
	 */
	iterator end(const size_t i) {
		checkRowBounds(i);
		return data + n * (i + 1);
	}

	/**
	 * Constant iterator to the beginning of the <em>i</em>-th row.
	 * @param i row index
	 * @return a constant iterator to the initial position in the row
	 */
	const_iterator begin(const size_t i) const {
		checkRowBounds(i);
		return data + n * i;
	}

	/**
	 * Constant iterator to the end of the <em>i</em>-th row.
	 * @param i row index
	 * @return a constant iterator to the final position in the row
	 */
	const_iterator end(const size_t i) const {
		checkRowBounds(i);
		return data + n * (i + 1);
	}

	/**
	 * Same as begin(const size_t).
	 * @param i row index
	 * @return an iterator to the initial position in the row
	 */
	iterator operator[](const size_t i) {
		return begin(i);
	}

	row_view row(const size_t i) {
		return std::make_pair(begin(i), end(i));
	}

	const_row_view row(const size_t i) const {
		return std::make_pair(begin(i), end(i));
	}

	/**
	 * Same as begin(const size_t)const.
	 * @param i row index
	 * @return a constant iterator to the initial position in the row
	 */
	const_iterator operator[](const size_t i) const {
		return begin(i);
	}

	/**
	 * Access an element in the matrix.
	 * @param i row index
	 * @param j column index
	 * @return the corresponding element
	 */
	T& operator()(const size_t i, const size_t j) {
		checkColBounds(j);
		return begin(i)[j];
	}

	/**
	 * Read-only access to an element in the matrix.
	 * @param i row index
	 * @param j column index
	 * @return the corresponding element
	 */
	const T& operator()(const size_t i, const size_t j) const {
		checkColBounds(j);
		return begin(i)[j];
	}

	template<class U> friend std::ostream& operator<<(std::ostream& out,
			const Matrix<U>& array);
};

/**
 * Prints a matrix to a stream.
 *
 * It prints each row separated by newlines an each element separated by spaces.
 * @param out an output stream
 * @param m a matrix
 * @return the stream passed as first argument
 */
template<class T> std::ostream& operator<<(std::ostream& out,
		const Matrix<T>& m) {
	for (size_t i = 0; i < m.numRows(); ++i) {
		print(out, m.row(i), " ");
		out << '\n';
	}
	return out;
}

}

}

namespace std {

/**
 * Specialization that calls bn::util::Matrix::swap(bn::util::Matrix&)
 * on the first argument.
 * @param a a matrix object
 * @param b another matrix object
 */
template<class T> void swap(bn::util::Matrix<T>& a, bn::util::Matrix<T>& b) {
	a.swap(b);
}

} // namespace std

#endif /* MATRIX_HPP_ */
