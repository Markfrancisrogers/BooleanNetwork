/*
 * naive.hpp
 *
 *  Created on: Apr 30, 2011
 *      Author: stewie
 */

#ifndef NAIVE_HPP_
#define NAIVE_HPP_

#include <utility>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include "../../core/BooleanDynamics.hpp"
#include "../../core/Attractor.hpp"

namespace bn {

namespace cycle_finder {

typedef boost::multi_index::multi_index_container<State,
		boost::multi_index::indexed_by<boost::multi_index::sequenced<>,
				boost::multi_index::hashed_unique<boost::multi_index::identity<
						State>, BitsetHasher> > > StateSet;

Attractor naive(BooleanDynamics& net, State s);

template<class Terminator> Attractor naive(BooleanDynamics& net, State s,
		Terminator t) {
	StateSet stateSet;
	for (size_t iter = 0; t(iter); net.update(s), ++iter) {
		const std::pair<StateSet::const_iterator, bool> p = stateSet.push_back(
				s);
		if (!p.second)
			return Attractor(std::make_pair(p.first, stateSet.end()));
	}
	return EMPTY_ATTRACTOR;
}

} // namespace cycle_finder

} // namespace bn

#endif /* NAIVE_HPP_ */
