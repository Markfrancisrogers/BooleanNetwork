/*
 * basin_of_attraction.hpp
 *
 *  Created on: May 12, 2011
 *      Author: stewie
 */

#ifndef BASIN_OF_ATTRACTION_HPP_
#define BASIN_OF_ATTRACTION_HPP_

#include "../util/Counter.hpp"
#include "cycle_finder.hpp"

namespace bn {

template<class StateRange, class Strategy> util::Counter<State> basin_of_attraction(const StateRange& r, const Strategy& s){
	return util::Counter<State>(r | s);
}

} // namespace bn

#endif /* BASIN_OF_ATTRACTION_HPP_ */
