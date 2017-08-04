/*
 * reachability_graph.hpp
 *
 *  Created on: May 12, 2011
 *      Author: stewie
 */

#ifndef REACHABILITY_GRAPH_HPP_
#define REACHABILITY_GRAPH_HPP_

#include <map>
#include <functional>

#include <boost/concept_check.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/ptr_container/indirect_fun.hpp>

#include "../core/Attractor.hpp"
#include "../core/BooleanDynamics.hpp"
#include "perturb_attractor.hpp"

namespace bn {

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS,
		Attractor, double> Graph;

typedef std::map<Attractor, Graph::vertex_descriptor> VertexMap;

template<class AttractorRange, class CycleFinder> Graph reachability_graph(
		const AttractorRange& r, CycleFinder f) {
	BOOST_CONCEPT_ASSERT((boost::ForwardRangeConcept<AttractorRange>));
	Graph g;
	VertexMap vmap;
	for (typename boost::range_iterator<AttractorRange>::type it =
			boost::begin(r), end = boost::end(r); it != end; ++it) {
		Graph::vertex_descriptor v = boost::add_vertex(*it, g);
		vmap[*it] = v;
	}
	Graph::vertex_iterator vit, vend;
	for (boost::tie(vit, vend) = boost::vertices(g); vit != vend; ++vit) {
		std::map<Attractor, double> m = perturb_attractor(g[*vit], f);
		for (std::map<Attractor, double>::const_iterator it = m.begin(), end =
				m.end(); it != end; ++it) {
			if (vmap.count(it->first)) {
				boost::add_edge(vmap[g[*vit]], vmap[it->first], it->second, g);
			}
		}
	}
	return g;
}

} // namespace bn

#endif /* REACHABILITY_GRAPH_HPP_ */
