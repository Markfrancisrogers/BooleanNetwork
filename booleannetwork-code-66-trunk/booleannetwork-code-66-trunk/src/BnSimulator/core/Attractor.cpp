/*
 * Attractor.cpp
 *
 *  Created on: May 12, 2011
 *      Author: stewie
 */

#include <BnSimulator/util/printing.hpp>
#include <BnSimulator/core/Attractor.hpp>

namespace bn {

std::ostream& operator<<(std::ostream& out, const ImplicitAttractor& a) {
	return out << a.representant;
}

std::ostream& operator<<(std::ostream& out, const Attractor& a) {
	util::print(out, a.states, "\n");
	return out;
}

} // namespace bn
