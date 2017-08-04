/*
 * cycle_finder.hpp
 *
 *  Created on: Apr 2, 2011
 *      Author: stewie
 */

#ifndef CYCLE_FINDER_HPP_
#define CYCLE_FINDER_HPP_

#include <cstddef>
#include <utility>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/adaptor/argument_fwd.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/mpl/void_fwd.hpp>

#include "cycle_finder/naive.hpp"
#include "cycle_finder/brent.hpp"

namespace bn {

namespace detail {

template<class It, class Strategy> struct AttractorIterator : boost::iterator_facade<
		AttractorIterator<It, Strategy> , const Attractor,
		boost::single_pass_traversal_tag> {
public:
	// begin
	AttractorIterator(const It iter, const It e, const Strategy& st) :
		it(iter), end(e), s(st) {
		ensureInvariant();
	}

	// end
	AttractorIterator(const It e, const Strategy& st) :
		it(e), end(e), s(st) {
	}

private:
	typedef boost::iterator_facade<AttractorIterator<It, Strategy> ,
			const Attractor, boost::forward_traversal_tag> base;
	friend class boost::iterator_core_access;
	It it, end;
	Strategy s;
	Attractor current;

	void ensureInvariant() {
		while (it != end && (current = s(*it)) == EMPTY_ATTRACTOR)
			++it;
	}

	typename base::reference dereference() const {
		return current;
	}

	bool equal(const AttractorIterator& other) const {
		return it == other.it;
	}

	void increment() {
		++it;
		ensureInvariant();
	}
};

template<class It, class Strategy> AttractorIterator<It, Strategy> operator++(
		AttractorIterator<It, Strategy>& it, int) {
	AttractorIterator<It, Strategy> temp(it);
	++it;
	return temp;
}

template<class StateRange, class Strategy> struct AttractorRange : boost::iterator_range<
		AttractorIterator<typename boost::range_iterator<StateRange>::type,
				Strategy> > {
private:
	typedef boost::iterator_range<AttractorIterator<
			typename boost::range_iterator<StateRange>::type, Strategy> > base;

public:
	AttractorRange(const StateRange& r, const Strategy& s) :
				base(typename base::iterator(r.begin(), r.end(), s),
						typename base::iterator(r.end(), s)) {
	}
};

template<class Strategy, class Terminator> struct CycleFinder {
	typedef Attractor result_type;
	BooleanDynamics& dyn;
	Terminator t;
	CycleFinder(BooleanDynamics& dyn, const Terminator& t) :
		dyn(dyn), t(t) {
	}
	result_type operator()(const State& s) const {
		return Strategy::call(dyn, s, t);
	}
};

template<class Strategy> struct CycleFinder<Strategy, boost::mpl::void_> {
	typedef Attractor result_type;
	BooleanDynamics& dyn;
	CycleFinder(BooleanDynamics& dyn) :
		dyn(dyn) {
	}
	result_type operator()(const State& s) const {
		return Strategy::call(dyn, s);
	}
};

struct NaiveStrategy {
	template<class Terminator> static Attractor call(BooleanDynamics& dyn,
			const State& s, const Terminator& t) {
		return cycle_finder::naive(dyn, s, t);
	}

	static Attractor call(BooleanDynamics& dyn, const State& s) {
		return cycle_finder::naive(dyn, s);
	}
};

struct BrentStrategy {
	template<class Terminator> static Attractor call(BooleanDynamics& dyn,
			const State& s, const Terminator& t) {
		return cycle_finder::brent(dyn, s, t);
	}

	static Attractor call(BooleanDynamics& dyn, const State& s) {
		return cycle_finder::brent(dyn, s);
	}
};

} // namespace detail

template<class SR, class S> detail::AttractorRange<SR, S> find_attractors(
		const SR& r, const S& s) {
	return detail::AttractorRange<SR, S>(r, s);
}

struct MaxIter {
	const size_t m;
	MaxIter(const size_t m) :
		m(m) {
	}
	bool operator()(const size_t iter) const {
		return iter < m;
	}
};

struct NotNone {
	bool operator()(const Attractor& a) const {
		return a != EMPTY_ATTRACTOR;
	}
};

typedef detail::CycleFinder<detail::NaiveStrategy, boost::mpl::void_>
		NaiveCycleFinder;

NaiveCycleFinder naive(BooleanDynamics& dyn) {
	return NaiveCycleFinder(dyn);
}

template<class Terminator> detail::CycleFinder<detail::NaiveStrategy,
		Terminator> naive(BooleanDynamics& dyn, const Terminator& t) {
	return detail::CycleFinder<detail::NaiveStrategy, Terminator>(dyn, t);
}

detail::CycleFinder<detail::BrentStrategy, boost::mpl::void_> brent(
		BooleanDynamics& dyn) {
	return detail::CycleFinder<detail::BrentStrategy, boost::mpl::void_>(dyn);
}

template<class Terminator> detail::CycleFinder<detail::BrentStrategy,
		Terminator> brent(BooleanDynamics& dyn, const Terminator& t) {
	return detail::CycleFinder<detail::BrentStrategy, Terminator>(dyn, t);
}

template<class SinglePassRange, class Strategy, class Terminator>
detail::AttractorRange<SinglePassRange, Strategy> operator|(
		SinglePassRange& rng,
		const detail::CycleFinder<Strategy, Terminator>& f) {
	return find_attractors(rng, f);
}

template<class SinglePassRange, class Strategy, class Terminator>
detail::AttractorRange<SinglePassRange, Strategy> operator|(
		const SinglePassRange& rng,
		const detail::CycleFinder<Strategy, Terminator>& f) {
	return find_attractors(rng, f);
}

} // namespace bn

#endif /* CYCLE_FINDER_HPP_ */
