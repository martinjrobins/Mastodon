/*
 * OperatorList.h
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

#ifndef OPERATORLIST_H_
#define OPERATORLIST_H_

#include "Operator.h"


template<typename Multivector>
class OperatorList: public Operator<Multivector> {
public:
	OperatorList() {}
	OperatorList(Operator& o) {
		list.push_back(&o);
	}
	OperatorList(std::initializer_list<Operator<Multivector>*> arg) {
		for (auto i: arg) {
			list.push_back(i);
		}

	}
	virtual ~OperatorList() {}

	static std::shared_ptr<Operator<Multivector> > New() {
		return std::shared_ptr<Operator<Multivector> >(new OperatorList());
	}

	void push_back(Operator<Multivector>* const i) {
		list.push_back(i);
	}
	void push_back(const OperatorList& i) {
		for (Operator* j: i.list) {
			list.push_back(j);
		}
	}

	std::vector<Operator<Multivector>*>& get_operators() {return list;}

protected:
	virtual void print_impl(std::ostream& out) const {
		out << "List of "<<list.size()<< " operators:"<< std::endl;
		for (auto i : list) {
			out << "\t" << *i << " ("<<i->get_time_string()<<")"<<std::endl;
		}
		out << "End list of "<<list.size()<< " operators";

	}
	virtual void execute_impl(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector> output) {
		for (auto i : list) {
			i->execute_impl(input,output);
		}
	}
	virtual void reset_impl() {
		for (auto i : list) {
			i->reset();
		}
	}

	std::vector<Operator*> list;
};

#endif /* OPERATORLIST_H_ */
