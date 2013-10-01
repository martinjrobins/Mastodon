/*
 * BucketSort.h
 *
 * Copyright 2012 Martin Robinson
 *
 * This file is part of RD_3D.
 *
 * RD_3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RD_3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RD_3D.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 22 Oct 2012
 *      Author: robinsonm
 */

#ifndef BUCKETSORT_H_
#define BUCKETSORT_H_

#include "Log.h"
#include "Traits.h"
#include "Constants.h"

namespace Mastodon {

const int CELL_EMPTY = -1;

template<typename T, typename MTraits=MultivectorTraits<T> >
class BucketSort {
public:
	typedef MTraits::position_type Vect3d;
	typedef MTraits::multivector_type multivector_type;

	typedef Vector<bool> Vect3b;
	typedef Vector<int> Vect3i;

	struct iterator;

	BucketSort():
			low(0),high(0),domain_size(high-low),periodic(false) {
			LOG(2,"Creating bucketsort data structure with lower corner = "<<low<<" and upper corner = "<<high);
			const double dx = (high-low).max()/10.0;
			reset(low, high, dx);
		}
	BucketSort(Vect3d low, Vect3d high, Vect3b periodic):
		low(low),high(high),domain_size(high-low),periodic(periodic) {
		LOG(2,"Creating bucketsort data structure with lower corner = "<<low<<" and upper corner = "<<high);
		const double dx = (high-low).max()/10.0;
		reset(low, high, dx);
	}

	void reset(const Vect3d& low, const Vect3d& high, double _max_interaction_radius);
	inline const Vect3d& get_low() {return low;}
	inline const Vect3d& get_high() {return high;}

	void embed_source_positions(multivector_type& input);

	iterator end() {return iterator(0);}
	iterator get_broadphase_neighbours(Vect3d& position);

	Vect3d correct_position_for_periodicity(const Vect3d& source_r, const Vect3d& to_correct_r);
	Vect3d correct_position_for_periodicity(const Vect3d& to_correct_r);

private:
	inline int vect_to_index(const Vect3i& vect) {
		return vect[0] * num_cells_along_yz + vect[1] * num_cells_along_axes[1] + vect[2];
	}
	inline int find_cell_index(const Vect3d &r) {
		const Vect3i celli = ((r-low)*(inv_cell_size) + Vect3d(1.0,1.0,1.0));
		ASSERT((celli[0] >= 0) && (celli[0] < num_cells_along_axes[0]), "position is outside of x-range");
		ASSERT((celli[1] >= 0) && (celli[1] < num_cells_along_axes[1]), "position is outside of y-range");
		ASSERT((celli[2] >= 0) && (celli[2] < num_cells_along_axes[2]), "position is outside of z-range");
		//std::cout << " looking in cell " << celli <<" out of total cells " << num_cells_along_axes << " at position " << r<< std::endl;
		return vect_to_index(celli);
	}

    std::vector<int> cells;
    std::vector<std::vector<int> > ghosting_indices_pb;
    std::vector<std::pair<int,int> > ghosting_indices_cb;
    std::vector<int> dirty_cells;
	std::vector<int> linked_list;
	std::vector<int> neighbr_list;
	Vect3d low,high,domain_size;
	const Vect3b periodic;
	Vect3d cell_size,inv_cell_size;
	Vect3i num_cells_along_axes;
	int num_cells_along_yz;
	double max_interaction_radius;
	std::vector<int> surrounding_cell_offsets;
};
template<typename T, typename MTraits=MultivectorTraits<T> >
void BucketSort<T,MTraits>::reset(const BucketSort<T,MTraits>::Vect3d& _low, const BucketSort<T,MTraits>::Vect3d& _high, double _max_interaction_radius) {
	LOG(2,"Resetting bucketsort data structure:");
	LOG(2,"\tMax interaction radius = "<<_max_interaction_radius);
	high = _high;
	low = _low;
	max_interaction_radius = _max_interaction_radius;
	Vect3i num_cells_without_ghost = ((high-low)/max_interaction_radius).cast<int>();
	Vect3d new_high = high;
	for (int i = 0; i < NDIM; ++i) {
		if (num_cells_without_ghost[i]==0) {
			LOG(2,"\tNote: Dimension "<<i<<" has no length, setting cell side equal to interaction radius.");
			new_high[i] = low[i] + max_interaction_radius;
			num_cells_without_ghost[i] = 1;
		}
	}
	num_cells_along_axes = num_cells_without_ghost + Vect3i(3,3,3);
	LOG(2,"\tNumber of cells along each axis = "<<num_cells_along_axes);
	cell_size = (new_high-low).cwiseQuotient(num_cells_without_ghost.cast<double>());
	LOG(2,"\tCell sizes along each axis = "<<cell_size);
	inv_cell_size = Vect3d(1,1,1).cwiseQuotient(cell_size);
	num_cells_along_yz = num_cells_along_axes[2]*num_cells_along_axes[1];
	const unsigned int num_cells = num_cells_along_axes.prod();
	cells.assign(num_cells, CELL_EMPTY);
	//TODO: assumed 3d
	surrounding_cell_offsets.clear();
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			for (int k = -1; k < 2; ++k) {
				surrounding_cell_offsets.push_back(vect_to_index(Vect3i(i,j,k)));
			}
		}
	}


	ghosting_indices_pb.assign(num_cells, std::vector<int>());
	ghosting_indices_cb.clear();
	for (int i = 0; i < NDIM; ++i) {
		if (!periodic[i]) continue;
		int j,k;
		switch (i) {
			case 0:
				j = 1;
				k = 2;
				break;
			case 1:
				j = 0;
				k = 2;
				break;
			case 2:
				j = 0;
				k = 1;
				break;
			default:
				break;
		}

		Vect3i tmp;
		const int n = num_cells_along_axes[i];
		for (int jj = 0; jj < num_cells_along_axes[j]-2; ++jj) {
			tmp[j] = jj;
			for (int kk = 0; kk < num_cells_along_axes[k]-2; ++kk) {
				tmp[k] = kk;
				tmp[i] = n-3;
				const int index_from1 = vect_to_index(tmp);
				tmp[i] = 0;
				const int index_to1 = vect_to_index(tmp);
				ghosting_indices_pb[index_from1].push_back(index_to1);
				ghosting_indices_cb.push_back(std::pair<int,int>(index_to1,index_from1));
				tmp[i] = 1;
				const int index_from2 = vect_to_index(tmp);
				tmp[i] = n-2;
				const int index_to2 = vect_to_index(tmp);
				ghosting_indices_pb[index_from2].push_back(index_to2);
				ghosting_indices_cb.push_back(std::pair<int,int>(index_to2,index_from2));
			}
		}
	}
}
template<typename T, typename MTraits=MultivectorTraits<T> >
void BucketSort<T,MTraits>::embed_source_positions(BucketSort<T,MTraits>::multivector_type &source) {
	const unsigned int n = Traits::get_number_of_rows(source);
	linked_list.assign(n, CELL_EMPTY);
	const bool particle_based = dirty_cells.size() < cells.size();
	if (particle_based) {
		BOOST_FOREACH(int i, dirty_cells) {
			cells[i] = CELL_EMPTY;
		}
	} else {
		cells.assign(cells.size(), CELL_EMPTY);
	}

	dirty_cells.clear();
	for (int i = 0; i < n; ++i) {
		const int celli = find_cell_index(Traits::get_position_vector(source,i));
		const int cell_entry = cells[celli];

		// Insert into own cell
		cells[celli] = i;
		dirty_cells.push_back(celli);
		linked_list[i] = cell_entry;

		// Insert into ghosted cells
		if (particle_based) {
			BOOST_FOREACH(int j, ghosting_indices_pb[celli]) {
				const int cell_entry = cells[j];
				cells[j] = i;
				dirty_cells.push_back(j);
				linked_list[i] = cell_entry;
			}
		}
	}

	if (!particle_based) {
		for (std::vector<std::pair<int,int> >::iterator index_pair = ghosting_indices_cb.begin(); index_pair != ghosting_indices_cb.end(); ++index_pair) {
			//BOOST_FOREACH(std::pair<int,int> index_pair, ghosting_indices) {
			cells[index_pair->first] = cells[index_pair->second];
		}
	}
}
template<typename T, typename MTraits=MultivectorTraits<T> >
BucketSort<T,MTraits>::iterator BucketSort<T,MTraits>::get_broadphase_neighbours(const BucketSort<T,MTraits>::Vect3d& r) {
	return iterator(*this,r);
}

template<typename T, typename MTraits=MultivectorTraits<T> >
BucketSort<T,MTraits>::Vect3d BucketSort<T,MTraits>::correct_position_for_periodicity(const BucketSort<T,MTraits>::Vect3d& source_r, const BucketSort<T,MTraits>::Vect3d& to_correct_r) {
	Vect3d corrected_r = to_correct_r - source_r;
	for (int i = 0; i < NDIM; ++i) {
		if (!periodic[i]) continue;
		if (corrected_r[i] > cell_size[i]) corrected_r[i] -= domain_size[i];
		if (corrected_r[i] < -cell_size[i]) corrected_r[i] += domain_size[i];
	}
	return corrected_r + source_r;
}

template<typename T, typename MTraits=MultivectorTraits<T> >
BucketSort<T,MTraits>::Vect3d BucketSort<T,MTraits>::correct_position_for_periodicity(const BucketSort<T,MTraits>::Vect3d& to_correct_r) {
	Vect3d corrected_r = to_correct_r;
	for (int i = 0; i < NDIM; ++i) {
		if (!periodic[i]) continue;
		while (corrected_r[i] >= high[i]) corrected_r[i] -= domain_size[i];
		while (corrected_r[i] < low[i]) corrected_r[i] += domain_size[i];
	}
	return corrected_r;
}

template<typename T, typename MTraits=MultivectorTraits<T> >
struct BucketSort<T,MTraits>::iterator {
	iterator(const int) {
		entry = CELL_EMPTY;
	}
	iterator(BucketSort<T,MTraits>& parent, const Vect3d& r):parent(parent) {
		cell_i = parent.find_cell_index(r);
		n = parent.surrounding_cell_offsets.size();
		i = 0;
		const int offset = parent.surrounding_cell_offsets[i];
		entry = parent.cells[cell_i + offset];
		while (entry == CELL_EMPTY) {
			i++;
			if (i == n) break;
			const int offset = parent.surrounding_cell_offsets[i];
			entry = parent.cells[cell_i + offset];
		}
	}
	bool operator ==(iterator b) {
		return entry==b.entry;
	}
	iterator& operator++() {
		entry = parent.linked_list[entry];
		while (entry == CELL_EMPTY) {
			i++;
			if (i == n) break;
			const int offset = parent.surrounding_cell_offsets[i];
			entry = parent.cells[cell_i + offset];
		}
		return *this;
	}

	iterator operator++(int) {
		iterator result(*this);   // make a copy for result
		++(*this);              // Now use the prefix version to do the work
		return result;          // return the copy (the old) value.
	}
	int cell_i,i,n,entry;
	BucketSort<T,MTraits>& parent;
};


}

#endif /* BUCKETSORT_H_ */
