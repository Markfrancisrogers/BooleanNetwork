/*
 * StateSpaceRunner.hpp
 *
 *  Created on: Jul 24, 2009
 *      Author: stewie
 */

#ifndef STATESPACERUNNER_HPP_
#define STATESPACERUNNER_HPP_

#define BOOST_NO_HASH 1

#include <cstddef>
#include <vector>
#include <map>
#include <functional> // for std::less
#include <boost/ptr_container/indirect_fun.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../util/utility.hpp"
#include "../core/Attractor.hpp"

namespace bn {

class BooleanDynamics;

/**
 * @ingroup runners
 *
 * This runner builds a subgraph of the attractor reachability graph.
 *
 * The attractor reachability graph of a boolean network is a directed graph
 * whose vertices are attractors and whose edges \f$(A, B)\f$ are labeled with a
 * transition probability map \f$\langle h, p\rangle\f$ estimated by a crude
 * Monte-Carlo sampling. Given an edge \f$(A, B)\f$, a transition probability map
 * specifies the probability @e p for the network to eventually reach attractor
 * @e B starting from a state in attractor @e A that undergoes a random perturbation
 * of magnitude @e h. Notice that these probabilities need not be symmetric so,
 * given a perturbation magnitude, the probability to reach @e B from a random
 * state in @e A might be different from the probability to reach @e A from a
 * random state in @e B. It is something very alike a transition graph for a
 * first order Markov model.
 *
 * For each attractor, a transition probability map is build by a number @e probes
 * of simulation of the net for each state in the source attractor and for each
 * perturbation magnitude \f$i = 1 \ldots h - 1\f$.
 *
 * A PerturbativeRunner is used to compute the forward-star of the reachability
 * graph.
 *
 * A reachability graph is represented by a Boost graph object. See <a href=
 * "http://www.boost.org/libs/graph/">The Boost Graph Library</a> for further
 * details.
 */
class StateSpaceRunner {
public:
	/**
	 * The label type for the vertices.
	 */
	typedef Attractor* NodeLabel;
	/**
	 * The label type for the edges.
	 */
	typedef std::map<size_t, double> TransitionLabel;
	/**
	 * The graph type.
	 *
	 * Instances of this type represent bidirectional graphs which do not allow
	 * for parallel edges.
	 *
	 * @note
	 * This type does not own the ExplicitAttractor objects associated to each
	 * vertex.
	 */
	typedef boost::adjacency_list<boost::setS, boost::vecS, //boost::listS,
			boost::bidirectionalS, NodeLabel, TransitionLabel> Graph;
	//			boost::bidirectionalS, boost::shared_ptr<ExplicitAttractor>, TransitionLabel> Graph;

	/**
	 * Initialises this runner with a maximum number of steps per sample and the
	 * number of samples.
	 * @param maxSteps maximum number of simulation steps for each sample
	 * @param probes number of samples to take
	 */
	StateSpaceRunner(const size_t maxSteps, const size_t probes) :
		maxSteps(maxSteps), probes(probes) {
	}

	Graph
			stateSpace(BooleanDynamics&, const Attractor&, const size_t h = 1) const;

	/**
	 * Computes an attractor reachability graph starting by a range of
	 * ExplicitAttractor objects.
	 *
	 * Behaviour is the same as the repeated invokation of method StateSpaceRunner::stateSpace(BooleanNetwork&,const ExplicitAttractor&) const
	 * once for each attractor in the range.
	 *
	 * Notice that the graph returned might not be connected.
	 * @param net a network to simulate
	 * @param first initial position in the range
	 * @param last final position in the range
	 * @param h upper bound of perturbation magnitude range
	 * @return a reachability graph object
	 */
	template<class Iterator> Graph stateSpace(BooleanDynamics& net,
			Iterator first, const Iterator last, const size_t h = 1) const {
		Graph g;
		VertexMap m;
		for (; first != last; ++first) {
			vertex_descriptor v = addAttractorVertex(new Attractor(*first), g,
					m);
			if (out_degree(v, g) == 0) { // not expanded vertex (avoid cycles)
				setupNeighbors(net, v, h, g, m); // add neighboring vertices
				recursiveStateSpace(net,
						util::make_vector(adjacent_vertices(v, g)), h, g, m); // start exploration
			}
		}
		return g;
	}

private:
	/**
	 * Vertex descriptor type.
	 */
	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	/**
	 * Map from attractors to vertex_descriptor's.
	 */
	typedef std::map<NodeLabel, vertex_descriptor, boost::indirect_fun<
			std::less<Attractor> > > VertexMap;
	/**
	 * Type of the iterators on neighboring vertices.
	 */
	typedef boost::graph_traits<Graph>::adjacency_iterator adjacency_iterator;

	/**
	 * Maximum number of simulation steps for each sample.
	 */
	const size_t maxSteps;
	/**
	 * Number of samples to take.
	 */
	const size_t probes;

	vertex_descriptor
	addAttractorVertex(const NodeLabel n, Graph& g, VertexMap& m) const;

	void setupNeighbors(BooleanDynamics&, const vertex_descriptor,
			const size_t h, Graph&, VertexMap&) const;

	void recursiveStateSpace(BooleanDynamics&,
			const std::vector<vertex_descriptor>&, const size_t h, Graph&,
			VertexMap&) const;
};

}

#endif /* STATESPACERUNNER_HPP_ */
