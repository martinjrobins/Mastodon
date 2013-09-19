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

template<typename F, typename T, typename Traits=BaseTraits<T> >
topology_type sum_over_neighbours(multivector_type input, multivector_type output, F function) {
	index_type nr = Traits::get_number_of_rows(output);
	index_type ne = Traits::get_number_of_elments(Traits::get_row(output,0));

	for (index_type i = 0; i < nr; ++i) {
		find_neighbours(i)
						construct multivector of only neighbours
						function(output_row,multivector_of_neighbours_input)
	}
}


template<typename F, typename T, typename Traits=BaseTraits<T> >
void for_each_stencil(multivector_type input, multivector_type output, F function, index_type stencil) {
	index_type n = Traits::get_number_of_rows(output);
	for (index_type i = 0; i < n; ++i) {
		function(input,Traits::get_row(output,i),i);
	}
}

template<typename F, typename T, typename Traits=BaseTraits<T> >
void for_each(multivector_type input, multivector_type output, F function) {
	index_type n = Traits::get_number_of_rows(output);
	for (index_type i = 0; i < n; ++i) {
		function(input,Traits::get_row(output,i),i);
	}
}



#endif /* ALGORITHMS_H_ */
