/*
 * PersistentPerturbation.hpp
 *
 *  Created on: Sep 2, 2009
 *      Author: stewie
 */

#ifndef PERSISTENTPERTURBATION_HPP_
#define PERSISTENTPERTURBATION_HPP_

#include <cassert>
#include <cstddef>
#include <vector>
#include <functional>

#include <boost/iterator/counting_iterator.hpp>

#include "network_state.hpp"

namespace bn {

class PersistentPerturbation : public std::unary_function<State&, void> {
protected:
	const State init;

public:
	PersistentPerturbation() :
		init(0) {
	}

	PersistentPerturbation(const State& init) :
		init(init) {
	}

	const State& getState() const {
		return init;
	}

	virtual ~PersistentPerturbation() {
	}

	virtual result_type operator()(argument_type s) = 0;
};

class Flip : public PersistentPerturbation {
private:
	const size_t i;
	const bool value;

public:
	Flip(State s, const size_t i) :
		PersistentPerturbation(s.flip(i)), i(i), value(s[i]) {
		assert(init[i] == value);
	}

	result_type operator()(argument_type s) {
		s[i] = value;
	}
};

class Clamp : public PersistentPerturbation {
private:
	const State andMask;
	const State orMask;

public:
	Clamp(const State& value, const State& mask) :
		PersistentPerturbation(value), andMask(~mask), orMask(mask & value) {
	}

	result_type operator()(argument_type s) {
		s &= andMask;
		s |= orMask;
#ifndef NDEBUG
		State temp(s);
		temp &= andMask;
		temp |= orMask;
		assert(temp == s);
#endif
	}
};

class PullUp : public PersistentPerturbation {
private:
	const State mask;

public:
	PullUp(const State& mask, const State& init) :
		PersistentPerturbation(init | mask), mask(mask) {
	}

	PullUp(const State& mask) :
		mask(mask) {
	}

	result_type operator()(argument_type s) {
		s |= mask;
	}
};

class PullDown : public PersistentPerturbation {
private:
	const State mask;

public:
	PullDown(const State& mask, const State& init) :
		PersistentPerturbation(init & ~mask), mask(~mask) {
	}

	PullDown(const State& mask) :
		mask(~mask) {
	}

	result_type operator()(argument_type s) {
		s &= mask;
	}
};

Clamp* perturb_state_randomly(State s, const size_t h) {
#ifndef NDEBUG
	State temp = s;
#endif
	const size_t n = s.size();
	State mask(n);
	std::vector<size_t> indexes(boost::counting_iterator<size_t>(0),
			boost::counting_iterator<size_t>(n));
	size_t limit = n;
	for (size_t i = 0; i < h; ++i) {
		const size_t current = rand() % (limit--);
		s.flip(indexes[current]);
		mask.flip(indexes[current]);
		std::swap(indexes[limit], indexes[current]);
	}
#ifndef NDEBUG
	assert((temp ^ s).count() == h);
	Clamp* const clamp = new Clamp(s, mask);
	assert((temp ^ clamp->getState()).count() == h);
	(*clamp)(temp);
	assert(temp == s);
	delete clamp;
#endif
	return new Clamp(s, mask);
}

}

#endif /* PERSISTENTPERTURBATION_HPP_ */
