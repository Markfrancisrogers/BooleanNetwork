#include <cmath>

#include <boost/scoped_ptr.hpp>
/*#include <boost/accumulators/accumulators.hpp>
 #include <boost/accumulators/statistics/stats.hpp>
 #include <boost/accumulators/statistics/min.hpp>
 #include <boost/accumulators/statistics/mean.hpp>
 #include <boost/accumulators/statistics/variance.hpp>
 #include <boost/accumulators/statistics/max.hpp>*/

#include <BnSimulator/util/utility.hpp>
#include <BnSimulator/util/state_util.hpp>
#include <BnSimulator/core/BooleanNetwork.hpp>
#include <BnSimulator/experiment/DamianiPlotter.hpp>

namespace bn {

using namespace std;

using namespace boost;

DamianiPlotter::PlotData DamianiPlotter::computePlot(BooleanNetwork& net,
		const size_t order) const {
	assert(order > 0);
	PlotData data(maxSteps);
	for (size_t p = 0; p < probes; ++p) {
		State s0 = util::random_state(net.size());
		State sx = util::perturb_state_randomly(s0, order);
		assert(util::hamming_distance(s0, sx) == order);
		scoped_ptr<vector<State> > t0(traceNetwork(net, s0));
		scoped_ptr<vector<State> > tx(traceNetwork(net, sx));
		for (size_t i = 0; i < maxSteps; ++i) {
			data[i] += (static_cast<double> (util::hamming_distance((*t0)[i],
					(*tx)[i])) / net.size()) / probes;
		}
	}
	return data;
}

DamianiPlotter::PlotData DamianiPlotter::computePlot(BooleanNetwork& net,
		const double d) const {
	assert(d >= 0 && d <= 1);
	return computePlot(net,
			static_cast<size_t> (std::floor(net.size() * d)));
}

vector<State>* DamianiPlotter::traceNetwork(BooleanNetwork& net,
		const State& s0) const {
	net.setState(s0);
	vector<State>* trajectory = new vector<State> (maxSteps);
	for (size_t t = 0; t < maxSteps; ++t, net.update()) {
		(*trajectory)[t] = net.getState();
	}
	return trajectory;
}

} // namespace bn
