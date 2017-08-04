/*
 * Counter.hpp
 *
 *  Created on: Aug 23, 2009
 *      Author: stewie
 */

#ifndef COUNTER_HPP_
#define COUNTER_HPP_

#include <cassert>
#include <cstddef>
#include <ostream>
#include <utility>
#include <map>
#include <algorithm> // for std::swap
#include <functional> // for std::less

#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/ptr_container/indirect_fun.hpp>

namespace bn {

namespace util {

/**
 * This data structure keeps track of the number of insertion attempts of an
 * element.
 *
 * This class implements a map that associates a key element to the number of
 * insertions of that element in the map. Differently from an STL multiset,
 * this class stores only @e one instance of each key element. Like other STL
 * associative containers, keys are compared for equality using the @a Comparator
 * type provided during instantiation.
 *
 * Typical use case of this class are in implementations of word counters or
 * histograms.
 *
 * This class handles object ownership in a customizable way. If type parameter
 * @a T is instantiated to a non-pointer type, the ownership follows the same
 * rules as for STL containers (that is the container stores copies of the
 * inserted elements). On the other hand, if type parameter @a T is a pointer
 * type, this class stores <a href="http://www.boost.org/libs/smart_ptr/">
 * boost::shared_ptr</a>'s of elements, that is, its value_type member type is
 * boost::shared_ptr<boost::remove_pointer<T>::type>. This way memory management
 * is simplified.
 *
 * Although only objects of type key_type can be inserted (like an STL set),
 * all iterator types return, when dereferenced, STL pairs of key_type and
 * mapped_type objects (like an STL map).
 *
 * @see <a href="http://www.boost.org/libs/smart_ptr/">The Boost Smart Pointer
 * Library</a> for instructions on how to use shared pointers, and
 * <a href="http://www.boost.org/libs/type_traits/">The Boost Type Traits
 * Library</a>.
 */
template<class T, class Comparator = std::less<
		typename boost::remove_pointer<T>::type> > class Counter {
private:
	/**
	 * MPL integral type equal to boost::mpl::true_ if @a T is a pointer type.
	 */
	typedef typename boost::is_pointer<T>::type is_ptr;
	/**
	 * Key type of the underlying associative container.
	 */
	typedef typename boost::mpl::if_<is_ptr, boost::shared_ptr<
			typename boost::remove_pointer<T>::type>, T>::type stored_type;
	/**
	 * Key comparator of the underlying associative container.
	 */
	typedef typename boost::mpl::if_<is_ptr, boost::indirect_fun<Comparator>,
			Comparator>::type comparator_type;
	/**
	 * Type of the underlying associative container.
	 */
	typedef std::map<stored_type, size_t, comparator_type> Container;

	/**
	 * Actual container that backs this class.
	 */
	Container map;
	/**
	 * Total number of insertions performed.
	 */
	size_t count;

public:
	/**
	 * Type of the elements contained in this class.
	 */
	typedef typename Container::key_type key_type;
	/**
	 * Integral type used to count the insertions.
	 */
	typedef typename Container::mapped_type mapped_type;
	/**
	 * Key comparator type.
	 */
	typedef typename Container::key_compare key_compare;
	/**
	 * size method returns object of this type.
	 */
	typedef typename Container::size_type size_type;
	/**
	 * Input iterator for this class.
	 *
	 * @todo
	 * Actually this is a bidirectional iterator (iterator type of the
	 * underlying STL map), but this would break incapsulation!
	 */
	typedef typename Container::iterator iterator;
	/**
	 * Input constant iterator for this class.
	 */
	typedef typename Container::const_iterator const_iterator;

	/**
	 * Initializes an empty Counter object.
	 * @param comp a comparator object for the keys
	 */
	explicit Counter(const comparator_type& comp = comparator_type()) :
		map(comp), count(0) {
	}

	/**
	 * Initializes a Counter object with a range of elements.
	 * @param first input iterator to the initial position of the first range
	 * @param last input iterator to the final position of the first range
	 * @param comp a comparator object for the keys
	 */
	template<class InputIterator> Counter(InputIterator first,
			const InputIterator last, const comparator_type& comp =
					comparator_type()) :
		map(comp), count(0) {
		for (; first != last; ++first)
			insert(*first);
	}

	/**
	 * Initializes a Counter object with a range of elements.
	 *
	 * This overload works with Boost ranges instead of iterators.
	 * @param r a single pass range
	 * @param comp a comparator object for the keys
	 */
	template<class SinglePassRange> explicit Counter(const SinglePassRange& r,
			const comparator_type& comp = comparator_type()) :
		map(comp), count(0) {
		for (typename boost::range_iterator<SinglePassRange>::type first =
				boost::begin(r), last = boost::end(r); first != last; ++first)
			insert(*first);
	}

	/**
	 * Copy constructor.
	 * @param c another Counter object
	 */
	Counter(const Counter& c) :
		map(c.map), count(c.count) {
	}

	/**
	 * Returns the number of key elements stored in this container.
	 * @return the number of key elements
	 */
	size_type size() const {
		return map.size();
	}

	/**
	 * Returns the number of key element insertions since the instantiation of
	 * this object.
	 *
	 * Joined with Counter::operator[](), it can be useful to compute ratios.
	 * @return the number of key element insertions
	 */
	size_type insertions() const {
		return count;
	}

	/**
	 * Tests if this container is empty.
	 * @return @e true if this container is empty
	 */
	bool empty() const {
		return map.empty();
	}

	/**
	 * Iterator to the beginning of the container.
	 * @return beginning iterator
	 */
	iterator begin() {
		return map.begin();
	}

	/**
	 * Iterator to the end of the container.
	 * @return end iterator
	 */
	iterator end() {
		return map.end();
	}

	/**
	 * Constant iterator to the beginning of the container.
	 * @return constant beginning iterator
	 */
	const_iterator begin() const {
		return map.begin();
	}

	/**
	 * Constant iterator to the end of the container.
	 * @return constant end iterator
	 */
	const_iterator end() const {
		return map.end();
	}

	/**
	 * Inserts a key element in this container and increments its count.
	 *
	 * Like an STL associative container, this method returns a pair whose
	 * @e first member is an iterator of this class and its @e second member
	 * is @e true if a new element was inserted.
	 * @param key an element to insert
	 * @return an STL pair
	 */
	std::pair<iterator, bool> insert(const key_type& key) {
		std::pair<iterator, bool> p = map.insert(
				typename Container::value_type(key, 1));
		if (!p.second)
			++(p.first->second);
		++count;
		return p;
	}

	/**
	 * Implementation of swap function.
	 * @param other a Counter object
	 */
	void swap(Counter& other) {
		std::swap(map, other.map);
		std::swap(count, other.count);
	}

	/**
	 * Returns the number of insertions of element @a key in this container.
	 * @param key a key element
	 * @return number of insertions of @a key element
	 */
	mapped_type operator[](const key_type& key) const {
		const typename Container::const_iterator it = map.find(key);
		return it == map.end() ? 0 : it->second;
	}

	template<class U, class Comp> friend std::ostream& operator<<(
			std::ostream& out, const Counter<U, Comp>& c);
};

/**
 * Prints a Counter to a stream.
 *
 * First it prints a key element using operator<<, then prints its count
 * separated by a space
 * @param out an output stream
 * @param c a Counter object
 * @return the stream passed as first argument
 */
template<class T, class Comparator> std::ostream& operator<<(std::ostream& out,
		const Counter<T, Comparator>& c) {
	for (typename Counter<T, Comparator>::const_iterator it = c.begin(), end =
			c.end(); it != end; ++it)
		out << it->first << ' ' << it->second << '\n';
	return out;
}

}

}

namespace std {

/**
 * Specialization that calls bn::util::Counter::swap(bn::util::Counter&)
 * on the first argument.
 * @param a a Counter object
 * @param b another Counter object
 */
template<class T, class Comparator> void swap(
		bn::util::Counter<T, Comparator>& a,
		bn::util::Counter<T, Comparator>& b) {
	a.swap(b);
}

}

#endif /* COUNTER_HPP_ */
