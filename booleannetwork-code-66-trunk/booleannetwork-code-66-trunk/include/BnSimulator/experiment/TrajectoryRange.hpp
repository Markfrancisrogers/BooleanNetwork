/*
 * TrajectoryRange.hpp
 *
 *  Created on: Apr 30, 2011
 *      Author: stewie
 */

#ifndef TRAJECTORYRANGE_HPP_
#define TRAJECTORYRANGE_HPP_

#include <cstddef>

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

#include "../core/network_state.hpp"
#include "../core/BooleanDynamics.hpp"

namespace bn {

namespace detail {

struct TrajectoryIterator : boost::iterator_facade<TrajectoryIterator,
		const State, boost::single_pass_traversal_tag> {
public:
	// end
	TrajectoryIterator(BooleanDynamics& dyn, const std::size_t n) :
		dyn(dyn), n(n) {
	}

	// begin
	TrajectoryIterator(BooleanDynamics& dyn, const State& s) :
		dyn(dyn), s(s), n(0) {
	}

private:
	friend class boost::iterator_core_access;

	BooleanDynamics& dyn;
	State s;
	std::size_t n;

	reference dereference() const {
		return s;
	}

	bool equal(const TrajectoryIterator& other) const {
		return n == other.n;
	}

	void increment() {
		dyn.update(s);
		++n;
	}
};

TrajectoryIterator operator++(TrajectoryIterator& it, int) {
	TrajectoryIterator tmp(it);
	++it;
	return tmp;
}

} // namespace detail

struct TrajectoryRange : boost::iterator_range<detail::TrajectoryIterator> {
private:
	typedef boost::iterator_range<detail::TrajectoryIterator> base;

public:
	TrajectoryRange(BooleanDynamics& dyn, const State& s, const std::size_t n) :
		base(iterator(dyn, s), iterator(dyn, n)) {
	}
};

} // namespace bn

#endif /* TRAJECTORYRANGE_HPP_ */
