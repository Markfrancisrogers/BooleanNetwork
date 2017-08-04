/*
 * BooleanDynamics.hpp
 *
 *  Created on: Nov 16, 2010
 *      Author: stewie
 */

#ifndef BOOLEANDYNAMICS_HPP_
#define BOOLEANDYNAMICS_HPP_

#include <cstddef>

#include "BooleanFunction.hpp"
#include "network_state.hpp"

namespace bn {

class BooleanDynamics {
public:
	virtual ~BooleanDynamics() {
	}

	virtual BooleanDynamics* clone() const = 0;

	/**
	 * @deprecated
	 */
	virtual const State& getState() const = 0;

	/**
	 * @deprecated
	 */
	virtual void setState(const State&) = 0;

	/**
	 * @deprecated
	 */
	virtual void update() = 0;

	/**
	 * @deprecated
	 */
	virtual void update(const size_t n) {
		for (std::size_t i = 0; i < n; ++i)
			update();
	}

	virtual std::size_t size() const {
		return getState().size();
	}

	virtual State operator()(const State& s) {
		State result(s);
		update(result);
		return result;
	}

	virtual void update(State& s) = 0;

	virtual BooleanFunction getFunction(const std::size_t i) const = 0;
};

} // namespace bn

#endif /* BOOLEANDYNAMICS_HPP_ */
