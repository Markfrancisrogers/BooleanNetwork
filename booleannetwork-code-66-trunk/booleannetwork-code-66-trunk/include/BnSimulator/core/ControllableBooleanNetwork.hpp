/*
 * ControllableBooleanNetwork.hpp
 *
 *  Created on: Nov 22, 2010
 *      Author: stewie
 */

#ifndef CONTROLLABLEBOOLEANNETWORK_HPP_
#define CONTROLLABLEBOOLEANNETWORK_HPP_

#include <cassert>
#include <cstddef>

#include <boost/graph/graph_selectors.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "network_state.hpp"
#include "BooleanDynamics.hpp"
#include "MutableBooleanNetwork.hpp"

namespace bn {

class ControllableBooleanNetwork : public MutableBooleanNetwork {
private:
	class Proxy {
	public:
		Proxy(State& s, std::size_t i) :
			s(s), i(i) {
		}

		operator bool() const;

		Proxy& operator=(const bool);

	private:
		State& s;

		std::size_t i;
	};
	friend class Proxy;

public:
	using BooleanDynamics::update; // make update(size_t) visible

	ControllableBooleanNetwork(const MutableBooleanNetwork& bn,
			const std::size_t inputs, const std::size_t outputs);

	void setState(const State& s);

	State getInput() const;

	void setInput(const State& s);

	Proxy operator[](const std::size_t i) {
		assert(i < inputs);
		return Proxy(state, i);
	}

	State getHidden() const;

	void setHidden(const State&);

	State getOutput() const;

	void update();

	void swap(ControllableBooleanNetwork& other);

	State operator()(const State& s);

	void update(State& s);

	ControllableBooleanNetwork& operator=(ControllableBooleanNetwork other);

protected:
	std::size_t inputs;

	std::size_t outputs;

	State sliceState(const std::size_t from, const std::size_t to) const;
};

void swap(ControllableBooleanNetwork& a, ControllableBooleanNetwork& b);

} // namespace bn

#endif /* CONTROLLABLEBOOLEANNETWORK_HPP_ */
