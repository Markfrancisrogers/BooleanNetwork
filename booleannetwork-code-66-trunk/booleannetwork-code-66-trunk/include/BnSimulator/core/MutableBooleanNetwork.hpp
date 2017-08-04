/*
 * MutableBooleanNetwork.hpp
 *
 *  Created on: Nov 18, 2010
 *      Author: stewie
 */

#ifndef MUTABLEBOOLEANNETWORK_HPP_
#define MUTABLEBOOLEANNETWORK_HPP_

#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <vector>

#include <boost/graph/graph_selectors.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "BooleanDynamics.hpp"
#include "BooleanFunction.hpp"

namespace bn {

class MutableBooleanNetwork : public BooleanDynamics {
private:
	class Updater;
	friend class Updater;

public:
	typedef std::vector<int> TruthTable;
	typedef boost::adjacency_list<boost::vecS, boost::vecS,
			boost::bidirectionalS, TruthTable> Network;

	using BooleanDynamics::update; // make update(size_t) visible

	/**
	 * Returns the number of nodes in the network.
	 * @return the number of nodes
	 */
	size_t size() const {
		return boost::num_vertices(net);
	}

	MutableBooleanNetwork* clone() const {
		return new MutableBooleanNetwork(*this);
	}

	Network& topology() {
		return net;
	}

	const Network& topology() const {
		return net;
	}

	/**
	 * Sets the state of this network.
	 * @param s the new state
	 */
	void setState(const State& s) {
		state = s;
		assert(state.size() == size());
	}

	/**
	 * Returns a reference to the current state of this network.
	 * @return the state of this network
	 */
	const State& getState() const {
		return state;
	}

	void update();

	BooleanFunction getFunction(const size_t i) const;

	std::vector<BooleanFunction> getFunctions() const;

	BooleanDynamics* simulate() {
		return new Updater(*this);
	}

	MutableBooleanNetwork& operator=(MutableBooleanNetwork other) {
		swap(other);
		return *this;
	}

	void swap(MutableBooleanNetwork& other) {
		using std::swap;
		swap(state, other.state);
		swap(net, other.net);
	}

	void update(State& s);

	static MutableBooleanNetwork makeNetwork(const char topologyFilename[],
			const char functionFilename[]);

	friend std::ostream& operator<<(std::ostream& out,
			const MutableBooleanNetwork& rbn);

protected:
	/**
	 * The current state of the netwotk.
	 */
	State state;

	Network net;

private:
	class Updater : public BooleanDynamics {
	public:
		using BooleanDynamics::update;

		Updater(MutableBooleanNetwork& bn) :
			bn(&bn) {
		}

		Updater* clone() const {
			return new Updater(*this);
		}

		const State& getState() const {
			return state;
		}

		void update();

		void setState(const State& s) {
			state = s;
		}

		void update(State& s);

		size_t size() const {
			return bn->size();
		}

		BooleanFunction getFunction(const size_t i) const {
			return bn->getFunction(i);
		}

	private:
		MutableBooleanNetwork* bn;

		State state;
	};
};

void swap(MutableBooleanNetwork& a, MutableBooleanNetwork& b);

} // namespace bn

#endif /* MUTABLEBOOLEANNETWORK_HPP_ */
