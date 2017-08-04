/*
 * Trajectory.hpp
 *
 *  Created on: Jul 15, 2009
 *      Author: stewie
 */

#ifndef TRAJECTORY_HPP_
#define TRAJECTORY_HPP_

#include <cstddef>
#include <vector>
#include <ostream>
#include <algorithm>

#include <boost/noncopyable.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "../util/utility.hpp"
#include "../util/printing.hpp"
#include "network_state.hpp"

namespace bn {

/**
 * A trajectory is the ordered sequence of the states that a boolean network
 * visits in its evolution from an initial state.
 *
 * A trajectory object is actually an aggregation of two ordered sequences of
 * states:
 * - transient
 * - cycle
 *
 * both of which can possibly be empty.
 */
class Trajectory : public boost::noncopyable {
private:
	/**
	 * Type of container.
	 */
	typedef std::vector<State> StateContainer;

	/**
	 * This class help to print a sequence of states to a stream.
	 *
	 * This immutable class is amied to be used "inline" like a stream modifier
	 * (like std::endl). For example you can write:
	 * @code
	 * vector<State> v = ...
	 * cout << "States sequence:" << endl <<
	 * 	PrintFromTo(v.begin(), v.end(), "\n") << endl;
	 * @endcode
	 *
	 * Printing is done by invoking operator<< on the objects.
	 */
	/*class PrintFromTo {
	private:

		const StateContainer& states;

		const char* const sep;

	public:

		PrintFromTo(const StateContainer& states, const char sep[]) :
			states(states), sep(sep) {
		}


		friend std::ostream& operator<<(std::ostream& out,
				const PrintFromTo& printer) {
			util::print(out, printer.states, sep);
			return out;
		}
	};*/

	/**
	 * Container for transient states.
	 */
	StateContainer trans;
	/**
	 * A possibly empty container of cycle states.
	 */
	StateContainer cycle;

public:

	//	Trajectory(const std::vector<State*>& trajectory,
	//			const std::vector<State*>::const_iterator cycleStart) :
	//		trans(boost::make_indirect_iterator(trajectory.begin()),
	//				boost::make_indirect_iterator(cycleStart)), cycle(
	//				boost::make_indirect_iterator(cycleStart),
	//				boost::make_indirect_iterator(trajectory.end())) {
	//	}

	/**
	 * Constructs a Trajectory from a range of states.
	 *
	 * This constructor makes no effort to ensure that the supplied state
	 * range actually contains a cycle, that the iterator does actually
	 * points to the beginning of a cycle or that the cycle contains ducplicate
	 * elements.
	 *
	 * If the input sequence does not contain a cycle, @a cycleStart should point
	 * to the one-past-end element of the range (for instance the iterator
	 * returned by calling vector::end()).
	 * @param r an ordered sequence of states
	 * @param cycleStart an iterator pointing to the beginning of the cycle in
	 * 	the sequence
	 */
	template<class SinglePassRange> Trajectory(const SinglePassRange& r,
			typename boost::range_iterator<SinglePassRange>::type cycleStart) :
		trans(boost::begin(r), cycleStart), cycle(cycleStart, boost::end(r)) {
	}

	/**
	 * Implementation of swap function.
	 *
	 * It simply swaps internal state containers of the two Trajectory objects.
	 * @param t the other trajectory
	 */
	void swap(Trajectory& t) {
		std::swap(trans, t.trans);
		std::swap(cycle, t.cycle);
	}

	/**
	 * Gets a reference to the transient states.
	 * @return the sequence of transient states
	 */
	const StateContainer& getTransient() const {
		return trans;
	}

	/**
	 * Gets a reference to the cycle states.
	 * @return the sequence of cycle states
	 */
	const StateContainer& getCycle() const {
		return cycle;
	}

	/**
	 * Returns the number of transient states.
	 *
	 * Equivalent to:
	 * @code
	 * getTransient().size();
	 * @endcode
	 * @return the number of transient states
	 */
	size_t transientLength() const {
		return trans.size();
	}

	/**
	 * Returns the number of cycle states.
	 *
	 * Equivalent to:
	 * @code
	 * getCycle().size();
	 * @endcode
	 * @return the number of cycle states
	 */
	size_t cycleLength() const {
		return cycle.size();
	}

	/**
	 * Convenient method to print all cycle states to a stream.
	 *
	 * It should be used "inline" like a stream modifier:
	 * @code
	 * Trajectory t;
	 * cout << "Cycle states\n:" << printCycle() << endl;
	 * @endcode
	 * @param sep a string separator that interleaves state representations
	 * @return an inaccessible (private) proxy object used for printing
	 */
	util::Printer<StateContainer> printCycle(const char sep[] = "\n") const {
		return util::dump(cycle, sep);
	}

	/**
	 * Convenient method to print all transient states to a stream.
	 *
	 * Has the same behaviour as Trajectory::attractor(const char[]) const.
	 * @param sep a string separator that interleaves state representations
	 * @return an inaccessible proxy object used for printing
	 */
	util::Printer<StateContainer> printTransient(const char sep[] = "\n") const {
		return util::dump(trans, sep);
	}

	/**
	 * Function to print all the states in a trajectory object.
	 *
	 * Equivalent to:
	 * @code
	 * Trajectory t;
	 * out << printTransient() << printCycle();
	 * @endcode
	 * @param out an output stream
	 * @param t a Trajectory object
	 * @return the stream passed as argument
	 */
	friend std::ostream& operator<<(std::ostream& out, const Trajectory& t) {
		return out << t.printTransient() << t.printCycle();
	}
};

}

namespace std {

/**
 * Specialization that calls bn::Trajectory::swap(Trajectory&) on the first
 * argument.
 * @param a a trajectory
 * @param b another trajectory
 */
inline void swap(bn::Trajectory& a, bn::Trajectory& b) {
	a.swap(b);
}

} // namespace std

#endif /* TRAJECTORY_HPP_ */
