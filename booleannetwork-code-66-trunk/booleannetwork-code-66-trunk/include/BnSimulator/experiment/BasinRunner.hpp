/*
 * BasinRunner.hpp
 *
 *  Created on: Jul 28, 2009
 *      Author: stewie
 */

#ifndef BASINRUNNER_HPP_
#define BASINRUNNER_HPP_

#include <cstddef>
#include <map>
#include <functional> // for std::less

#include <boost/ptr_container/indirect_fun.hpp>

#include "../core/Attractor.hpp"

namespace bn {

/**
 * @ingroup runners
 *
 * This runner estimates the sizes of the basins of attraction of a random
 * boolean network.
 *
 * The estimate is calculated as follow: given a number of samples @e s, a
 * network is evolved to an attractor \f$A_i\f$ @e s times starting from a
 * random initial state. The size of the basin of \f$A_i\f$ is the number of
 * times that the attractor is found during the @e s runs.
 *
 * This runner uses internally a TrajectoryRunner to find attractors.
 */
class BasinRunner {
public:
	/**
	 * Maps that associates an attractor to its basin size.
	 *
	 * @note
	 * This object does not own its attractor objects, therefore the user has
	 * to manually delete them.
	 */
	typedef std::map<Attractor*, size_t, boost::indirect_fun<std::less<
			Attractor> > > BasinSizes;

	/**
	 * Initializes the maximum number of simulation steps and the number
	 * of samples to take.
	 * @param maxSteps maximum number of simulation stesp
	 * @param probes number of samples to take
	 */
	BasinRunner(const size_t maxSteps, const size_t probes) :
		maxSteps(maxSteps), probes(probes) {
	}

	BasinSizes basinSizes(BooleanNetwork& net) const;

private:
	/**
	 * Maximum number of simulation steps.
	 */
	const size_t maxSteps;
	/**
	 * Number of samples to take.
	 */
	const size_t probes;

};

}

#endif /* BASINRUNNER_HPP_ */
