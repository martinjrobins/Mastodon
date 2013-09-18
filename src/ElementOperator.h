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

#include "Operator.h"

namespace Mastodon {

template< typename F, typename T, typename Traits=BaseTraits<T> >
class ElementOperator: public Operator<T,Traits> {
public:
	DEFINE_TYPEDEFS

	ElementOperator(F function, const std::string& name):function(function),name(name) {}
	virtual ~ElementOperator() {};

protected:
	virtual void execute_impl(multivector_type input, multivector_type output);
	virtual void print_impl(std::ostream& out) const;

private:
	F function;
	const std::string name;
};

template< typename F, typename T, typename Traits=BaseTraits<T> >
inline void ElementOperator<T,Traits>::execute_impl(
		multivector_type input,
		multivector_type output) {
	index_type nr = Traits::get_number_of_rows(output);
	index_type ne = Traits::get_number_of_elments(Traits::get_row(output,0));

	for (index_type i = 0; i < nr; ++i) {
		row_type output_row = Traits::get_row(output,i);
		row_type input_row = Traits::get_row(input,i);
		for (int j = 0; j < ne; ++j) {
			Traits::get_element(output_row,j) = function(Traits::get_element(input_row,j));
		}
	}
}

template< typename F, typename T, typename Traits=BaseTraits<T> >
inline void ElementOperator<T, Traits, F>::print_impl(
		std::ostream& out) const {
	out << name;
}

template< typename F, typename T, typename Traits=BaseTraits<T> >
std::shared_ptr<Operator<T,Traits> > create_element_operator(F function, const std::string name, std::shared_ptr<T> example_multi_vector) {
	return std::shared_ptr<ElementOperator<T,Traits,F> >(ElementOperator<T,Traits,F>(function,name));
}


}


#endif /* ELEMENTOPERATOR_H_ */
