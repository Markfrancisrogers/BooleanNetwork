#include <cassert>
#include <algorithm>

#include <BnSimulator/core/BooleanFunction.hpp>

using namespace std;

namespace bn {

BooleanFunction::BooleanFunction(const State& s) :
	def(s), arity(arityFromSize(def.size())) {
	for (size_t i = 0; i < def.size(); ++i)
		def[i] = s[i];
}

vector<int> BooleanFunction::truthTable() const {
	vector<int> tt(def.size());
	for (size_t i = 0; i < def.size(); ++i)
		tt[i] = def[i];
	return tt;
}

bool BooleanFunction::isInfluent(const size_t i) const {
	const pair<State, State> m = demultiplex(i);
	return (m.first ^ m.second).any();
}

BooleanFunction BooleanFunction::clamped(const std::size_t i, const bool v) const {
	const pair<State, State> m = demultiplex(i);
	return BooleanFunction(v ? m.second : m.first);
}

void BooleanFunction::clamp(const std::size_t i, const bool v) {
	const pair<State, State> m = demultiplex(i);
	def = v ? m.second : m.first;
	--arity;
}

pair<State, State> BooleanFunction::demultiplex(const std::size_t i) const {
	assert(arity > 0 && i < arity);
	const size_t windowLength = 1 << i;
	const size_t numWindows = 1 << (arity - i);
	size_t index = 0;
	bool flag = false;
	State zero, one;
	for (size_t w = 0; w < numWindows; ++w) {
		for (size_t j = 0; j < windowLength; ++j, ++index) {
			if (flag)
				one.push_back(def[index]);
			else
				zero.push_back(def[index]);
		}
		flag = !flag;
	}
	assert(zero.size() == one.size());
	return make_pair(zero, one);
}

bool BooleanFunction::operator()(const State& x) const {
	assert(arity == x.size());
	size_t index = 0; // index computation is the most expensive operation
	for (size_t j = 0; j < arity; ++j)
		index += (1 << j) * x[j];
	assert(index < def.size());
	return def[index];
}

bool BooleanFunction::operator<(const BooleanFunction& other) const {
	State otherDef(other.def);
	otherDef.resize(def.size());
	return def < otherDef;
}

ostream& operator<<(ostream& out, const BooleanFunction& f) {
	for (size_t i = 0; i < f.def.size(); ++i)
		out << f.def[i];
	return out;
}

size_t hash_value(const BooleanFunction& f) {
	return bitset_hash(f.def);
}

void swap(BooleanFunction& a, BooleanFunction& b) {
	using std::swap;
	swap(a.def, b.def);
	swap(a.arity, b.arity);
}

} // namespace bn
