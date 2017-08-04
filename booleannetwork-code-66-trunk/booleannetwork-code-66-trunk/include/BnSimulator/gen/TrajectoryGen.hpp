#ifndef TRAJECTORYGEN_HPP_
#define TRAJECTORYGEN_HPP_

#include <cstddef>
#include <iterator>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include "../core/BooleanNetwork_fwd.hpp"
#include "../core/Trajectory.hpp"
#include "../experiment/NetworkDynamics.hpp"

namespace bn {

namespace gen {

namespace detail {

template<class StateIt, class TrajectoryType> class TrajectoryIterator;

template<class StateIt, class TrajectoryType = Trajectory> struct TrajectoryIteratorBase {
	typedef typename boost::remove_pointer<TrajectoryType>::type
			base_value_type;
	typedef typename boost::is_pointer<TrajectoryType>::type is_ptr;
	typedef typename boost::mpl::if_<is_ptr,
			boost::shared_ptr<base_value_type>, base_value_type>::type
			value_type;
	typedef boost::iterator_adaptor<
			AttractorIterator<StateIt, TrajectoryType> , StateIt, value_type,
			boost::single_pass_traversal_tag, const value_type> super;
};

/**
 * Input Iterator.
 */
template<class StateIt, class TrajectoryType>
class TrajectoryIterator : public TrajectoryIteratorBase<StateIt,
		TrajectoryType>::super {
private:
	friend class boost::iterator_core_access;

	typedef TrajectoryIteratorBase<StateIt, TrajectoryType> base;
	typedef typename base::is_ptr is_ptr;
	typedef typename base::base_value_type base_value_type;
	typedef typename base::super::value_type value_t;

	BooleanNetwork* net;
	NetworkDynamics runner;

	template<class IsPtr>
	typename boost::enable_if<IsPtr, value_t>::type nextTrajectory() const {
		return boost::make_shared<base_value_type>(runner.computeTrajectory(
				*net, *this->base_reference()));
	}

	template<class IsPtr>
	typename boost::disable_if<IsPtr, value_t>::type nextTrajectory() const {
		boost::scoped_ptr<Trajectory> t(runner.computeTrajectory(*net,
				*this->base_reference()));
		return value_t(*t);
	}

	value_t dereference() const {
		return nextTrajectory<is_ptr> ();
	}

public:

	TrajectoryIterator() :
		net(NULL) {
	}

	TrajectoryIterator(const StateIt it, BooleanNetwork& net,
			const size_t maxSteps) :
		TrajectoryIterator::iterator_adaptor_(it), net(&net), runner(maxSteps) {
	}

	TrajectoryIterator(const StateIt it) :
		TrajectoryIterator::iterator_adaptor_(it), net(NULL) {
	}
};

}

/**
 * @ingroup generator
 */
template<class SinglePassRange, class TrajectoryType = Trajectory> class TrajectoryGen : public boost::iterator_range<
		detail::TrajectoryIterator<typename boost::range_iterator<
				SinglePassRange>::type, TrajectoryType> > {
private:
	typedef detail::TrajectoryIterator<typename boost::range_iterator<
			SinglePassRange>::type, TrajectoryType> iter;
	typedef boost::iterator_range<iter> base;

public:

	TrajectoryGen(SinglePassRange& gen, BooleanNetwork& net,
			const uint maxSteps) :
		base(iter(boost::begin(gen), net, maxSteps), iter(boost::end(gen))) {
	}

	template<class OtherRange> TrajectoryGen(const TrajectoryGen<OtherRange,
			TrajectoryType>& other) :
		base(other) {
	}
};

namespace tag {

template<class TrajectoryType = Trajectory> struct generate_trajectories {

	template<class StateRange> struct apply {
		typedef TrajectoryGen<StateRange, TrajectoryType> type;
	};
};

} // namespace tag

template<class TrajectoryType = Trajectory> struct generate_trajectories {
	BooleanNetwork& net;
	const size_t maxSteps;

	generate_trajectories(BooleanNetwork& net, const size_t maxSteps) :
		net(net), maxSteps(maxSteps) {
	}
};

template<class SinglePassRange, class TrajectoryType> TrajectoryGen<
		SinglePassRange, TrajectoryType> operator|(SinglePassRange& sr,
		const generate_trajectories<TrajectoryType>& ga) {
	return TrajectoryGen<SinglePassRange, TrajectoryType> (sr, ga.net,
			ga.maxSteps);
}

template<class SinglePassRange, class TrajectoryType> TrajectoryGen<
		const SinglePassRange, TrajectoryType> operator|(
		const SinglePassRange& sr,
		const generate_trajectories<TrajectoryType>& ga) {
	return TrajectoryGen<const SinglePassRange, TrajectoryType> (sr, ga.net,
			ga.maxSteps);
}

}

}

#endif /* TRAJECTORYGEN_HPP_ */
