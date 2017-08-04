/*
 * network_state.hpp
 *
 *  Created on: Apr 25, 2010
 *      Author: stewie
 */

#ifndef NETWORK_STATE_HPP_
#define NETWORK_STATE_HPP_

#include <cstddef>

#include <boost/function_output_iterator.hpp>
#include <boost/dynamic_bitset.hpp>

namespace bn {

/**
 * Type that represents the state of a network.
 */
typedef boost::dynamic_bitset<> State;

class BitsetHasher {
private:
	template<class BlockType> class BlockHasher {
	private:
		BlockType& acc;

	public:

		BlockHasher(BlockType& acc) :
			acc(acc) {
		}

		void operator()(const BlockType b) const {
			acc ^= b;
		}
	};

public:

	template<class BlockType> std::size_t operator()(
			const boost::dynamic_bitset<BlockType>& b) const {
		BlockType h = 0;
		boost::to_block_range(
				b,
				boost::make_function_output_iterator(BlockHasher<BlockType> (h)));
		return h;
	}
};

static const BitsetHasher bitset_hash = BitsetHasher();

} // namespace bn

#endif /* NETWORK_STATE_HPP_ */
