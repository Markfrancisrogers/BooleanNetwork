/*
 * AttractorGen.hpp
 *
 *  Created on: Aug 26, 2009
 *      Author: stewie
 */

#ifndef ATTRACTORGEN_HPP_
#define ATTRACTORGEN_HPP_

#include <cstddef>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include "../core/BooleanDynamics_fwd.hpp"
#include "../experiment/NetworkDynamics.hpp"

namespace bn {

namespace gen {

namespace detail {

template<class StateIt, class AttractorType> class AttractorIterator;

template<class StateIt, class AttractorType = Attractor> struct AtttractorIteratorBase {
	typedef typename boost::remove_pointer<AttractorType>::type base_value_type;
	typedef typename boost::is_pointer<AttractorType>::type is_ptr;
	typedef typename boost::mpl::if_<is_ptr,
			boost::shared_ptr<base_value_type>, base_value_type>::type
			value_type;
	typedef boost::iterator_adaptor<AttractorIterator<StateIt, AttractorType> ,
			StateIt, value_type, boost::single_pass_traversal_tag,
			const value_type> super;
};

/**
 * Input Iterator.
 */
template<class StateIt, class AttractorType>
class AttractorIterator : public AtttractorIteratorBase<StateIt, AttractorType>::super {
private:
	friend class boost::iterator_core_access;

	typedef AtttractorIteratorBase<StateIt, AttractorType> base;
	typedef typename base::is_ptr is_ptr;
	typedef typename base::base_value_type base_value_type;
	typedef typename base::super::value_type value_t;

	BooleanDynamics* net;
	NetworkDynamics runner;

	template<class IsPtr>
	typename boost::enable_if<IsPtr, value_t>::type nextAttractor() const {
		return boost::make_shared<base_value_type>(runner.findAttractor(*net,
				*this->base_reference()));
	}

	template<class IsPtr>
	typename boost::disable_if<IsPtr, value_t>::type nextAttractor() const {
		boost::scoped_ptr<ExplicitAttractor> t(runner.findAttractor(*net,
				*this->base_reference()));
		return value_t(*t);
	}

	value_t dereference() const {
		return nextAttractor<is_ptr> ();
	}

public:

	AttractorIterator() :
		net(NULL) {
	}

	AttractorIterator(const StateIt it, BooleanDynamics& net,
			const size_t muteSteps) :
		AttractorIterator::iterator_adaptor_(it), net(&net), runner(0,
				muteSteps) {
	}

	AttractorIterator(const StateIt it) :
		AttractorIterator::iterator_adaptor_(it), net(NULL) {
	}
};

}

/**
 * @ingroup generator
 */
template<class SinglePassRange, class AttractorType = ExplicitAttractor> class AttractorGen : public boost::iterator_range<
		detail::AttractorIterator<typename boost::range_iterator<
				SinglePassRange>::type, AttractorType> > {
private:
	typedef detail::AttractorIterator<typename boost::range_iterator<
			SinglePassRange>::type, AttractorType> iter;
	typedef boost::iterator_range<iter> base;

public:

	AttractorGen(SinglePassRange& gen, BooleanDynamics& net,
			const size_t muteSteps) :
		base(iter(boost::begin(gen), net, muteSteps), iter(boost::end(gen))) {
	}

	template<class OtherRange> AttractorGen(const AttractorGen<OtherRange,
			AttractorType>& other) :
		base(other) {
	}
};

namespace tag {

template<class AttractorType = Attractor> struct generate_attractors {

	template<class StateRange> struct apply {
		typedef AttractorGen<StateRange, AttractorType> type;
	};
};

} // namespace tag

template<class AttractorType = Attractor> struct generate_attractors {
	BooleanDynamics& net;
	const size_t muteSteps;

	generate_attractors(BooleanDynamics& net, const size_t muteSteps) :
		net(net), muteSteps(muteSteps) {
	}
};

template<class SinglePassRange, class AttractorType> AttractorGen<
		SinglePassRange, AttractorType> operator|(SinglePassRange& sr,
		const generate_attractors<AttractorType>& ga) {
	return AttractorGen<SinglePassRange, AttractorType> (sr, ga.net,
			ga.muteSteps);
}

template<class SinglePassRange, class AttractorType> AttractorGen<
		const SinglePassRange, AttractorType> operator|(
		const SinglePassRange& sr, const generate_attractors<AttractorType>& ga) {
	return AttractorGen<const SinglePassRange, AttractorType> (sr, ga.net,
			ga.muteSteps);
}

}

}

#endif /* ATTRACTORGEN_HPP_ */
