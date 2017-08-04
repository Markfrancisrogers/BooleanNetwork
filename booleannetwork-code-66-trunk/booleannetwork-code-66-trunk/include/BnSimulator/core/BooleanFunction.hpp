/*
 * BooleanFunction.hpp
 *
 *  Created on: Jun 1, 2010
 *      Author: stewie
 */

#ifndef BOOLEANFUNCTION_HPP_
#define BOOLEANFUNCTION_HPP_

#include <cstddef>
#include <cmath>
#include <iosfwd>
#include <utility>
#include <vector>

#include <boost/concept_check.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/size.hpp>

#include "network_state.hpp"

namespace bn {

class BooleanFunction {
public:
	template<class RandomAccessRange> BooleanFunction(
			const RandomAccessRange& r) :
		def(boost::size(r)), arity(arityFromSize(def.size())) {
		BOOST_CONCEPT_ASSERT((boost::RandomAccessRangeConcept<RandomAccessRange>));
		typename boost::range_iterator<const RandomAccessRange>::type it =
				boost::begin(r);
		for (std::size_t i = 0; i < def.size(); ++i, ++it)
			def[i] = *it;
	}

	BooleanFunction(const bool val = false) :
		def(1, val), arity(0) {
	}

	explicit BooleanFunction(const State& s);

	std::size_t getArity() const {
		return arity;
	}

	std::size_t size() const {
		return def.size();
	}

	std::vector<int> truthTable() const;

	bool isInfluent(const std::size_t i) const;

	BooleanFunction clamped(const std::size_t i, const bool v) const;

	void clamp(const std::size_t i, const bool v);

	std::pair<bool, bool> isConstant() const {
		const bool allZeros = def.none();
		const bool allOnes = (~def).none();
		return std::make_pair(allZeros || allOnes, allOnes);
	}

	bool operator()(const State& x) const;

	bool operator==(const BooleanFunction& other) const {
		return def == other.def;
	}

	bool operator<(const BooleanFunction& other) const;

	friend std::ostream
	& operator<<(std::ostream& out, const BooleanFunction& f);

	friend std::size_t hash_value(const BooleanFunction&);

	friend void swap(BooleanFunction&, BooleanFunction&);

private:
	State def;
	std::size_t arity;

	std::pair<State, State> demultiplex(const std::size_t i) const;

	static std::size_t arityFromSize(const std::size_t n) {
		return static_cast<std::size_t> (log2(n));
	}
};

} // namespace bn

#endif /* BOOLEANFUNCTION_HPP_ */
