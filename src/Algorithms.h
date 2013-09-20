/* 
 * Algorithms.h
 *
 * Copyright 2012 Martin Robinson
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
 *  Created on: Sep 19, 2013
 *      Author: mrobins
 */

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include "Traits.h"

template<typename F, typename T1, typename T2, typename Traits=TopologyTraits<T1,T2> >
void for_each_neighbours(multivector_type input, multivector_type output, F function, topology_type topology=topology_type()) {
	index_type nr = Traits::get_number_of_rows(output);
	Traits::embed_source_positions(topology,input);
	for (index_type i = 0; i < nr; ++i) {
		multivector_type neighbours = Traits::find_neighbours(topology,output,i);
		function(Traits::get_row(output,i),neighbours,i);
	}
}


template<typename F, typename T, typename Traits=BaseTraits<T> >
void for_each(multivector_type input, multivector_type output, F function) {
	index_type n = Traits::get_number_of_rows(output);
	for (index_type i = 0; i < n; ++i) {
		function(Traits::get_row(output,i),Traits::get_row(output,i),i);
	}
}



#endif /* ALGORITHMS_H_ */
