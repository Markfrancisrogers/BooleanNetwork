#include <cassert>
#include <cstdlib>
#include <iostream>

#include <boost/smart_ptr/scoped_ptr.hpp>

#include <BnSimulator/core/BooleanNetwork.hpp>
#include <BnSimulator/core/Trajectory.hpp>
#include <BnSimulator/core/Attractor.hpp>
#include <BnSimulator/experiment/TrajectoryRunner.hpp>

int main(int argc, char **argv) {
	using namespace bn;
	// network and parameter initialization
	const uint maxSteps = std::atoi(argv[4]);
	BooleanNetwork net = BooleanNetwork::makeNetwork(std::atoi(argv[1]),
			argv[2], argv[3]);
	std::srand(std::atoi(argv[5]));
	// experiment initialization
	TrajectoryRunner runner(maxSteps);
	// run experiment
	boost::scoped_ptr<const Trajectory> t(runner.findAttractor(net));
	// post-process results
	std::cout << *t << std::endl; // print the whole trajectory
	std::cout << "Cycle:\n" << t->printCycle(); // print only the cycle
	if (t->cycleLength() > 0) {
		const Attractor a(*t);
		std::cout << "Attractor representant:\n" << a << std::endl;
		std::cout << "Attractor length: " << a.getLength() << std::endl;
		boost::scoped_ptr<ExplicitAttractor> ax(new ExplicitAttractor(*t));
		std::cout << "Attractor states:\n" << *ax;
		t.reset(runner.findAttractor(net, a.getRepresentant()));
		assert(t->getTransient().empty());
		assert(t->getCycle().size() == a.getLength());
		std::cout << "Cycle:\n" << t->printCycle();
	}
	return EXIT_SUCCESS;
}
