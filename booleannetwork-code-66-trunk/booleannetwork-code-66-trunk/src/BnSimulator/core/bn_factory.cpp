/*
 * factory.cpp
 *
 *  Created on: Dec 21, 2010
 *      Author: stewie
 */

#include <BnSimulator/core/bn_factory.hpp>

using namespace std;

namespace bn {

MutableBooleanNetwork make_network(const vector<vector<size_t> >& topology,
		const vector<vector<int> >& functions) {
	MutableBooleanNetwork res;
	for (vector<vector<int> >::const_iterator it = functions.begin(), end =
			functions.end(); it != end; ++it) // add vertices and functions
		add_vertex(MutableBooleanNetwork::TruthTable(it->begin(), it->end()),
				res.topology());
	for (size_t ui = 0; ui < topology.size(); ++ui) {
		MutableBooleanNetwork::Network::vertex_descriptor u = vertex(ui,
				res.topology());
		for (vector<size_t>::const_iterator vi = topology[ui].begin(), vend =
				topology[ui].end(); vi != vend; ++vi) { // add edges
			add_edge(vertex(*vi, res.topology()), u, res.topology());
		}
	}
	res.setState(State(res.size()));
	return res;
}

} // namespace bn
