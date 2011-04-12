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

#ifndef TPFIELD_H
#define TPFIELD_H


#include <tp/globals.h>
#include <tp/types.h>
#include <tp/string.h>
#include <tp/array.h>
#include <tp/referenced.h>
#include <tp/refptr.h>


#ifdef _MSC_VER
#pragma warning( disable: 4251 )
#pragma warning( disable: 4275 )
#endif

enum tpFieldState
{
    TP_FIELD_UNCHANGED = 0,
    TP_FIELD_CHANGED
};



class tpFieldConnector;


/*!
	\class tpField
	\brief is a generic property of scenegraph nodes

	
*/

class tpField : public tpReferenced {
public:

	//! standard c'tor
	tpField(const tpString& name = "unnamed") : tpReferenced(), m_name(name) {}

	//! get a name
	const tpString& getName() const { return m_name; }

protected:
	
	//! d'tor
	virtual ~tpField() {}
	
	tpString	m_name;
};


typedef tpArray< tpRefPtr<tpField> > tpFields;


/*!
	\brief A single property of a node
*/


template <class T> class tpSingleField : public tpField 
{
	//! c'tor
	tpSingleField() {};
	
	//! copy c'tor
	tpSingleField(const tpSingleField& field) {}
	
public:
		
	//! initial c'tor
	tpSingleField(T& val,const tpString& name = "noname") : tpField(name),  m_value(val) {}

	//! return stored value
	const T& getValue() const;
	
	//! set value
	void setValue(const T& val);
	
	//! assign values of other fields an dirty this one
	tpSingleField& operator = (const tpSingleField<T>&);
		
protected:
	
	T& m_value;
};



#if 0

/*!
	\brief an array of properties
*/
template <class T> class tpMultiField : public tpField
{
public:
	//! standard c'tor
	tpMultiField();
	
	//! copy c'tor
	tpMultiField(const tpMultiField& field);

	//! return of the values 
	const tpArray<T>& getValue() const;

	//! set the whole value set
	void setValue(const tpArray<T>& val);

	//! add a single value
	void addValue(const T& val);

	//! remove a value 
	void removeValue(const T& val);

	//! clean the whole value set
	void empty();

	//! return the sieze of the value set
	tpULong getSize() const;

	//! get a certain value at a position
	const T& operator[] (tpSizeT index);


protected:
	tpArray<T> m_value;
};





// -----------------------------------------------------------------------------------------------

template <class T> 
inline tpSingleField<T>::tpSingleField() : tpField()
{};

template <class T>
inline tpSingleField<T>::tpSingleField(const tpSingleField& field)
: tpField(), m_value(field.m_value)
{
};

template <class T>
inline tpSingleField<T>::tpSingleField(const T& val,
									   const tpString& name)
	: tpField(name), m_value(val)

{
};


template <class T> 
inline const T& tpSingleField<T>::getValue() const
{
	return m_value;
};

template <class T> inline void tpSingleField<T>::setValue(const T& val)
{
	m_value = val;
	evaluate();
};


template <class T> 
inline tpSingleField<T>& tpSingleField<T>::operator = (const tpSingleField<T>& v)
{
	m_value = v.m_value;
	evaluate();
	return *this;
};



// -----------------------------------------------------------------------------------------------

template <class T> inline tpMultiField<T>::tpMultiField() : tpField()
{};

template <class T> inline tpMultiField<T>::tpMultiField(const tpMultiField& field) 
: tpField(),
m_value(field.m_value)
{
};

template <class T> 
inline const tpArray<T>& tpMultiField<T>::getValue() const
{
	return m_value;
};


template <class T> 
inline void tpMultiField<T>::setValue(const tpArray<T>& val)
{
	m_value = val;
	evaluate();
};

template <class T> 
inline void tpMultiField<T>::addValue(const T& val)
{
	m_value += val;
	evaluate();
};

template <class T> 
inline void tpMultiField<T>::removeValue(const T& val)
{
	tpLong idx = m_value.find(val);
	if (idx > -1)
	{
		m_value.remove(idx);
		evaluate();
	};
};

template <class T> 
inline void tpMultiField<T>::empty()
{
	m_value.empty();
	evaluate();
};

template <class T> 
inline tpULong tpMultiField<T>::getSize() const
{
	return m_value.getSize();
};


template <class T> 
inline const T& tpMultiField<T>::operator[] (tpSizeT index)
{
	return m_value[index];
};




template <class T> 
inline tpBool operator==(const tpSingleField<T>& l, const tpSingleField<T>& r)
{
  return r.getValue() == l.getValue();
}

template <class T> 
inline tpBool operator!=(const tpSingleField<T>& l, const tpSingleField<T>& r)
{
  return !operator == (l,r);
}


template <class T> 
inline tpBool operator==(const tpMultiField<T>& l, const tpMultiField<T>& r)
{
  return r.getValue() == l.getValue();
}

template <class T> 
inline tpBool operator!=(const tpMultiField<T>& l, const tpMultiField<T>& r)
{
  return !operator == (l,r);
}




class TP_SG_API tpSFFloat : public tpSingleField<tpFloat>
{
public:		
	tpSFFloat();
};


class TP_SG_API tpSFDouble : public tpSingleField<tpDouble>
{
public:		
	tpSFDouble();
};

class TP_SG_API tpSFInt : public tpSingleField<tpInt>
{
public:		
	tpSFInt();
};

class TP_SG_API tpSFUInt : public tpSingleField<tpUInt>
{
public:		
	tpSFUInt();
};



class TP_SG_API tpSFBool : public tpSingleField<tpBool>
{
public:
	tpSFBool();
};

class TP_SG_API tpSFString : public tpSingleField<tpString>
{
public:
	tpSFString();
};


class TP_SG_API tpMFInt : public tpMultiField<tpInt>
{
public:
	tpMFInt();
};


// ----------------------------------------------------------------------------



class TP_SG_API tpSFVec3f : public tpSingleField<tpVec3f>
{
public:			
	tpSFVec3f();
};

class TP_SG_API tpSFVec3d : public tpSingleField<tpVec3d>
{
public:		
	tpSFVec3d();
};

class TP_SG_API tpSFRotation : public tpSingleField<tpVec4d>
{
public:			
	tpSFRotation();
};


class TP_SG_API tpMFVec3f : public tpMultiField<tpVec3f>
{
public:
	tpMFVec3f();
};


class TP_SG_API tpMFVec3d : public tpMultiField<tpVec3d>
{
public:
	tpMFVec3d();
};

typedef tpSingleField<tpVec4f> tpSFColor;

class TP_SG_API tpSFVec4f : public tpSingleField<tpVec4f>
{
public:			
	tpSFVec4f(){};
};

#endif

#endif


