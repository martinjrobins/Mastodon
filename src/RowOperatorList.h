/*
 * RowOperatorList.h
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

#ifndef ROWOPERATORLIST_H_
#define ROWOPERATORLIST_H_

#include "RowOperator.h"


template<typename Multivector>
class RowOperatorList: public RowOperator<Multivector> {
public:
	RowOperatorList() {}
	RowOperatorList(RowOperator& o) {
		list.push_back(&o);
	}
	RowOperatorList(std::initializer_list<RowOperator<Multivector>*> arg) {
		for (auto i: arg) {
			list.push_back(i);
		}

	}
	virtual ~RowOperatorList() {}

	static std::shared_ptr<RowOperator<Multivector> > New() {
		return std::shared_ptr<RowOperator<Multivector> >(new RowOperatorList());
	}

	void push_back(RowOperator<Multivector>* const i) {
		list.push_back(i);
	}
	void push_back(const RowOperatorList& i) {
		for (RowOperator* j: i.list) {
			list.push_back(j);
		}
	}

	std::vector<RowOperator<Multivector>*>& get_operators() {return list;}

protected:
	virtual void print_impl(std::ostream& out) const {
		out << "List of "<<list.size()<< " row operators:"<< std::endl;
		for (auto i : list) {
			out << "\t" << *i << " ("<<i->get_time_string()<<")"<<std::endl;
		}
		out << "End list of "<<list.size()<< " row operators";

	}
	virtual void execute_impl(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector> output) {
		ASSERT(input.size()==output.size());
		foreach(input) {
			for (auto j : list) {
				i->execute_row_impl(input[i],output[i]);
			}
		}
	}

	virtual void reset_impl() {
		for (auto i : list) {
			i->reset();
		}
	}

	std::vector<RowOperator*> list;
};


#endif /* ROWOPERATORLIST_H_ */
