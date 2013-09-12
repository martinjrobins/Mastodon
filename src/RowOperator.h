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

namespace Mastodon {

template<typename Multivector>
class RowOperator {
public:
	RowOperator() {}
	virtual ~RowOperator() {};


protected:
	virtual void execute_impl(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector> output);
	virtual void execute_row_impl(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector::RowType> output);
	virtual void print_impl(std::ostream& out) const;
};

template<typename Multivector>
inline void Mastodon::RowOperator<Multivector>::execute_impl(
		std::shared_ptr<Multivector> input,
		std::shared_ptr<Multivector> output) {
	foreach(output) {
		execute_row_impl(input,output[i]);
	}
}

template<typename Multivector>
inline void Mastodon::RowOperator<Multivector>::print_impl(
		std::ostream& out) const {
	out << "Row Operator";
}



template<typename Multivector>
inline void Mastodon::RowOperator<Multivector>::execute_row_impl(
		std::shared_ptr<Multivector> input,
		std::shared_ptr<Multivector::RowType> output) {
}

}
#endif /* ROWOPERATOR_H_ */
