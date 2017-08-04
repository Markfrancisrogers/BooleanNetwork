/*
 * brent.hpp
 *
 *  Created on: Apr 30, 2011
 *      Author: stewie
 */

#ifndef BRENT_HPP_
#define BRENT_HPP_

#include <cstddef>

#include "../../core/BooleanDynamics.hpp"
#include "../../core/Attractor.hpp"
#include "../TrajectoryRange.hpp"

namespace bn {

namespace cycle_finder {

Attractor brent(BooleanDynamics& dyn, State s);

template<class Terminator> Attractor brent(BooleanDynamics& dyn, State s,
		Terminator term) {
	std::size_t power = 1, lambda = 1;
	State tortoise = s;
	dyn.update(s);
	for (size_t iter = 0; tortoise != s; ++iter) {
		if (term(iter))
			return EMPTY_ATTRACTOR;
		if (power == lambda) {
			tortoise = s;
			power *= 2;
			lambda = 0;
		}
		dyn.update(s);
		++lambda;
	}
	// now state s is inside a cycle
	return Attractor(TrajectoryRange(dyn, s, lambda));
}

} // namespace cycle_finder

} // namespace bn

#endif /* BRENT_HPP_ */
