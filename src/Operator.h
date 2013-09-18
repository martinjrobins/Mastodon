/*
 * Operator.h
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
 *  Created on: 30 Oct 2012
 *      Author: robinsonm
 */

#ifndef OPERATOR_H_
#define OPERATOR_H_

#define OPERATOR_PREAMBLE \
	template<typename T, typename Traits=BaseTraits<T> >

#define FUNCTOR_OPERATOR_PREAMBLE \
	template< typename F, typename T, typename Traits=BaseTraits<T>>


#define DEFINE_TYPEDEFS \
	typedef Traits::multivector_type multivector_type; \
	typedef Traits::row_type row_type; \
	typedef Traits::element_type element_type; \
	typedef Traits::index_type index_type; \


#include <boost/timer/timer.hpp>
#include <initializer_list>
#include <memory>

namespace Mastodon {

template<typename T>
struct BaseTraits {
	typedef std::shared_ptr<T> multivector_type;
	typedef std::shared_ptr<T::value_type> row_type;
	typedef T::value_type::value_type element_type;
	typedef unsigned int index_type;

	static index_type get_number_of_rows(multivector_type v) {return v->size();}
	static index_type get_number_of_elements(row_type r) {return r->size();}
	static row_type get_row(multivector_type v, int i) {return row_type(*v[i]);}
	static element_type& get_element(row_type& v, int i) {return *v[i];}
	static element_type& get_element(multivector_type& v, int i, int j) {return get_element(get_row(i),j);}
};





template<typename T, typename Traits=BaseTraits<T> >
class Operator {
public:
	DEFINE_TYPEDEFS

	Operator();
	virtual ~Operator() {};
	void execute(multivector_type input, multivector_type output);
	std::string get_time_string() const;
	void reset();
	double get_time() const {return time;}
	friend std::ostream& operator<<( std::ostream& out, const Operator& b ) {
		b.print(out);
		return out;
	}

protected:
	virtual void reset_impl() = 0;
	virtual void execute_impl(multivector_type input, multivector_type output) = 0;
	virtual void print_impl(std::ostream& out) const = 0;

private:
	void resume_timer();
	void stop_timer();

	boost::timer::cpu_timer timer;
	double total_time;
};



template<typename T>
Operator<T>::Operator() {
	total_time = 0;
}

template<typename T>
void Operator<T>::resume_timer() {
	timer.start();
}

template<typename T>
void Operator<T>::execute(multivector_type input, multivector_type output) {
	Operator::resume_timer();
	execute_impl(input,output);
	Operator::stop_timer();
}

template<typename T>
void Operator<T>::reset() {
	reset_impl();
}

template<typename T>
void Operator<T>::stop_timer() {
	timer.stop();
	//global_timer.stop();
	const double time = (timer.elapsed().user + timer.elapsed().user)/double(1000000000);
	total_time += time;
}


template <typename T>
const std::string to_string(const T& data)
{
   std::ostringstream conv;
   conv << data;
   return conv.str();
}

template<typename T>
std::string Operator<T>::get_time_string() const {
	return "Time to execute: " + to_string(total_time) + " s";
}


}
#endif /* OPERATOR_H_ */
