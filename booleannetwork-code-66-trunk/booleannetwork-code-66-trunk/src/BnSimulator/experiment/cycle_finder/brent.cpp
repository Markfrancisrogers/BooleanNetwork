/*
 * brent.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: stewie
 */

#include <BnSimulator/experiment/cycle_finder/brent.hpp>

namespace bn {

namespace cycle_finder {

Attractor brent(BooleanDynamics& dyn, State s) {
	std::size_t power = 1, lambda = 1;
	bn::State tortoise = s;
	dyn.update(s);
	while (tortoise != s) {
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
