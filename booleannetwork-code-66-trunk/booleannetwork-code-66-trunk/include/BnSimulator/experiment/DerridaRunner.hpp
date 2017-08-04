/*
 * DerridaRunner.hpp
 *
 *  Created on: Jul 31, 2009
 *      Author: stewie
 */

#ifndef DERRIDARUNNER_HPP_
#define DERRIDARUNNER_HPP_

#include <cassert>
#include <cstddef>
#include <vector>

#include "../util/DataSummary.hpp"
#include "../core/BooleanNetwork_fwd.hpp"

namespace bn {

/**
 * @ingroup runners
 *
 * This runner computes ordinary, generalized and modified Derrida plot for a
 * random boolean network.
 *
 * Given a network with @e n nodes, three Derrida plots can be defined:
 * - Ordinary Derrida plots map the hamming distance @e h between two initial
 * 	states \f$s_0\f$ and \f$s'_0\f$ to the distance between their successors
 * 	\f$s_1\f$ and \f$s'_1\f$ for each \f$h = 0 \ldots n\f$.
 * - Generalized Derrida plots are different than ordinary Derrida plots in that
 * 	they evaluate the hamming distance between the states after \f$t > 1\f$
 * 	simulation steps. @e t is called @e order of the plot.
 * - Modified Derrida plots are ordinary Derrida plots in which the initial state
 * 	\f$s_0\f$ belongs to an attractor (\f$s'_0\f$ can lay outside a cycle).
 *
 * Each point of a plot is computed by averaging the samples obtained through a
 * set number of runs.
 *
 * The collection of points of a Derrida plot are implicitly represented by the
 * type PlotData, which is a sequence of DataSummary objects. Each object at
 * index @e h in the sequence represent the value for an initial distance of @e h.
 *
 * @see DataSummary for the representation of plot points.
 */
class DerridaRunner {
public:
	/**
	 * This type representa a point in a Derrida plot.
	 */
	typedef util::DataSummary<double> PlotPoint;
	/**
	 * This type represents the points in a Derrida plot.
	 */
	typedef std::vector<PlotPoint> PlotData;

	/**
	 * Initializes runner instance parameters.
	 *
	 * A parameter \f$step > 0\f$ controls the resolution of Derrida plots by
	 * setting the increment of the initial perturbation between two
	 * successive points. For some @a step, @e h takes values in
	 * \f$1, \ step + 1, \ 2 \, step + 1, \ 3 \, step + 1 \ldots\f$.
	 * @param probes samples to obtain for each plot point
	 * @param step resolution of Derrida plots
	 * @param maxSteps maximum number of simulation stesp for each sample
	 *
	 */
	DerridaRunner(const size_t probes, const size_t step, const size_t maxSteps) :
		probes(probes), step(step), maxSteps(maxSteps) {
		assert(step > 0);
	}

	PlotData computePlot(BooleanNetwork& net, const size_t order) const;

	PlotData computeModifiedPlot(BooleanNetwork& net, const size_t order) const;

private:
	/**
	 * Samples to obtain for each plot point.
	 */
	const size_t probes;
	/**
	 * Resolution of Derrida plots.
	 */
	const size_t step;
	/**
	 * Maximum number of simulation stesp for each sample
	 */
	const size_t maxSteps;
};

}

#endif /* DERRIDARUNNER_HPP_ */
