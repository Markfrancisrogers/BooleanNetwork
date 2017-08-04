/*
 * BasinRunner.cpp
 *
 *  Created on: Jul 28, 2009
 *      Author: stewie
 */

#include <memory> // for std::auto_ptr

#include <BnSimulator/core/network_state.hpp>
#ifndef NDEBUG
#include <BnSimulator/core/BooleanNetwork.hpp>
#endif
#include <BnSimulator/experiment/TrajectoryRunner.hpp>
#include <BnSimulator/experiment/BasinRunner.hpp>

namespace bn {

using namespace std;

/**
 * Execute the experiment given a random boolean network.
 * @param net a network to simulate
 * @return a BasinSizes object
 */
BasinRunner::BasinSizes BasinRunner::basinSizes(BooleanNetwork& net) const {
	BasinSizes map;
	TrajectoryRunner traj(maxSteps);
	for (size_t i = 0; i < probes; ++i) {
		const auto_ptr<Trajectory> t(traj.findAttractor(net));
		if (t->cycleLength() > 0) {
			auto_ptr<Attractor> attractor(new Attractor(*t));
#ifndef NDEBUG
			State temp = net.getState();
			net.setState(attractor->getRepresentant());
			net.update(attractor->getLength());
			assert(net.getState() == attractor->getRepresentant());
			net.setState(temp);
#endif
			const pair<BasinSizes::iterator, bool> p = map.insert(
					BasinSizes::value_type(attractor.get(), 1));
			if (!p.second) {
				++(p.first->second);
			} else
				attractor.release();
		}
	}
	return map;
}

}
