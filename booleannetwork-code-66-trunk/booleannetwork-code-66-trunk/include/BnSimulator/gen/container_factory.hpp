/*
 * container_factory.hpp
 *
 *  Created on: Aug 26, 2009
 *      Author: stewie
 */

#ifndef CONTAINER_FACTORY_HPP_
#define CONTAINER_FACTORY_HPP_

#include <set>
#include <functional> // for std::less

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/if.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/ptr_container/indirect_fun.hpp>

namespace bn {

template<class AttractorType> struct Garden {
	typedef typename boost::remove_pointer<AttractorType>::type base_type;
	typedef typename boost::mpl::if_<
			typename boost::is_pointer<AttractorType>::type, std::set<
					boost::shared_ptr<base_type>, boost::indirect_fun<
							std::less<base_type> > >, std::set<base_type> >::type
			type;

};

}

#endif /* CONTAINER_FACTORY_HPP_ */
