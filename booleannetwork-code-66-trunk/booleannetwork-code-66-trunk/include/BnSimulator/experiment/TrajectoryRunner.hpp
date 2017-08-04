/*
 * TrajectoryRunner.hpp
 *
 *  Created on: Jul 15, 2009
 *      Author: stewie
 */

#ifndef TRAJECTORYRUNNER_HPP_
#define TRAJECTORYRUNNER_HPP_

#include <cstddef>
#include <limits>

#include "../core/BooleanNetwork_fwd.hpp"
#include "../core/Trajectory.hpp"

namespace bn {

/**
 * @ingroup runners
 *
 * This runner finds a trajectory in the state space.
 *
 * Typically this class is used to find attractors (that is cycles in the state
 * space). This runner accepts a maximum number of execution steps that the
 * boolean network can perform in order to prevent memory overflow for the
 * simulation of chaotic networks, which can have very long cycles.
 *
 * @see Trajectory
 */
class TrajectoryRunner {
private:
	/**
	 * Maximum number of simulation steps.
	 */
	size_t maxSteps;

public:
	/**
	 * Default constructor for this runner.
	 *
	 * Initializes the maximum number of simulation steps to the largets unsigned
	 * integer available on the system.
	 */
	TrajectoryRunner() :
		maxSteps(std::numeric_limits<size_t>::max()) {
	}

	/**
	 * Initializes the maximum number of simulation steps to the value specified.
	 * @param maxSteps maximum number of simulation stesp
	 */
	TrajectoryRunner(const size_t maxSteps) :
		maxSteps(maxSteps) {
	}

	Trajectory* findAttractor(BooleanNetwork& net) const;

	Trajectory
			* findAttractor(BooleanNetwork& net, const State& initialState) const;
};

}

#endif /* TRAJECTORYRUNNER_HPP_ */
