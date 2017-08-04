/*
 * MutableBooleanNetwork.cpp
 *
 *  Created on: Nov 18, 2010
 *      Author: stewie
 */

#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <boost/tuple/tuple.hpp>

#include <BnSimulator/util/printing.hpp>
#include <BnSimulator/util/JaggedArray.hpp>
#include <BnSimulator/core/MutableBooleanNetwork.hpp>

using namespace std;
using namespace boost;

namespace bn {

void MutableBooleanNetwork::Updater::update() {
	using std::swap;
	State next(size());
	MutableBooleanNetwork::Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(bn->net); vi != vend; ++vi) {
		size_t index = 0;
		size_t j = 0;
		MutableBooleanNetwork::Network::inv_adjacency_iterator it, end;
		for (tie(it, end) = inv_adjacent_vertices(*vi, bn->net); it != end; ++it, ++j) {
			index += (1 << j) * state[*it];
		}
		next[*vi] = bn->net[*vi][index];
	}
	swap(state, next);
}

void MutableBooleanNetwork::Updater::update(State& s) {
	using std::swap;
	State next(size());
	MutableBooleanNetwork::Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(bn->net); vi != vend; ++vi) {
		size_t index = 0;
		size_t j = 0;
		MutableBooleanNetwork::Network::inv_adjacency_iterator it, end;
		for (tie(it, end) = inv_adjacent_vertices(*vi, bn->net); it != end; ++it, ++j) {
			index += (1 << j) * s[*it];
		}
		next[*vi] = bn->net[*vi][index];
	}
	swap(s, next);
}

/**
 * Triggers a simulation step.
 *
 * @note
 * From an implementation point of view, this is @e the most expensive piece
 * of code in the whole library.
 */
void MutableBooleanNetwork::update() {
	using std::swap;
	State next(size());
	Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(net); vi != vend; ++vi) {
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

void MutableBooleanNetwork::update(State& s) {
	using std::swap;
	State next(size());
	Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(net); vi != vend; ++vi) {
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

BooleanFunction MutableBooleanNetwork::getFunction(const size_t i) const {
	assert(i < size());
	return BooleanFunction(net[vertex(i, net)]);
}

vector<BooleanFunction> MutableBooleanNetwork::getFunctions() const {
	vector<BooleanFunction> res;
	for (size_t i = 0; i < size(); ++i) {
		res.push_back(getFunction(i));
	}
	return res;
}

/**
 * Convenient function to initialize a boolean network from two files containing
 * a textual description of its topology and node functions.
 *
 * Topology file must contain, for each node, a whitespace separated list of
 * node indices which describe the inputs of a node.
 * Thus, for the <em>i</em>-th node the <em>i</em>-th row reads:\n
 * \<first input\> \<second input\> ... \<<em>k</em>-th input\>
 *
 * Function file must contain, for each node, a whitespace separated list of
 * '0' or '1' characters which define the ouputs of the boolean function for that
 * node. Thus, if the <em>i</em>-th node has associated a function @e f, in the
 * <em>i</em>-th row we read:\n
 * \f$\underbrace{f(\overbrace{00 \ldots 00}^k) \quad (\overbrace{00 \ldots 01}^k)
 * \quad \ldots \quad f(\overbrace{11 \ldots 11}^k)}_{2^k}\f$\n
 * which totals to \f$2^k\f$ elements if <em>i</em>-th node has @e k inputs.
 *
 * Input order is specified by topology file with the @e leftmost integer
 * indicating the <em>least significant bit</em>.
 *
 * It is advisable to use this method instead of the constuctor.
 * @param n number of nodes in the network
 * @param topologyFilename file name of topology description
 * @param functionFilename file name of function definitions
 * @return a new boolean network
 *
 * @todo
 * Parameter @a n is actually useless since it can be inferred by input files.
 * It might be kept as a double check.
 */
MutableBooleanNetwork MutableBooleanNetwork::makeNetwork(
		const char topologyFilename[], const char functionFilename[]) {
	MutableBooleanNetwork res;
	ifstream topoFile(topologyFilename);
	util::JaggedArray<size_t> topo;
	topoFile >> topo;
	topoFile.close();
	ifstream funcFile(functionFilename);
	util::JaggedArray<bool> func;
	funcFile >> func;
	funcFile.close();
	for (size_t i = 0; i < func.numRows(); ++i) // add vertices and functions
		add_vertex(TruthTable(func.begin(i), func.end(i)), res.net);
	for (size_t i = 0; i < topo.numRows(); ++i) { // add edges
		Network::vertex_descriptor v = vertex(i, res.net);
		for (util::JaggedArray<size_t>::const_iterator j = topo.begin(i), end =
				topo.end(i); j != end; ++j) { //
			add_edge(vertex(*j, res.net), v, res.net);
		}
	}
	res.state = State(res.size());
	return res;
}

/**
 * Prints a network to a stream.
 *
 * It prints only the number of nodes. It is not very informative indeed.
 * @param out an output stream
 * @param rbn a random boolean network
 * @return the stream passed as first argument
 */
ostream& operator<<(ostream& out, const MutableBooleanNetwork& bn) {
	MutableBooleanNetwork::Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(bn.net); vi != vend; ++vi) {
		out << *vi << ") ";
		util::print(out, inv_adjacent_vertices(*vi, bn.net));
		out << " : " << util::dump(bn.net[*vi]) << '\n';
	}
	return out;
}

void swap(MutableBooleanNetwork& a, MutableBooleanNetwork& b) {
	a.swap(b);
}

}
