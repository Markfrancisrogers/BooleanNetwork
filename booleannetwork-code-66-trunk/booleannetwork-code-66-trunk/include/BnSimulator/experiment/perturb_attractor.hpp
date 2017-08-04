/*
 * perturb_attractor.hpp
 *
 *  Created on: May 1, 2011
 *      Author: stewie
 */

#ifndef PERTURB_ATTRACTOR_HPP_
#define PERTURB_ATTRACTOR_HPP_

#include <cstddef>
#include <utility>
#include <map>

#include <boost/iterator/transform_iterator.hpp>

#include "../util/Counter.hpp"
#include "../core/Attractor.hpp"
#include "cycle_finder.hpp"

namespace bn {

namespace detail {

struct Normalize {
	template<class Sig> struct result;
	template<class K> struct result<Normalize(std::pair<const K, std::size_t>&)> {
		typedef std::pair<K, double> type;
	};
	const std::size_t n;
	Normalize(const std::size_t n) :
		n(n) {
	}
	template<class K> std::pair<K, double> operator()(
			const std::pair<const K, std::size_t>& p) const {
		return std::make_pair(p.first, static_cast<double> (p.second) / n);
	}
};

} // namespace detail

template<class CycleFinder> std::map<Attractor, double> perturb_attractor(
		const Attractor& a, CycleFinder f) {
	util::Counter<Attractor> c;
	for (Attractor::const_iterator it = a.begin(), end = a.end(); it != end; ++it) {
		for (std::size_t i = 0; i < it->size(); ++i) {
			const Attractor x = f(State(*it).flip(i));
			if (x != EMPTY_ATTRACTOR) {
				c.insert(x);
			}
		}
	}
	const detail::Normalize func(c.insertions());
	return std::map<Attractor, double>(
			boost::make_transform_iterator(c.begin(), func),
			boost::make_transform_iterator(c.end(), func));
}

} // namespace bn

#endif /* PERTURB_ATTRACTOR_HPP_ */
