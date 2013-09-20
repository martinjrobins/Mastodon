/*
 * Vector.h
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
 *  Created on: 20 Sep 2013
 *      Author: robinsonm
 */

#ifndef VECTOR_H_
#define VECTOR_H_

namespace Mastodon {

template<typename T>
class Vector {
public:
	Vector() {}
	Vector(T x, T y, T z):data({x,y,z}){}
	inline T& operator[](unsigned int i) {
		return data[i];
	}
	inline T max() {
		return std::max(data[0],std::max(data[1],data[2]));
	}

private:
	T data[3];
};

template<typename T>
inline Vector<T> operator*(T a, Vector b) {
	Vector ret;
	ret[0] = a*b[0];
	ret[1] = a*b[1];
	ret[2] = a*b[2];
	return ret;
}

template<typename T>
inline Vector<T> operator*(Vector a, T b) {
	Vector ret;
	ret[0] = b*a[0];
	ret[1] = b*a[1];
	ret[2] = b*a[2];
	return ret;
}

template<typename T>
inline Vector<T> operator-(Vector a, Vector b) {
	Vector ret;
	ret[0] = a[0]-b[0];
	ret[1] = a[1]-b[1];
	ret[2] = a[2]-b[2];
	return ret;
}

template<typename T>
inline Vector<T> operator+(Vector<T> a, Vector<T> b) {
	Vector<T> ret;
	ret[0] = a[0]+b[0];
	ret[1] = a[1]+b[1];
	ret[2] = a[2]+b[2];
	return ret;
}

} /* namespace Mastodon */
#endif /* VECTOR_H_ */
