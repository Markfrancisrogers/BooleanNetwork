/*
 * state_util.hpp
 *
 *  Created on: May 20, 2010
 *      Author: stewie
 */

#ifndef STATE_UTIL_HPP_
#define STATE_UTIL_HPP_

#include <cstddef>

#include "../core/network_state.hpp"

namespace bn {

class BooleanDynamics;

namespace util {

inline size_t hamming_distance(const State& a, const State& b) {
	return (a ^ b).count();
}

bool next(State& s);

State random_state(const size_t n);

State random_state(const BooleanDynamics& net);

State perturb_state_randomly(State s, const size_t h);

} // namespace util

} // namespace bn

#endif /* STATE_UTIL_HPP_ */
