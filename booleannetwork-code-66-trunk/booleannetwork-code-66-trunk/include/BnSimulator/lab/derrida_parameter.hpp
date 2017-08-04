/*
 * derrida_parameter.hpp
 *
 *  Created on: May 19, 2010
 *      Author: stewie
 */

#ifndef DERRIDA_PARAMETER_HPP_
#define DERRIDA_PARAMETER_HPP_

#include <cstddef>

#include "../core/BooleanDynamics.hpp"

namespace bn {

double raw_derrida_parameter(BooleanDynamics& net, const size_t probes,
		const size_t order = 1);

inline double derrida_parameter(BooleanDynamics& net, const size_t probes,
		const size_t order = 1) {
	return raw_derrida_parameter(net, probes, order) * net.size();
}

} // namespace bn

#endif /* DERRIDA_PARAMETER_HPP_ */
