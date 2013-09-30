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
	typedef T multivector_type;
	typedef T::value_type row_type;
	typedef T::value_type::value_type element_type;
	typedef Vector<element_type> position_type;
	typedef unsigned int index_type;
	typedef element_type interaction_radius_type;

	static index_type get_number_of_rows(const multivector_type& v) {return v->size();}
	static index_type get_number_of_elements(const row_type& r) {return r->size();}
	static row_type& get_row(multivector_type& v, int i) {return *v[i];}
	static element_type& get_element(row_type& v, int i) {return *v[i];}
	static element_type& get_element(multivector_type& v, int i, int j) {return get_element(get_row(i),j);}


	static position_type get_position_vector(row_type& row) {
		return position_type(get_element(row,0),get_element(row,1),get_element(row,2));
	}
	static interaction_radius_type get_interaction_radius(row_type& row) {
		return interaction_radius_type(get_element(row,3));
	}
};

template<typename T>
struct GraphTraits {
	typedef T graph_type;
	typedef T::value_type row_type;
	typedef T::value_type::value_type index_type;

	static index_type get_number_of_rows(const graph_type& v) {return v->size();}
	static index_type get_number_of_elements(const row_type& r) {return r->size();}
	static void add_edge(row_type& r, index_type j) {r.push_back(j);}
	static void remove_all_edges(row_type& r) {r.clear();}
	static row_type& get_row(graph_type& v, int i) {return *v[i];}
	static index_type& get_element(row_type& v, int i) {return *v[i];}
	static index_type& get_element(graph_type& v, int i, int j) {return get_element(get_row(i),j);}

	static graph_type create_graph(index_type number_of_rows);


	static position_type get_position_vector(row_type& row) {
		return position_type(get_element(row,0),get_element(row,1),get_element(row,2));
	}
	static interaction_radius_type get_interaction_radius(row_type& row) {
		return interaction_radius_type(get_element(row,3));
	}
};





#endif /* TRAITS_H_ */
