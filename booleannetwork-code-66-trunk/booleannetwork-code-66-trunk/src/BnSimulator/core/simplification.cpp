/*
 * simplification.cpp
 *
 *  Created on: Nov 22, 2010
 *      Author: stewie
 */

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <deque>
#include <set>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/algorithm/find.hpp>

#include <BnSimulator/core/MutableBooleanNetwork.hpp>
#include <BnSimulator/core/simplification.hpp>

using namespace std;
using namespace boost;

namespace bn {

typedef adjacency_list<listS, listS, bidirectionalS, BooleanFunction> TempGraph;
typedef TempGraph::vertex_descriptor vertex;

void simplify_node(const vertex v, TempGraph& g, set<vertex>& q, set<vertex>& toRemove) {
	if (toRemove.count(v)) {
		//cerr << "Vertex " << v << " should be removed" << endl;
		return;
	}
	//cerr << "Examining node " << v << " [" << g[v] << "]" << endl;
	if (out_degree(v, g) == 0) {
		//cerr << "has no outputs" << endl;
		TempGraph::inv_adjacency_iterator vi, vend;
		for (tie(vi, vend) = inv_adjacent_vertices(v, g); vi != vend; ++vi)
			q.insert(*vi);
		clear_vertex(v, g);
		//remove_vertex(v, g);
		toRemove.insert(v);
	} else {
		BooleanFunction& f = g[v];
		pair<bool, bool> constant = f.isConstant();
		if (constant.first) {
			//cerr << "is constantly " << constant.second << endl;
			TempGraph::inv_adjacency_iterator vi, vend;
			for (tie(vi, vend) = inv_adjacent_vertices(v, g); vi != vend; ++vi)
				if (*vi != v) // no self-edges
					q.insert(*vi);
			TempGraph::adjacency_iterator ai, aend;
			for (tie(ai, aend) = adjacent_vertices(v, g); ai != aend; ++ai) {
				if (*ai != v) { // no self-edges
					/*size_t xi = 0;
					for (tie(vi, vend) = inv_adjacent_vertices(*ai, g); vi
							!= vend; ++vi, ++xi) {
						if (*vi == v)
							break;
					}*/
					const size_t xi = find(inv_adjacent_vertices(*ai, g) | adaptors::indexed(0), v).index();
					g[*ai] = g[*ai].clamped(xi, constant.second);
					q.insert(*ai);
				}
			}
			clear_vertex(v, g);
			//remove_vertex(v, g);
			toRemove.insert(v);
		} else {
			size_t xi = 0;
			TempGraph::in_edge_iterator ei, eend;
			// inputs are ordered from LSB to MSB
			for (tie(ei, eend) = in_edges(v, g); ei != eend;){
				if (g[v].isInfluent(xi)) {
					++ei;
					++xi;
				} else {
					//cerr << "Variable " << xi << " for " << g[v] << " is not influent...";
					g[v] = g[v].clamped(xi, true); // true or false doesn't matter
					//cerr << g[v] << endl;
					TempGraph::in_edge_iterator temp = ei++; // avoid iterator invalidation
					q.insert(opposite(*temp, v, g)); // will later check if parent has out degree > 0
					remove_edge(*temp, g);
				}
			}
		}
	}
}

struct MakeFunction {
	const MutableBooleanNetwork::Network& from;
	TempGraph& to;
	MakeFunction(const MutableBooleanNetwork::Network& from, TempGraph& to) :
		from(from), to(to) {
	}

	void operator()(MutableBooleanNetwork::Network::vertex_descriptor u,
			TempGraph::vertex_descriptor v) const {
		to[v] = BooleanFunction(from[u]);
	}
};

struct GetTruthTable {
	const TempGraph& from;
	MutableBooleanNetwork::Network& to;
	GetTruthTable(const TempGraph& from, MutableBooleanNetwork::Network& to) :
		from(from), to(to) {
	}

	void operator()(TempGraph::vertex_descriptor u,
			MutableBooleanNetwork::Network::vertex_descriptor v) const {
		to[v] = from[u].truthTable();
	}
};

MutableBooleanNetwork::Network simplify(const MutableBooleanNetwork& net) {
	TempGraph g;
	copy_graph(net.topology(), g, vertex_copy(MakeFunction(net.topology(), g)));

	TempGraph::vertex_iterator vi, vend;
	set<vertex> q, toRemove;
	for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
		simplify_node(*vi, g, q, toRemove);
	}
	while (!q.empty()) {
		set<vertex>::const_iterator it = q.begin();
		//vertex v = *q.begin();
		simplify_node(*it, g, q, toRemove);
		q.erase(it);
	}
	for(set<vertex>::const_iterator it = toRemove.begin(); it != toRemove.end(); ++it)
		remove_vertex(*it, g);

	map<vertex, size_t> vertexMap;
	size_t id = 0;
	for (tie(vi, vend) = vertices(g); vi != vend; ++vi, ++id) {
		vertexMap[*vi] = id;
	}
	MutableBooleanNetwork::Network topology;
	copy_graph(
			g,
			topology,
			vertex_copy(GetTruthTable(g, topology)). vertex_index_map(
					make_assoc_property_map(vertexMap)));
	return topology;
}

} // namespace bn
