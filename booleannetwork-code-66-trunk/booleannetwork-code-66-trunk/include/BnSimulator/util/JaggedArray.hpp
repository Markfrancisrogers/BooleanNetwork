/*
 * JaggedArray.hpp
 *
 *  Created on: Jul 14, 2009
 *      Author: stewie
 */

#ifndef JAGGEDARRAY_HPP_
#define JAGGEDARRAY_HPP_

#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <ostream>
#include <istream>
#include <sstream>
#include <vector>
#include <algorithm> // for std::copy

#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "printing.hpp"

namespace bn {

namespace util {

template<class T> class JaggedArray {
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef boost::iterator_range<iterator> row_view;
	typedef boost::iterator_range<const_iterator> const_row_view;
	typedef size_t size_type;

	JaggedArray() :
		data(NULL), delimiters(1, 0) {
	}

	JaggedArray(const JaggedArray& array) :
		data(new T[array.bufferSize()]), delimiters(array.delimiters) {
		assert(bufferSize() == array.bufferSize());
		memcpy(data, array.data, sizeof(T) * bufferSize());
	}

	~JaggedArray() {
		delete[] data;
	}

	size_type numRows() const {
		return delimiters.size() - 1;
	}

	size_type sizeOf(const size_type i) const {
		assert(checkBounds(i));
		return delimiters[i + 1] - delimiters[i];
	}

	iterator begin(const size_type i) {
		assert(checkBounds(i));
		return data + delimiters[i];
		//return iterators[i];
	}

	const_iterator begin(const size_t i) const {
		assert(checkBounds(i));
		return data + delimiters[i];
		//return iterators[i];
	}

	iterator end(const size_t i) {
		assert(checkBounds(i));
		return data + delimiters[i + 1];
		//return iterators[i+1];
	}

	const_iterator end(const size_t i) const {
		assert(checkBounds(i));
		return data + delimiters[i + 1];
		//return iterators[i+1];
	}

	row_view row(const size_type i) {
		return boost::make_iterator_range(begin(i), end(i));
	}

	const_row_view row(const size_type i) const {
		return boost::make_iterator_range(begin(i), end(i));
	}

	iterator operator[](const size_type i) {
		return begin(i);
	}

	const_iterator operator[](const size_type i) const {
		return begin(i);
	}

	template<class U> friend std::istream& operator>>(std::istream& is,
			JaggedArray<U>& array);

	template<class U> friend std::ostream
	& operator<<(std::ostream& out, const JaggedArray<U>& array);

	template<class U> friend void std::swap(JaggedArray<U>& a,
			JaggedArray<U>& b);

private:
	T* data;
	std::vector<size_type> delimiters;
	//std::vector<const_iterator> iterators;

#ifndef NDEBUG
	bool checkBounds(const size_type i) const {
		return i < numRows();
	}
#endif

	size_type bufferSize() const {
		return delimiters.back();
	}

	JaggedArray& operator=(const JaggedArray& array);
};

template<class T> std::ostream& operator<<(std::ostream& out,
		const JaggedArray<T>& array) {
	for (size_t i = 0; i < array.numRows(); ++i) {
		print(out, array.row(i), " ");
		out << '\n';
	}
	return out;
}

template<class T> std::istream& operator>>(std::istream& is,
		JaggedArray<T>& array) {
	std::string line;
	size_t index = 0;
	std::vector<T> dataBuffer;
	while (std::getline(is, line).good()) {
		boost::trim(line);
		if (!boost::starts_with(line, "#")) {
			if (!line.empty()) {
				std::istringstream istr(line);
				while (istr.good()) {
					++index;
					T elem;
					istr >> elem;
					dataBuffer.push_back(elem);
				}
			}
			array.delimiters.push_back(index);
		}
	}
	assert(dataBuffer.size() == array.bufferSize());
	array.data = new T[dataBuffer.size()];
	std::copy(dataBuffer.begin(), dataBuffer.end(), array.data);
	return is;
}

}

}

namespace std {

template<class T> void swap(bn::util::JaggedArray<T>& a, bn::util::JaggedArray<
		T>& b) {
	//	std::cout << "Inexpensive JaggedArray swap" << std::endl;
	swap(a.data, b.data);
	swap(a.delimiters, b.delimiters);
}

} // namespace std

#endif /* JAGGEDARRAY_HPP_ */
