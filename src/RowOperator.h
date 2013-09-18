/*
 * RowOperator.h
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

#ifndef ROWOPERATOR_H_
#define ROWOPERATOR_H_

#include "Operator.h"

namespace Mastodon {

template<typename T, typename Traits=BaseTraits<T>, typename F>
class RowOperator: public Operator<T,Traits> {
public:
	DEFINE_TYPEDEFS

	RowOperator(F function, const std::string& name):function(function),name(name) {}
	virtual ~RowOperator() {};

protected:
	virtual void execute_impl(multivector_type input, multivector_type output);
	virtual void print_impl(std::ostream& out) const;

private:
	F function;
	const std::string name;
};

template<typename T, typename Traits=BaseTraits<T> >
inline void RowOperator<T,Traits>::execute_impl(
		multivector_type input,
		multivector_type output) {
	index_type n = Traits::get_number_of_rows(output);
	for (index_type i = 0; i < n; ++i) {
		function(input,Traits::get_row(output,i),i);
	}
}

template<typename T, typename Traits = BaseTraits<T>, typename F>
inline void RowOperator<T, Traits, F>::print_impl(
		std::ostream& out) const {
	out << name;
}

template<typename T, typename F>
std::shared_ptr<Operator<T,Traits> > create_row_operator(F function, const std::string name, std::shared_ptr<T> example_multi_vector) {
	return std::shared_ptr<RowOperator<T,Traits,F> >(RowOperator<T,Traits,F>(function,name));
}

}



#endif /* ROWOPERATOR_H_ */
