/*
 * boolean_analysis.hpp
 *
 *  Created on: Jun 1, 2010
 *      Author: stewie
 */

#ifndef BOOLEAN_ANALYSIS_HPP_
#define BOOLEAN_ANALYSIS_HPP_

#include <cstddef>

#include "../core/network_state.hpp"

namespace bn {

class BooleanFunction;

class BooleanDynamics;

bool derivative(const BooleanFunction& f, const size_t i, State x);

double influence(const BooleanFunction& f, const size_t i);

double sensitivity(const BooleanFunction& f);

double average_sensitivity(const BooleanDynamics& net);

} // namespace bn

#endif /* BOOLEAN_ANALYSIS_HPP_ */
