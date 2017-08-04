/*
 * hamming.hpp
 *
 *  Created on: Aug 28, 2009
 *      Author: stewie
 */

#ifndef HAMMING_HPP_
#define HAMMING_HPP_

#include <cstddef>
#include <functional>

#include "../core/network_state.hpp"
#include "../core/Attractor.hpp"
#include "../util/state_util.hpp"

namespace bn {

/**
 * @ingroup lab
 *
 * A function object to compute the Hamming distance between two bitsets.
 */
struct Hamming : std::binary_function<State, State, size_t> {

	/**
	 * Computes the Hamming distance between two bitsets of the same length.
	 * @param x the first boolean vector
	 * @param y the second boolean vector
	 * @return the Hamming distance
	 */
	result_type operator()(const State& x, const State& y) const {
		return util::hamming_distance(x, y);
	}
};

size_t minimum_hamming_distance(const Attractor& a, const Attractor& b);

size_t maximum_hamming_distance(const Attractor& a, const Attractor& b);

}

#endif /* HAMMING_HPP_ */
