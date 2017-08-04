/*
 * JaggedArray.hpp
 *
 *  Created on: Jul 14, 2009
 *      Author: stewie
 */

#ifndef JAGGEDARRAY2_HPP_
#define JAGGEDARRAY2_HPP_

#include <cassert>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>

#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>

template<class T> class JaggedArray2 {
public:
	typedef const T* const_iterator;

	JaggedArray2() :
		data(NULL), delimiters(1, 0) {
	}

	JaggedArray2(const JaggedArray2& array) :
		data(new T[array.bufferSize()]), delimiters(array.delimiters) {
		assert(bufferSize() == array.bufferSize());
		memcpy(data, array.data, sizeof(T) * bufferSize());
	}

	~JaggedArray2() {
		delete[] data;
	}

	uint numRows() const {
		return delimiters.size() - 1;
	}

	uint sizeOf(const uint i) const {
		checkBounds(i);
		return delimiters[i + 1] - delimiters[i];
	}

	void swap(JaggedArray2& other) {
		std::swap(data, other.data);
		std::swap(delimiters, other.delimiters);
	}

	const_iterator begin(const uint i) const {
		checkBounds(i);
		return data + delimiters[i];
	}

	const_iterator end(const uint i) const {
		checkBounds(i);
		return data + delimiters[i + 1];
	}

	const_iterator operator[](const uint i) const {
		return begin(i);
	}

	static std::ostream& toStream(std::ostream& out, const JaggedArray2& array) {
		for (unsigned int i = 0; i < array.numRows(); ++i) {
			std::copy(array.begin(i), array.end(i), std::ostream_iterator<T>(
					out, " "));
			out << '\n';
		}
		std::copy(array.delimiters.begin(), array.delimiters.end(),
				std::ostream_iterator<uint>(out, " "));
		return out;
	}

	static std::istream& fromStream(std::istream& is, JaggedArray2& array) {
		std::string line;
		uint index = 0;
		std::vector<T> dataBuffer;
		while (std::getline(is, line).good()) {
			boost::trim(line);
			if (!boost::starts_with(line, "#")) {
				std::istringstream istr(line);
				while (istr.good()) {
					++index;
					T elem;
					istr >> elem;
					dataBuffer.push_back(elem);
				}
				array.delimiters.push_back(index);
			}
		}
		assert(dataBuffer.size() == array.bufferSize());
		array.data = new T[dataBuffer.size()];
		std::copy(dataBuffer.begin(), dataBuffer.end(), array.data);
		return is;
	}

private:
	T* data;
	std::vector<uint> delimiters;

	void checkBounds(const uint i) const {
		assert(i >= 0 && i < numRows());
	}

	uint bufferSize() const {
		return delimiters.back();
	}

	JaggedArray2& operator=(const JaggedArray2& array);
};

template<> class JaggedArray2<bool> {
public:
	typedef std::vector<bool> DataBuffer;

	class RowIterator : public std::iterator<std::input_iterator_tag,
			const bool, ptrdiff_t, const bool, const bool> {
	private:
		const DataBuffer& data;
		uint i;

	public:

		RowIterator(const DataBuffer& data, const uint i) :
			data(data), i(i) {
		}

		bool operator==(const RowIterator& it) const {
			return i == it.i;
		}

		bool operator!=(const RowIterator& it) const {
			return !operator==(it);
		}

		value_type operator*() const {
			return data[i];
		}

		RowIterator& operator++() {
			++i;
			return *this;
		}
	};

	class RowProxy {
	private:
		const DataBuffer& data;
		const uint row;
		const uint limit;

	public:

		RowProxy(const DataBuffer& data, const uint row, const uint limit) :
			data(data), row(row), limit(limit) {
		}

		bool operator[](const uint offset) const {
			assert(offset < limit);
			return data[row + offset];
		}
	};

	typedef RowIterator const_iterator;

	JaggedArray2() :
		delimiters(1, 0) {
	}

	JaggedArray2(const JaggedArray2& array) :
		data(array.data), delimiters(array.delimiters) {
		assert(bufferSize() == array.bufferSize());
	}

	uint numRows() const {
		return delimiters.size() - 1;
	}

	uint sizeOf(const uint i) const {
		checkBounds(i);
		return delimiters[i + 1] - delimiters[i];
	}

	const_iterator begin(const uint i) const {
		checkBounds(i);
		return const_iterator(data, delimiters[i]);
	}

	const_iterator end(const uint i) const {
		checkBounds(i);
		return const_iterator(data, delimiters[i + 1]);
	}

	RowProxy operator[](const uint i) const {
		checkBounds(i);
		return RowProxy(data, delimiters[i], sizeOf(i));
	}

	static std::ostream& toStream(std::ostream& out, const JaggedArray2& array) {
		for (unsigned int i = 0; i < array.numRows(); ++i) {
			std::copy(array.begin(i), array.end(i),
					std::ostream_iterator<bool>(out, " "));
			out << '\n';

		}
		std::copy(array.delimiters.begin(), array.delimiters.end(),
				std::ostream_iterator<uint>(out, " "));
		return out;
	}

	static std::istream& fromStream(std::istream& is, JaggedArray2& array) {
		std::string line;
		uint index = 0;
		std::vector<bool> dataBuffer;
		while (std::getline(is, line).good()) {
			boost::trim(line);
			if (!boost::starts_with(line, "#")) {
				std::istringstream istr(line);
				while (istr.good()) {
					++index;
					bool elem;
					istr >> elem;
					dataBuffer.push_back(elem);
				}
				array.delimiters.push_back(index);
			}
		}
		array.data.resize(dataBuffer.size(), false);
		assert(dataBuffer.size() == array.bufferSize());
		for (uint i = 0; i < dataBuffer.size(); ++i) {
			array.data[i] = dataBuffer[i];
		}
		return is;
	}

private:
	DataBuffer data;
	std::vector<uint> delimiters;

	void checkBounds(const uint i) const {
		assert(i >= 0 && i < numRows());
	}

	uint bufferSize() const {
		return delimiters.back();
	}

	JaggedArray2& operator=(const JaggedArray2& array);
};

/*template<class T> std::ostream& JaggedArray2<T>::toStream(std::ostream& out,
		const JaggedArray2<T>& array) {
	for (unsigned int i = 0; i < array.numRows(); ++i) {
		std::copy(array.begin(i), array.end(i), std::ostream_iterator<T>(out,
				" "));
		out << '\n';
	}
	std::copy(array.delimiters.begin(), array.delimiters.end(),
			std::ostream_iterator<uint>(out, " "));
	return out;
}

template<class T> std::istream& JaggedArray2<T>::fromStream(std::istream& is,
		JaggedArray2<T>& array) {
	std::string line;
	uint index = 0;
	std::vector<T> dataBuffer;
	while (std::getline(is, line).good()) {
		boost::trim(line);
		if (!boost::starts_with(line, "#")) {
			std::istringstream istr(line);
			while (istr.good()) {
				++index;
				T elem;
				istr >> elem;
				dataBuffer.push_back(elem);
			}
			array.delimiters.push_back(index);
		}
	}
	assert(dataBuffer.size() == array.bufferSize());
	array.data = new T[dataBuffer.size()];
	std::copy(dataBuffer.begin(), dataBuffer.end(), array.data);
	return is;
}

std::ostream& JaggedArray2<bool>::toStream(std::ostream& out,
		const JaggedArray2<bool>& array) {
	for (unsigned int i = 0; i < array.numRows(); ++i) {
		std::copy(array.begin(i), array.end(i), std::ostream_iterator<bool>(
				out, " "));
		out << '\n';

	}
	std::copy(array.delimiters.begin(), array.delimiters.end(),
			std::ostream_iterator<uint>(out, " "));
	return out;
}

std::istream& JaggedArray2<bool>::fromStream(std::istream& is, JaggedArray2<
		bool>& array) {
	std::string line;
	uint index = 0;
	std::vector<bool> dataBuffer;
	while (std::getline(is, line).good()) {
		boost::trim(line);
		if (!boost::starts_with(line, "#")) {
			std::istringstream istr(line);
			while (istr.good()) {
				++index;
				bool elem;
				istr >> elem;
				dataBuffer.push_back(elem);
			}
			array.delimiters.push_back(index);
		}
	}
	array.data.resize(dataBuffer.size(), false);
	assert(dataBuffer.size() == array.bufferSize());
	for (uint i = 0; i < dataBuffer.size(); ++i) {
		array.data[i] = dataBuffer[i];
	}
	return is;
}*/

template<class T> std::ostream& operator<<(std::ostream& out,
		const JaggedArray2<T>& array) {
	return JaggedArray2<T>::toStream(out, array);
}

template<class T> std::istream& operator>>(std::istream& is,
		JaggedArray2<T>& array) {
	return JaggedArray2<T>::fromStream(is, array);
}

namespace std {

template<class T> void swap(JaggedArray2<T>& a, JaggedArray2<T>& b) {
	a.swap(b);
}

} // namespace std

#endif /* JAGGEDARRAY2_HPP_ */
