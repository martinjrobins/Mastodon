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


#include <boost/timer/timer.hpp>
#include <initializer_list>
#include <memory>

namespace Mastodon {

template<typename Multivector>
class Operator {
public:
	Operator();
	virtual ~Operator() {};
	void execute(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector> output);
	std::string get_time_string() const;
	void reset();
	double get_time() const {return time;}
	friend std::ostream& operator<<( std::ostream& out, const Operator& b ) {
		b.print(out);
		return out;
	}

protected:
	virtual void reset_impl();
	virtual void execute_impl(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector> output);
	virtual void print_impl(std::ostream& out) const;

private:
	void resume_timer();
	void stop_timer();

	boost::timer::cpu_timer timer;
	double total_time;
};



template<typename Multivector>
Operator<Multivector>::Operator() {
	total_time = 0;
}

template<typename Multivector>
void Operator<Multivector>::resume_timer() {
	timer.start();
}

template<typename Multivector>
void Operator<Multivector>::execute(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector> output) {
	Operator::resume_timer();
	execute_impl(input,output);
	Operator::stop_timer();
}

template<typename Multivector>
void Operator<Multivector>::reset() {
	reset_impl();
}

template<typename Multivector>
void Operator<Multivector>::stop_timer() {
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

template<typename Multivector>
std::string Operator<Multivector>::get_time_string() const {
	return "Time to execute: " + to_string(total_time) + " s";
}

template<typename Multivector>
void Operator<Multivector>::reset_impl() {
}

template<typename Multivector>
void Operator<Multivector>::execute_impl(std::shared_ptr<Multivector> input, std::shared_ptr<Multivector> output) {
}

template<typename Multivector>
void Operator<Multivector>::print_impl(std::ostream& out) const {
	out << "Default Operator";
}

}
#endif /* OPERATOR_H_ */
