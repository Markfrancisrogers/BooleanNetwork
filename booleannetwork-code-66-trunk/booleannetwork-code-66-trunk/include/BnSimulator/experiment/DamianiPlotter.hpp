/*
 * DamianiPlotter.hpp
 *
 *  Created on: May 19, 2010
 *      Author: stewie
 */

#ifndef DAMIANIPLOTTER_HPP_
#define DAMIANIPLOTTER_HPP_

#include <cstddef>
#include <vector>

#include "../util/DataSummary.hpp"
#include "../core/network_state.hpp"

namespace bn {

class DamianiPlotter {
public:
	/**
	 * This type representa a point in a Damiani plot.
	 */
	typedef double PlotPoint;
	/**
	 * This type represents the points in a Damiani plot.
	 */
	typedef std::vector<double> PlotData;

	DamianiPlotter(const size_t probes, const size_t maxSteps) :
		probes(probes), maxSteps(maxSteps) {
	}

	PlotData computePlot(BooleanNetwork& net, const size_t order) const;

	PlotData computePlot(BooleanNetwork& net, const double d) const;

private:
	const size_t probes;
	const size_t maxSteps;

	std::vector<State>* traceNetwork(BooleanNetwork& net, const State& s0) const;
};

} // namespace bn

#endif /* DAMIANIPLOTTER_HPP_ */
