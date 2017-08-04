/*
 * PerturbativeRunner.hpp
 *
 *  Created on: Jul 18, 2009
 *      Author: stewie
 */

#ifndef PERTURBATIVERUNNER_HPP_
#define PERTURBATIVERUNNER_HPP_

#include <cstddef>
#include <map>
#include <functional> // for std::less

#include <boost/ptr_container/indirect_fun.hpp>

#include "../core/BooleanDynamics_fwd.hpp"
#include "../core/Attractor.hpp"

namespace bn {

/**
 * @ingroup runners
 *
 * This runner estimates the reachability of attractors in the state space
 * given a fixed perturbation magnitude (fixed to 1).
 *
 * Given a source attractor @e A, this experiments estimates the reachability
 * of an attractor @e B (with @e B which might be equal to @e A) by counting
 * the number of times the network evolves to @e B starting from a perturbed
 * state in the source attractor. This process is repeated for each state in
 * @e A and for each number of samples to take. State perturbations are
 * performed by randomly choosing @e h different nodes to flip.
 *
 * This runner uses internally a TrajectoryRunner to find attractors.
 *
 * @see BasinRunner or GardenerRunner for similar experiments and the definition
 * of coherency of an attractor.
 *
 * @todo
 * Obtain also transient length statistics, since they are easilly available.
 */
class PerturbativeRunner {
public:
	/**
	 * This type maps Attractor objects to their reachability.
	 *
	 * @note
	 * Instances of this type do not have ownership of their contained objects.
	 */
	typedef std::map<Attractor*, size_t, boost::indirect_fun<std::less<
			Attractor> > > Equilibria;
	/**
	 * This type maps ExplicitAttractor objects to their reachability.
	 *
	 * @note
	 * Instances of this type do not have ownership of their contained objects.
	 */
	typedef std::map<ExplicitAttractor*, size_t, boost::indirect_fun<std::less<
			Attractor> > > ExplicitEquilibria;

	/**
	 * Initializes the maximum number of simulation steps and the number
	 * of samples to take.
	 * @param maxSteps maximum number of simulation stesp
	 * @param probes number of samples
	 */
	PerturbativeRunner(const size_t maxSteps, const size_t probes) :
		maxSteps(maxSteps), probes(probes) {
	}

	Equilibria
	perturbAttractor(BooleanDynamics&, const ExplicitAttractor&,
			const size_t h = 1) const;

	ExplicitEquilibria
	perturbAttractorExplicit(BooleanDynamics&, const ExplicitAttractor&,
			const size_t h = 1) const;

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

#endif /* PERTURBATIVERUNNER_HPP_ */
