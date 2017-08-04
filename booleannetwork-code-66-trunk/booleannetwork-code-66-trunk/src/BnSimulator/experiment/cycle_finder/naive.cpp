/*
 * naive.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: stewie
 */

#include <BnSimulator/experiment/cycle_finder/naive.hpp>

namespace bn {

namespace cycle_finder {

Attractor naive(BooleanDynamics& net, State s) {
	StateSet stateSet;
	for (; true; net.update(s)) {
		const std::pair<StateSet::const_iterator, bool> p = stateSet.push_back(
				s);
		if (!p.second)
			return Attractor(std::make_pair(p.first, stateSet.end()));
	}
}

} // namespace cycle_finder

} // namespace bn
