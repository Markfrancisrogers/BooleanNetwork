/*
 * ControllableBooleanNetwork.cpp
 *
 *  Created on: Nov 22, 2010
 *      Author: stewie
 */

#include <vector>
#include <algorithm>

#include <boost/graph/graph_utility.hpp>

#include <BnSimulator/core/ControllableBooleanNetwork.hpp>

using namespace std;
using namespace boost;

namespace bn {

struct AlwaysTrue {
	template<class T> bool operator()(const T&) const {
		return true;
	}
};

ControllableBooleanNetwork::ControllableBooleanNetwork(
		const MutableBooleanNetwork& bn, const size_t inputs,
		const size_t outputs) :
	MutableBooleanNetwork(bn), inputs(inputs), outputs(outputs) {
	using std::swap;
	assert(inputs < outputs);
	assert(getInput().none() && getOutput().none());
	for (size_t i = 0; i < inputs; ++i) {
		remove_in_edge_if(i, AlwaysTrue(), net);
		TruthTable empty;
		swap(net[i], empty);
	}
}

ControllableBooleanNetwork& ControllableBooleanNetwork::operator=(
		ControllableBooleanNetwork other) {
	swap(other);
	return *this;
}

void ControllableBooleanNetwork::setState(const State& s) {
	assert(size() == s.size());
	for (size_t i = inputs; i < size(); ++i)
		state[i] = s[i];
}

void ControllableBooleanNetwork::update() {
	using std::swap;
	State next(state);
	Network::vertex_iterator vi, vend;
	tie(vi, vend) = vertices(net);
	for (vi += inputs; vi != vend; ++vi) {
		size_t index = 0; // index computation is the most expensive operation
		size_t j = 0;
		Network::inv_adjacency_iterator it, end;
		for (tie(it, end) = inv_adjacent_vertices(*vi, net); it != end; ++it, ++j) {
			index += (1 << j) * state[*it];
		}
		next[*vi] = net[*vi][index]; // vertex descriptors are integers
	}
	swap(state, next);
}

void ControllableBooleanNetwork::update(State& s) {
	using std::swap;
	State next(s);
	Network::vertex_iterator vi, vend;
	tie(vi, vend) = vertices(net);
	for (vi += inputs; vi != vend; ++vi) {
		size_t index = 0; // index computation is the most expensive operation
		size_t j = 0;
		Network::inv_adjacency_iterator it, end;
		for (tie(it, end) = inv_adjacent_vertices(*vi, net); it != end; ++it, ++j) {
			index += (1 << j) * s[*it];
		}
		next[*vi] = net[*vi][index]; // vertex descriptors are integers
	}
	swap(s, next);
}

State ControllableBooleanNetwork::operator()(const State& s) {
	using std::swap;
	State next(s);
	Network::vertex_iterator vi, vend;
	tie(vi, vend) = vertices(net);
	for (vi += inputs; vi != vend; ++vi) {
		size_t index = 0; // index computation is the most expensive operation
		size_t j = 0;
		Network::inv_adjacency_iterator it, end;
		for (tie(it, end) = inv_adjacent_vertices(*vi, net); it != end; ++it, ++j) {
			index += (1 << j) * s[*it];
		}
		next[*vi] = net[*vi][index]; // vertex descriptors are integers
	}
	return next;
}

State ControllableBooleanNetwork::getInput() const {
	return sliceState(0, inputs);
}

void ControllableBooleanNetwork::setInput(const State& s) {
	assert(s.size() == inputs);
	for (size_t i = 0; i < inputs; ++i)
		state[i] = s[i];
}

State ControllableBooleanNetwork::getHidden() const {
	return sliceState(inputs + outputs, size());
}

void ControllableBooleanNetwork::setHidden(const State& s) {
	assert(s.size() == size() - (outputs + inputs));
	for (size_t i = 0; i < s.size(); ++i)
		state[i + inputs + outputs] = s[i];
}

State ControllableBooleanNetwork::getOutput() const {
	return sliceState(inputs, inputs + outputs);
}

State ControllableBooleanNetwork::sliceState(const size_t from, const size_t to) const {
	assert(from <= to);
	State res(to - from);
	for (size_t i = from, j = 0; i < to; ++i, ++j) {
		res[j] = state[i];
	}
	return res;
}

void ControllableBooleanNetwork::swap(ControllableBooleanNetwork& other) {
	using std::swap;
	MutableBooleanNetwork::swap(other);
	swap(inputs, other.inputs);
	swap(outputs, other.outputs);
}

ControllableBooleanNetwork::Proxy::operator bool() const {
	return s[i];
}

ControllableBooleanNetwork::Proxy& ControllableBooleanNetwork::Proxy::operator=(
		const bool b) {
	s[i] = b;
	return *this;
}

void swap(ControllableBooleanNetwork& a, ControllableBooleanNetwork& b) {
	a.swap(b);
}

} // namespace bn
