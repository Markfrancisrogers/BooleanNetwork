/**
 * @file sample_attractors.cpp
 *
 * This file contains example code explained in mini-tutorial @ref sample_attractors.
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include <boost/ptr_container/indirect_fun.hpp>

#include <BnSimulator/core/BooleanNetwork.hpp>
#include <BnSimulator/util/utility.hpp>
#include <BnSimulator/experiment/GardenerRunner.hpp>
#include <BnSimulator/lab/PrintAttractor.hpp>

namespace bn {

/**
 * @internal
 * Namespace for classes and function used in the examples.
 *
 * This namespace is used to avoid cluttering the global namespace.
 */
namespace example {

/**
 * Adapter class to print Attractor objects referred to by a pointer.
 *
 * It is implemented only for didactical purposes since it is more concise and
 * safe to use a combination of bn::PrintAttractor with
 * <a href="http://www.boost.org/libs/ptr_container/">Boost Pointer Container</a>
 * library functionalities.
 */
struct MyPrintAdapter {
	/**
	 * Adapted object.
	 */
	bn::PrintAttractor adapted;

	/**
	 * Constructor for the adaptor.
	 *
	 * Has the same signature as the constructor of bn::PrintAttractor::PrintAttractor(Rbn&,std::ostream&).
	 * @param net a Boolean Network
	 * @param out an output stream
	 */
	MyPrintAdapter(BooleanNetwork& net, std::ostream& out) :
		adapted(net, out) {
	}

	/**
	 * Method wrapper around bn::PrintAttractor::operator()().
	 * @param a an attractor to print
	 */
	void operator()(Attractor* a) const {
		adapted(*a);
	}
};

}

}

/**
 * Entry point for this program.
 *
 * It accepts the following required parameters in order:
 * @li number of nodes in the network
 * @li path to topology file
 * @li path to node function file
 * @li maximum number of steps before the simulator gives up on finding
 * an attractor
 * @li number of initial random states (non necessairly different)
 * @li seed for the random number generator
 */
int main(int argc, char **argv) {
	using namespace bn;
	// network and parameter initialization
	const uint maxSteps = std::atoi(argv[4]);
	const uint samples = std::atoi(argv[5]);
	BooleanNetwork net = BooleanNetwork::makeNetwork(std::atoi(argv[1]),
			argv[2], argv[3]);
	std::srand(std::atoi(argv[6]));
	// experiment initialization
	GardenerRunner gardener(maxSteps, samples);
	// run experiment
	GardenerRunner::Garden g = gardener.findGarden(net);
	// post-process results
	std::for_each(g.begin(), g.end(), example::MyPrintAdapter(net, std::cout));
	std::for_each(g.begin(), g.end(), boost::make_indirect_fun(PrintAttractor(
			net, std::cout)));
	// clean-up code
	util::purge(g); // free memory used by Attractor objects in g
	return EXIT_SUCCESS;
}
