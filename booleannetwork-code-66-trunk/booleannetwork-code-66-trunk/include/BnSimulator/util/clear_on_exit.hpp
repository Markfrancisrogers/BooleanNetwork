/*
 * clear_on_exit.hpp
 *
 *  Created on: Sep 3, 2009
 *      Author: stewie
 */

#ifndef CLEAR_ON_EXIT_HPP_
#define CLEAR_ON_EXIT_HPP_

#include "scope_guard.hpp"
#include "utility.hpp"

namespace bn {

namespace util {

namespace detail {

/**
 * Scope-guard to invoke purge() on an stl sequence.
 *
 * Type parameter Sequence must be a non-associative stl sequence. purge() is
 * called when destructor is invoked.
 *
 * A rather unintuitive piece of code like this:
 * @code
 * vector<SomePolymorphicObject*> v;
 * // fill v with values
 * // compute some value of type SomeType
 * SomeType result = computeValue(v);
 * purge(v); // free memory
 * return result;
 * @endcode
 * becomes much more clear and safe:
 * @code
 * vector<SomePolymorphicObject*> v;
 * // set up scope guard
 * clear_on_exit<vector<SomePolymorphicObject*> > guard(v); // variable name is irrelevant
 * // fill v with values
 * // compute some value of type SomeType and return it immediately
 * return computeValue(v); // v is automatically freed, no memory leak
 * @endcode
 */
template<class Sequence> class clear_on_exit_impl : public scope_guard_facade<
		clear_on_exit_impl<Sequence> > {
private:
	/**
	 * Sequence to purge.
	 */
	Sequence& target;

public:
	/**
	 * Initializes this scope-guard.
	 * @param target a sequence to purge
	 */
	explicit clear_on_exit_impl(Sequence& target) :
		target(target) {
	}

	/**
	 * Invokes purge() on the sequence.
	 */
	void operator()() const {
		purge(target);
	}
};

/**
 * Scope-guard to invoke purge() on a pointer to an stl sequence.
 *
 * The destructor of this class also deletes the sequence by calling @e delete
 * on the pointer.
 *
 * Has the same requirement and behaviour as clear_on_exit<Sequence>.
 */
template<class Sequence> class clear_on_exit_impl<Sequence*> : public scope_guard_facade<
		clear_on_exit_impl<Sequence*> > {
private:
	/**
	 * Pointer to the sequence to purge.
	 */
	Sequence* const target;

public:
	/**
	 * Initializes this scope-guard.
	 * @param target a pointer to a sequence to purge
	 */
	explicit clear_on_exit_impl(Sequence* const target) :
		target(target) {
	}

	/**
	 * Invokes purge() on the sequence and then deletes the sequence through
	 * the pointer.
	 */
	void operator()() const {
		purge(*target);
		delete target;
	}
};

}

template<class Sequence> detail::clear_on_exit_impl<Sequence> clear_on_exit(
		Sequence& seq) {
	return detail::clear_on_exit_impl<Sequence>(seq);
}

template<class Sequence> detail::clear_on_exit_impl<Sequence*> clear_on_exit(
		Sequence* seq) {
	return detail::clear_on_exit_impl<Sequence*>(seq);
}

}

}

#endif /* CLEAR_ON_EXIT_HPP_ */
