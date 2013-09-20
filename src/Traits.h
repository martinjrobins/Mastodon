/*
 * Traits.h
 * 
 * Copyright 2013 Martin Robinson
 *
 * This file is part of Mastodon.
 *
 * Mastodon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mastodon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Mastodon.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 19 Sep 2013
 *      Author: robinsonm
 */

#ifndef TRAITS_H_
#define TRAITS_H_

#include "Vector.h"

template<typename T>
struct MultivectorTraits {
	typedef std::shared_ptr<T> multivector_type;
	typedef T::value_type row_type;
	typedef T::value_type::value_type element_type;
	typedef Vector<element_type> position_type;
	typedef unsigned int index_type;
	typedef element_type interaction_radius_type;

	static index_type get_number_of_rows(multivector_type v) {return v->size();}
	static index_type get_number_of_elements(row_type r) {return r->size();}
	static row_type& get_row(multivector_type v, int i) {return *v[i];}
	static element_type& get_element(row_type& v, int i) {return *v[i];}
	static element_type& get_element(multivector_type& v, int i, int j) {return get_element(get_row(i),j);}

	static position_type get_position_vector(row_type row) {
		return position_type(get_element(row,0),get_element(row,1),get_element(row,2));
	}
	static interaction_radius_type get_interaction_radius(row_type row) {
		return interaction_radius_type(get_element(row,3));
	}
};

template<typename T1, typename T2>
struct TopologyTraits: MultivectorTraits<T1> {

	typedef MultivectorTraits<T1> Traits;
	typedef Traits::multivector_type multivector_type;
	typedef Traits::row_type row_type;
	typedef Traits::element_type element_type;
	typedef Traits::index_type index_type;
	typedef Traits::position_type position_type;
	typedef Traits::interaction_radius_type interaction_radius_type;

	typedef std::shared_ptr<T2> topology_type;

	static void embed_source_positions(topology_type topology, multivector_type source) {
		topology.embed_source_positions(source);
	}

	static multivector_type find_neighbours(topology_type topology, multivector_type destination, index_type i) {
		return topology.find_neighbours(destination,i);
	}
};



#endif /* TRAITS_H_ */
