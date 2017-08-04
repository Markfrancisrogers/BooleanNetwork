/*
 * factory.hpp
 *
 *  Created on: Dec 21, 2010
 *      Author: stewie
 */

#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include <cstddef>
#include <vector>

#include "MutableBooleanNetwork.hpp"

namespace bn {

MutableBooleanNetwork make_network(
		const std::vector<std::vector<size_t> >& topology, const std::vector<
				std::vector<int> >& functions);

} // namespace bn

#endif /* FACTORY_HPP_ */
