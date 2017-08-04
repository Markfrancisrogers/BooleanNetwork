#ifndef NETWORKDYNAMICS_HPP_
#define NETWORKDYNAMICS_HPP_

#include <cstddef>
#include <limits>

#include "../core/network_state.hpp"
#include "../core/BooleanDynamics.hpp"
#include "../core/Attractor.hpp"

namespace bn {

class NetworkDynamics {
private:
	size_t maxSteps;
	size_t muteSteps;

public:

	NetworkDynamics() :
		maxSteps(std::numeric_limits<size_t>::max()), muteSteps(0) {
	}

	NetworkDynamics(const size_t maxSteps, const size_t muteSteps = 0) :
		maxSteps(maxSteps), muteSteps(muteSteps) {
	}

	Attractor* findAttractor(BooleanDynamics& net) const;

	Attractor
			* findAttractor(BooleanDynamics& net, const State& initialState) const;
};

} // namespace bn

#endif /* NETWORKDYNAMICS_HPP_ */
