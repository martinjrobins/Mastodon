/*
 * ElementOperator.h
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
 *  Created on: 12 Sep 2013
 *      Author: robinsonm
 */

#ifndef ELEMENTOPERATOR_H_
#define ELEMENTOPERATOR_H_

#include "RowOperator.h"

namespace Mastodon {

template<typename T, typename Traits=BaseTraits<T> >
class ElementOperator: public RowOperator<T,Traits> {
public:
	typedef Traits::multivector_type multivector_type;
	typedef Traits::row_type row_type;
	typedef Traits::element_type element_type;
	typedef Traits::index_type index_type;

	ElementOperator() {}
	virtual ~ElementOperator() {};


protected:
	virtual void execute_row_impl(multivector_type input, row_type output, index_type i);
	virtual void execute_element_impl(element_type input, element_type& output) = 0;
	virtual void print_impl(std::ostream& out) const = 0;
};




template<typename Multivector>
inline void Mastodon::ElementRowOperator<Multivector>::execute_row_impl(
		multivector_type input,
		row_type output,
		index_type i) {
	index_type n = Traits::get_number_of_elements(output);
	for (index_type j = 0; j < n; ++j) {
		row_type row = Traits::get_row(output,j);
		execute_element_impl(Traits::get_element(input,i,j),Traits::get_element(row,j));
	}
}

}


#endif /* ELEMENTOPERATOR_H_ */
