/*
 * ImmutableBooleanNetwork.hpp
 *
 *  Created on: Nov 17, 2010
 *      Author: stewie
 */

#ifndef IMMUTABLEBOOLEANNETWORK_HPP_
#define IMMUTABLEBOOLEANNETWORK_HPP_

#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <vector>

#include <boost/graph/graph_selectors.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>

#include "BooleanDynamics.hpp"
#include "BooleanFunction.hpp"

namespace bn {

class ImmutableBooleanNetwork : public BooleanDynamics {
public:
	using BooleanDynamics::update; // make update(size_t) visible

	/**
	 * Returns the number of nodes in the network.
	 * @return the number of nodes
	 */
	size_t size() const {
		return boost::num_vertices(net);
	}

	/**
	 * Sets the state of this network.
	 * @param s the new state
	 */
	void setState(const State& s) {
		assert(size() == s.size());
		state = s;
	}

	ImmutableBooleanNetwork* clone() const {
		return new ImmutableBooleanNetwork(*this);
	}

	/**
	 * Returns a reference to the current state of this network.
	 * @return the state of this network
	 */
	const State& getState() const {
		return state;
	}

	void update();

	State operator()(const State& s);

	void update(State& s);

	BooleanFunction getFunction(const size_t i) const;

	std::vector<BooleanFunction> getFunctions() const;

	static ImmutableBooleanNetwork makeNetwork(const char topologyFilename[],
			const char functionFilename[]);

	friend std::ostream& operator<<(std::ostream& out,
			const ImmutableBooleanNetwork& rbn);

protected:
	typedef std::vector<int> TruthTable;
	typedef boost::compressed_sparse_row_graph<boost::directedS, TruthTable>
			Network;
	/**
	 * The current state of the netwotk.
	 */
	State state;

	Network net;

private:
	/**
	 * This class disallows assignment.
	 */
	ImmutableBooleanNetwork& operator=(const ImmutableBooleanNetwork&);
};

}

#endif /* IMMUTABLEBOOLEANNETWORK_HPP_ */
