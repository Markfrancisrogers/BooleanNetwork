/**
 * @file sample_boa_sizes.cpp
 *
 * This file contains example code explained in mini-tutorial
 * @ref sample_boa_sizes.
 */

#include <cstdlib>
#include <iostream>

#include <BnSimulator/core/BooleanNetwork.hpp>
#include <BnSimulator/experiment/BasinRunner.hpp>

using namespace bn;

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
int main(int argc, char* argv[]) {
	// network and parameter initialization
	const uint maxSteps = std::atoi(argv[4]);
	const uint probes = std::atoi(argv[5]);
	BooleanNetwork net = BooleanNetwork::makeNetwork(std::atoi(argv[1]),
			argv[2], argv[3]);
	std::srand(std::atoi(argv[6]));
	// initialize and run experiment
	BasinRunner::BasinSizes map = BasinRunner(maxSteps, probes).basinSizes(net);
	// post-process results
	for (BasinRunner::BasinSizes::const_iterator it = map.begin(); it
			!= map.end(); ++it) {
		std::cout << *it->first << " " << it->second << std::endl;
	}
	// clean-up code
	for (BasinRunner::BasinSizes::const_iterator it = map.begin(); it
			!= map.end(); ++it)
		delete it->first;
	return EXIT_SUCCESS;
}
