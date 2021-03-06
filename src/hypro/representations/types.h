#pragma once
#include <boost/variant.hpp>

namespace hypro {

/**
 * @brief      Enum encapsulating all provided state set representations to be able to determine a type.
 */
enum representation_name { box, constraint_set, polytope_h, polytope_v, ppl_polytope, support_function, taylor_model, zonotope };

} // namespace hypro

namespace std {
	/*
	ostream& operator<<(ostream& out, hypro::representation_name in) {
		switch(in){
			case(hypro::representation_name::box): { return out << "box"; }
			case(hypro::representation_name::constraint_set): { return out << "constraint_set"; }
			case(hypro::representation_name::polytope_h): { return out << "polytope_h"; }
			case(hypro::representation_name::polytope_v): { return out << "polytope_v"; }
			case(hypro::representation_name::ppl_polytope): { return out << "ppl_polytope"; }
			case(hypro::representation_name::support_function): { return out << "support_function"; }
			case(hypro::representation_name::taylor_model): { return out << "taylor_model"; }
			case(hypro::representation_name::zonotope): { return out << "zonotope"; }
			default: return out;
		}
	}
	*/

} // namespace std
