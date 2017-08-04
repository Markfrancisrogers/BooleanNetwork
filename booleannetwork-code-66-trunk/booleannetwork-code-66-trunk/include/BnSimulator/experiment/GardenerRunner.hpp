/*
 * GardenerRunner.hpp
 *
 *  Created on: Jul 15, 2009
 *      Author: stewie
 */

#ifndef GARDENERRUNNER_HPP_
#define GARDENERRUNNER_HPP_

#include <cstddef>
#include <set>
#include <functional> // for std::less

#include <boost/ptr_container/indirect_fun.hpp>

#include "../core/network_state.hpp"
#include "../core/BooleanNetwork_fwd.hpp"
#include "../core/Attractor.hpp"

namespace bn {

/**
 * @ingroup runners
 *
 * This runner finds a set of attractors (garden) of a boolean network.
 *
 * A network is repeatedly simulated and all attractors found are stored in a
 * set. A number of maximum simulation steps must be provided.
 *
 * This runner uses internally a TrajectoryRunner to find attractors.
 *
 * @see BasinRunner for a similar experiment.
 */
class GardenerRunner {
public:
	/**
	 * A set of attractors.
	 */
	typedef std::set<Attractor*, boost::indirect_fun<std::less<Attractor> > >
			Garden;

	/**
	 * Initializes the maximum number of simulation steps and the maximum number
	 * of attractors to find during an experiment (including duplicates).
	 * @param maxSteps maximum number of simulation stesp
	 * @param probes maximum number of attractors
	 */
	GardenerRunner(const size_t maxSteps, const size_t probes) :
		maxSteps(maxSteps), probes(probes) {
	}

	Garden findGarden(BooleanNetwork& net) const;

	Garden enumerateGarden(BooleanNetwork& net) const;

private:
	/**
	 * Maximum number of simulation steps.
	 */
	const size_t maxSteps;
	/**
	 * Maximum number of attractors to find during an experiment (including
	 * duplicates).
	 */
	const size_t probes;

};

}

#endif /* GARDENERRUNNER_HPP_ */
