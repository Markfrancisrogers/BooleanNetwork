/*
 * enumerate_flips.hpp
 *
 *  Created on: May 1, 2011
 *      Author: stewie
 */

#ifndef ENUMERATE_FLIPS_HPP_
#define ENUMERATE_FLIPS_HPP_

#include <cstddef>

#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

#include "../core/network_state.hpp"

namespace bn {

namespace gen {

namespace detail {

struct FlipIterator : boost::iterator_facade<FlipIterator, State,
		boost::single_pass_traversal_tag, State> {
public:
	FlipIterator() {
	}

	FlipIterator(State s, std::size_t i) :
		s(s), i(i) {
	}

private:
	friend class boost::iterator_core_access;
	State s;
	std::size_t i;

	void increment() {
		++i;
	}

	bool equal(const FlipIterator& other) const {
		return i == other.i;
	}

	reference dereference() const {
		return State(s).flip(i);
	}
};

FlipIterator operator++(FlipIterator& it, int) {
	FlipIterator tmp(it);
	++it;
	return tmp;
}

} // namespace detail

struct AllFlips : boost::iterator_range<detail::FlipIterator> {
	typedef boost::iterator_range<detail::FlipIterator> base;
	AllFlips() {
	}
	AllFlips(const State& s) :
		base(iterator(s, 0), iterator(s, s.size())) {
	}
};

namespace functional {

struct all_flips {
	typedef AllFlips result_type;
	result_type operator()(const State& s) const {
		return AllFlips(s);
	}
};

} // namespace functional

static const functional::all_flips all_flips = { };

} // namespace gen

} // namespace bn

#endif /* ENUMERATE_FLIPS_HPP_ */
