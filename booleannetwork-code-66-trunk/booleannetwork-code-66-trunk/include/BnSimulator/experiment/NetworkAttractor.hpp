/*
 * NetworkAttractor.hpp
 *
 *  Created on: Nov 16, 2010
 *      Author: stewie
 */

#ifndef NETWORKATTRACTOR_HPP_
#define NETWORKATTRACTOR_HPP_

#include <cstddef>
#include <climits>

#include "../core/network_state.hpp"

namespace bn {

class Attractor;

class BooleanDynamics;

class NetworkAttractor {
public:
	NetworkAttractor() :
		maxSteps(std::numeric_limits<size_t>::max()) {
	}

	NetworkAttractor(const size_t maxSteps, const size_t muteSteps = 0) :
		maxSteps(maxSteps) {
	}

	Attractor* findAttractor(BooleanDynamics& net) const;

	Attractor
			* findAttractor(BooleanDynamics& net, const State& initialState) const;

private:
	size_t maxSteps;
};

} // namespace bn


#endif /* NETWORKATTRACTOR_HPP_ */
