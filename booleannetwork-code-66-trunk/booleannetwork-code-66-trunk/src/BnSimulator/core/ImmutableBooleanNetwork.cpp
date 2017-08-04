/*
 * ImmutableBooleanNetwork.cpp
 *
 *  Created on: Nov 17, 2010
 *      Author: stewie
 */

#include <utility>
#include <algorithm>
#include <fstream>

#include <boost/tuple/tuple.hpp>

#include <BnSimulator/util/printing.hpp>
#include <BnSimulator/util/JaggedArray.hpp>
#include <BnSimulator/core/ImmutableBooleanNetwork.hpp>

using namespace std;
using namespace boost;

namespace bn {

/**
 * Triggers a simulation step.
 *
 * @note
 * From an implementation point of view, this is @e the most expensive piece
 * of code in the whole library.
 */
void ImmutableBooleanNetwork::update() {
	using std::swap;
	State next(size());
	Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(net); vi != vend; ++vi) {
		size_t index = 0; // index computation is the most expensive operation
		size_t j = 0;
		Network::out_edge_iterator it, end;
		for (tie(it, end) = out_edges(*vi, net); it != end; ++it, ++j) {
			index += (1 << j) * state[target(*it, net)];
		}
		next[*vi] = net[*vi][index];
	}
	swap(state, next);
}

State ImmutableBooleanNetwork::operator()(const State& s) {
	using std::swap;
	State next(size());
	Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(net); vi != vend; ++vi) {
		size_t index = 0; // index computation is the most expensive operation
		size_t j = 0;
		Network::out_edge_iterator it, end;
		for (tie(it, end) = out_edges(*vi, net); it != end; ++it, ++j) {
			index += (1 << j) * s[target(*it, net)];
		}
		next[*vi] = net[*vi][index];
	}
	return next;
}

void ImmutableBooleanNetwork::update(State& s) {
	using std::swap;
	State next(size());
	Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(net); vi != vend; ++vi) {
		size_t index = 0; // index computation is the most expensive operation
		size_t j = 0;
		Network::out_edge_iterator it, end;
		for (tie(it, end) = out_edges(*vi, net); it != end; ++it, ++j) {
			index += (1 << j) * s[target(*it, net)];
		}
		next[*vi] = net[*vi][index];
	}
	swap(s, next);
}

BooleanFunction ImmutableBooleanNetwork::getFunction(const size_t i) const {
	assert(i < size());
	return BooleanFunction(net[vertex(i, net)]);
}

vector<BooleanFunction> ImmutableBooleanNetwork::getFunctions() const {
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
ImmutableBooleanNetwork ImmutableBooleanNetwork::makeNetwork(
		const char topologyFilename[], const char functionFilename[]) {
	ifstream topoFile(topologyFilename);
	util::JaggedArray<size_t> topo;
	topoFile >> topo;
	topoFile.close();
	ifstream funcFile(functionFilename);
	util::JaggedArray<bool> func;
	funcFile >> func;
	funcFile.close();
	typedef pair<size_t, size_t> E;
	vector<E> edges;
	for (size_t i = 0; i < topo.numRows(); ++i) { // add edges
		for (util::JaggedArray<size_t>::const_iterator j = topo.begin(i), end =
				topo.end(i); j != end; ++j) {
			edges.push_back(E(i, *j));
		}
	}
	ImmutableBooleanNetwork::Network g(boost::edges_are_unsorted,
			edges.begin(), edges.end(), topo.numRows());
	for (size_t i = 0; i < func.numRows(); ++i) {
		g[i] = TruthTable(func.begin(i), func.end(i));
	}
	ImmutableBooleanNetwork res;
	swap(res.net, g);
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
ostream& operator<<(ostream& out, const ImmutableBooleanNetwork& bn) {
	ImmutableBooleanNetwork::Network::vertex_iterator vi, vend;
	for (tie(vi, vend) = vertices(bn.net); vi != vend; ++vi) {
		out << *vi << ") ";
		util::print(out, adjacent_vertices(*vi, bn.net));
		out << " : " << util::dump(bn.net[*vi]) << '\n';
	}
	return out;
}

}
