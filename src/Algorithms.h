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
#include <math.h>
#include <memory>
#include "BucketSort.h"

namespace Mastodon {

template<typename F, typename T1, typename T2, typename MTraits=MultivectorTraits<T1>, typename GTraits=GraphTraits<T2> >
void for_each_neighbours(T1& input, T1& output, F function, T2& graph) {
	index_type nr = Traits::get_number_of_rows(output);
	Traits::embed_source_positions(topology,input);
	for (index_type i = 0; i < nr; ++i) {
		multivector_type neighbours = Traits::find_neighbours(topology,output,i);
		function(Traits::get_row(output,i),neighbours,i);
	}
}



template<typename T1, typename T2, typename MTraits=MultivectorTraits<T1>, typename GTraits=GraphTraits<T2> >
void find_nearest_neighbours(T1& input, T1& output, T2& graph, MTraits::index_type interaction_radius_column, std::unique_ptr<BucketSort<T1> >& bucket_sort) {
	typedef GTraits::index_type index_type;
	typedef MTraits::position_type position_type;
	const index_type ni = MTraits::get_number_of_rows(input);
	position_type min(100,100,100),max(-100,-100,-100);
	double maxh = -100;
	for (index_type i = 0; i < ni; ++i) {
		GTraits::remove_all_edges(MTraits::get_row(graph,i));
		const position_type p = MTraits::get_position_vector(MTraits::get_row(input,i));
		const double h = MTraits::get_element(MTraits::get_row(input,i),interaction_radius_column);
		for (int d = 0; d < 3; ++d) {
			if (p[d]<min[d]) min[d] = p[d];
			if (p[d]>max[d]) max[d] = p[d];
			if (h > maxh) maxh = h;
		}
	}
	bucket_sort->update(min,max,maxh);
	bucket_sort->embed_source_positions(input);
	const index_type no = MTraits::get_number_of_rows(output);
	bool self = MTraits::same_multivector(input,output);
	if (self) {
		for (index_type i = 0; i < no; ++i) {
			GTraits::row_type graph_row = GTraits::get_row(graph,i);
			const position_type ri = MTraits::get_position_vector(MTraits::get_row(output,i));
			const double hi =  MTraits::get_element(MTraits::get_row(input,i),interaction_radius_column);
			bool found_self = false;
			for (auto iterator = bucket_sort->get_broadphase_neighbours(ri);
					iterator != bucket_sort->end(); ++iterator) {
				if (found_self) {
					const index_type j = *iterator;
					const position_type rj = MTraits::get_position_vector(MTraits::get_row(output,j));
					const double hj = MTraits::get_element(MTraits::get_row(input,j),interaction_radius_column);
					const double maxh2 = pow(std::max(hi,hj),2);
					if ((ri-rj).squaredNorm() < maxh2) {
						GTraits::add_edge(graph_row, j);
						GTraits::add_edge(GTraits::get_row(graph,j), i);
					}
				} else if (i==*iterator) {
					found_self = true;
				}
			}
		}

	} else {
		for (index_type i = 0; i < no; ++i) {
			GTraits::row_type graph_row = GTraits::get_row(graph,i);
			const position_type ri = MTraits::get_position_vector(MTraits::get_row(output,i));
			const double hi =  MTraits::get_element(MTraits::get_row(input,i),interaction_radius_column);
			for (auto iterator = bucket_sort->get_broadphase_neighbours(ri);
					iterator != bucket_sort->end(); ++iterator) {
				const index_type j = *iterator;
				const position_type rj = MTraits::get_position_vector(MTraits::get_row(output,j));
				const double hj = MTraits::get_element(MTraits::get_row(input,j),interaction_radius_column);
				const double maxh2 = pow(std::max(hi,hj),2);
				if ((ri-rj).squaredNorm() < maxh2) {
					GTraits::add_edge(graph_row, j);
				}
			}
		}
	}
}

template<typename T1, typename T2, typename MTraits=MultivectorTraits<T1>, typename GTraits=GraphTraits<T2> >
std::unique_ptr<BucketSort<T1> > find_nearest_neighbours(T1& input, T1& output, T2& graph, MTraits::index_type interaction_radius_column) {
	std::unique_ptr<BucketSort<T1> > bucket_sort(new BucketSort<T1>);
	find_nearest_neighbours(input,output,graph,interaction_radius_column, bucket_sort);
	return bucket_sort;
}


template<typename F, typename T, typename Traits=BaseTraits<T> >
void for_each(T& input, T& output, F function) {
	index_type n = Traits::get_number_of_rows(output);
	for (index_type i = 0; i < n; ++i) {
		function(Traits::get_row(output,i),Traits::get_row(output,i),i);
	}
}

}


#endif /* ALGORITHMS_H_ */
