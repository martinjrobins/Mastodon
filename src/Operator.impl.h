/*
 * Operator.cpp
 *
 *  Created on: 30 Oct 2012
 *      Author: robinsonm
 */

#include "Operator.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

namespace Mastodon {

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



