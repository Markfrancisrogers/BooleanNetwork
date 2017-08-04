/*
 * PrintAttractor.hpp
 *
 *  Created on: Aug 12, 2009
 *      Author: stewie
 */

#ifndef PRINTATTRACTOR_HPP_
#define PRINTATTRACTOR_HPP_

#include <ostream>

#include "../core/BooleanNetwork_fwd.hpp"
#include "../core/Attractor.hpp"

namespace bn {

/**
 * @ingroup lab
 *
 * This class prints to a stream all the states of an Attractor object.
 *
 * Instances of this class are aimed to be used as function objects, in a standard
 * library algorithm for instance.
 *
 * @see ExpandAttractor for a similar concept.
 */
struct PrintAttractor {
private:
	/**
	 * The network that generated the attractors.
	 */
	BooleanNetwork& net;
	/**
	 * Stream to print to.
	 */
	std::ostream& out;

public:
	/**
	 * Result type of this function object.
	 */
	typedef void result_type;

	/**
	 * Initializes this object with the network that generated the attractors
	 * to be printed.
	 */
	PrintAttractor(BooleanNetwork& net, std::ostream& out) :
		net(net), out(out) {
	}

	void operator()(const Attractor& a) const;
};

}

#endif /* PRINTATTRACTOR_HPP_ */
