/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website.
 *
 */

#ifndef TPFIXEDPOINT_H
#define TPFIXEDPOINT_H

/**
 * \file tpFixed.h
 *
 * Provides a wrapper around floating point to fixed point conversion which
 * is mostly needed on mobile devices.
 */

#include <tp/globals.h>
#include <tp/types.h>
#include <tp/math.h>


typedef tpLong tpFixedType; //<! type definition for internal fixed point rep

#define TP_INT_TO_FIXED(x)         ((x) << 16)
#define TP_DOUBLE_TO_FIXED(x)      ((tpLong)(x * 65536.0 + 0.5))
#define TP_FIXED_TO_INT(x)         ((x) >> 16)
#define TP_FIXED_TO_DOUBLE(x)      (((tpDouble)x) / 65536.0)
#define TP_ROUND_FIXED_TO_INT(x)   (((x) + 0x8000) >> 16)

#define TP_FIXED_ONE       TP_INT_TO_FIXED(1)
#define TP_FIXED_PI        205887L
#define TP_FIXED_2PI       411775L
#define TP_FIXED_E         178144L
#define TP_FIXED_ROOT2      74804L
#define TP_FIXED_ROOT3     113512L
#define TP_FIXED_GOLDEN    106039L


template <typename T>
class tpFixed {
	
	T m;
	
public:
	
	tpFixed() : m(T(0)) {}
	tpFixed(const float& f) { set(f); }
	tpFixed(const double& d) { set(d); }
	tpFixed(const int& i) { set(i); }
	
	tpFixed(const tpFixed& rhv) { m = rhv.m; }
	
	const static int _bits = sizeof(T) << 3;
	const static int _bits_half  = sizeof(T) << 2;
	const static int _bits_quarter  = sizeof(T) << 1;
	const static int _bits_half_val  = (1 << _bits_half);
	const static int _bits_quarter_val  = (1 << _bits_quarter);
	
	void set(const int& v) { m = v << _bits_half; }
	void set(const double& v) { m = (T)(v * _bits_half_val + 0.5); }
	void set(const float& v) { m = (T)(v * _bits_half_val + 0.5f); }
	
	void get(double& v) const { v = ((double) m) / (double)_bits_half_val; }
	void get(float& v) const { v = ((float) m) / (float)_bits_half_val; }
	void get(int& v) const { v = m >> _bits_half; }
	
	int getInt() const { int v; get(v); return v; }
	int getIntRounded() const {  return (m + _bits_quarter_val) >> _bits_half; }
	
	double getDouble() const { double v; get(v); return v;  }
	float getFloat() const { float v; get(v); return v;  }
	
	tpFixed<T>& mul(const tpFixed<T>& r) { m = ((tpInt64)m * r.m) >> r._bits_half;  return *this; }
	tpFixed<T>& div(const tpFixed<T>& r) { m = ((tpInt64)m << r._bits_half) / r.m;  return *this; }
	tpFixed<T>& add(const tpFixed<T>& r) { m += r.m; return *this; }
	tpFixed<T>& sub(const tpFixed<T>& r) { m -= r.m; return *this; }
	tpFixed<T>& sqrt() {set(tpSqrt(getFloat())); return *this; }
	

	tpFixed<T>& operator  = (const tpFixed<T>& rhv) { this->m = rhv.m; return *this; }
	tpFixed<T>& operator *= (const tpFixed<T>& rhv) { return mul(rhv); }
	tpFixed<T>& operator /= (const tpFixed<T>& rhv) { return div(rhv); }
	tpFixed<T>& operator += (const tpFixed<T>& rhv) { return add(rhv); }
	tpFixed<T>& operator -= (const tpFixed<T>& rhv) { return add(rhv); }
	
	bool operator < (const tpFixed<T>& rhv) { return (m < rhv.m); }
	bool operator > (const tpFixed<T>& rhv) { return (m > rhv.m); }
	bool operator == (const tpFixed<T>& rhv) { return (m == rhv.m); }
	
	T& getX() { return m; }
	const T& getX() const { return m; }
	void setX(const T& val) { m = val; }
	
	
	//int operator = (const tpFixed<T>& rhv) { return getInt(); }
	
	//bool operator < (const int& rhv) { return (m < tpFixed<T>(rhv).m); }
	
	
};


// operators
template <typename T> 
tpFixed<T> operator * (const tpFixed<T>& a, const tpFixed<T>& b) 
{
	tpFixed<T> res(a); res *= b; return res;
}

template <typename T> 
tpFixed<T> operator + (const tpFixed<T>& a, const tpFixed<T>& b) 
{
	tpFixed<T> res(a); res += b; return res;
}

template <typename T> 
tpFixed<T> operator - (const tpFixed<T>& a, const tpFixed<T>& b) 
{
	tpFixed<T> res(a); res -= b; return res;
}

template <typename T> 
tpFixed<T> operator / (const tpFixed<T>& nom, const tpFixed<T>& den) 
{
	tpFixed<T> res(nom); res /= den; return res;
}


// math overload
template<typename T> inline tpFixed<T> sqrt(const tpFixed<T>& in)
{
	tpFixed<T> res(in); res.sqrt(); return res; 
}

template<typename T> inline tpFixed<T> acos(const tpFixed<T>& in)
{
	tpFixed<T> res(acos(in.getFloat())); return res; 
}

template<typename T> inline tpFixed<T> asin(const tpFixed<T>& in)
{
	tpFixed<T> res(asin(in.getFloat())); return res; 
}

template<typename T> inline tpFixed<T> cos(const tpFixed<T>& in)
{
	tpFixed<T> res(cos(in.getFloat())); return res; 
}

template<typename T> inline tpFixed<T> sin(const tpFixed<T>& in)
{
	tpFixed<T> res(sin(in.getFloat())); return res; 
}

#if 0

// implementations
template <typename T>
tpFixed<T>& tpFixed<T>::sqrt() 
{
	
	
	T g = 0;
	T bshft = (_bits_half - 1); // 15
	T b = 1 << bshft;
	do { 
		T temp = (g + g + b) << bshft;
		if (m >= temp)
		{
			g += b; 
			m -= temp;
		}
		b >>= 1;
	} while (bshft--);
	
	m = g;
	
	#elif 0
	
	T res(0);
	T op(m);
	T one(0);
	
	/* "one" starts at the highest power of four <= than the argument. */
	one = 1 << (_bits - 2);	/* was 30 second-to-top bit set */
	while (one > op) one >>= 2;
	while (one != 0) 
	{
		if (op >= res + one) 
		{
			op = op - (res + one);
			res = res +  (one<<1);
		}
		res >>= 1;
		one >>= 2;
	}
	
	m = res;
	
	return *this;
}

#endif


/*
ui32 g = 0;
ui32 bshft = 15;
ui32 b = 1<<bshift;
do {
	ui32 temp = (g+g+b)<<bshft;
	if (value >= temp) {
		g += b;
		value -= temp;
	}
	b>>=1;
} while (bshft--);

return g;
*/

typedef tpFixed<tpInt32> tpFixed32;


#endif
