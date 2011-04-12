/* 

    libTAP - The Architectural Playground 
    a minimalistic collaborative virtual environment framework
    
    Copyright (C) 1999-2003 Hartmut Seichter 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA


*/

#ifndef TPLIST_H
#define TPLIST_H


#ifdef _MSC_VER
// switch of warning for simple types in tpListIterator
#pragma warning(disable:4284)
#endif


#include "tpTypes.h"

/*!
	\class tpListItem
	\brief item in a list

	An item is part of a list. It stores
	pointers to the item before and after the
	regarding item
*/

template <class T> class tpListItem
{
public:
	//! default c'tor
	tpListItem(const T& v);

	//! copy c'tor
	tpListItem(const tpListItem<T>& item);
	
	//! returns the stored item
	T* get();
	//! the item in raw format
	T value;
	//! pointer to previous item
	tpListItem<T>* prev;
	//! pointer to next item
	tpListItem<T>* next;

	//! const accessor to next element
	tpListItem<T>* getNext() const;
	//! const accessor to previous element
	tpListItem<T>* getPrevious() const;

};


/*!
	\class tpListIterator
	\brief a simple iterator for the list

	Accessing all items of a list is more
	convenient through iterators. Just
	to illustrate that functionality:
	
	\code
	
	tpList<int> list;
	
	tpListIterator<int> iterator(list.getBegin());
	
	while (iterator.getItem())
	{
		cout << *iterator.getItem() << ", ";
		iterator++;
	};	
	\endcode
	
*/  
template <class T> class tpListIterator
{
public:
	//! standard c'tor
	tpListIterator(tpListItem<T>* item);

	//! copy c'tor
	tpListIterator(const tpListIterator&);

	//! returns the current item
	// T* getItem() ;

	//! returns reference to item
	T* get();
	
	//! resets the iterator to a certain item	
	void set(tpListItem<T>* item);
	
	//! operator for going up in the list
	void operator ++ (int);
	//! operator for going down in the list
	void operator -- (int);

	void operator = (const tpListIterator& rhs);


	tpBool operator == (const tpListIterator& rhs) const;
	tpBool operator != (const tpListIterator& rhs) const;

	T& operator->();

	T& operator *() const;


private:
	tpListItem<T>* m_current;
		
};

/*!
	\class tpList
	\brief a simple double chained list

	A very simple but hopefully effective template
	for a list capable of iterating and searching
	items.
*/

template <class T> class tpList {
public:

	typedef tpListIterator<T> iterator;

	//! default c'tor
	tpList();
	//! copy c'tor	
	tpList(const tpList<T>& list);
	//! d'tor
	virtual ~tpList();
	
	//! adds an item on the end of the list
	tpListItem<T>* add(const T& item);
	
	//! removes an item (does not delete it)
	tpListItem<T>* remove(const T& item);
	
	//! removes last item
	void pop();
	
	//! appends a list
	void append(const tpList<T>& list);
	//! deletes all items
	void empty();

	//! deletes all items
	void purge();

	//! check if the item is part of the list
	tpBool contains(const T& item);
	//! find item (returns -1 if it is not a member)
	tpLong find(const T& item) const;

	T& getItem(tpULong id);

	const T& getItem(tpULong id) const;


	T& operator [] (tpULong id);

	//! get the starting item
	T* getFirst() const;
	//! get last item
	T* getLast() const;
	
	//! get the head (for iterating)
	tpListIterator<T> getBegin() const;
	//! get the tail (for iterating)
	tpListIterator<T> getEnd() const;

	tpULong getSize() const;

		

private:
	tpULong m_length;
	tpListItem<T> *head;                            
	tpListItem<T> *tail;
};


// ----------------------------------------------------------------------------


template <class T> inline tpListItem<T>::tpListItem(const T& v) :
	value(v),
	prev(NULL),
	next(NULL)	
{
};

/*
template <class T> inline tpListItem<T>::tpListItem(const tpListItem<T>& item)
{
	prev = item.prev;
	next = item.next;

	
	value = item.value;
};
*/
	
template <class T> inline T* tpListItem<T>::get()
{ 
	return &value;
};


// ----------------------------------------------------------------------------

template <class T> 
inline tpListIterator<T>::tpListIterator(tpListItem<T>* item)
: m_current(item)
{	
};


template <class T> 
inline tpListIterator<T>::tpListIterator(const tpListIterator& iter)
: m_current(iter.m_current)
{
};
	
template <class T> 
inline T* tpListIterator<T>::get()
{
	return m_current ? m_current->get() : NULL;
};

	
	
template <class T> 
inline void tpListIterator<T>::set(tpListItem<T>* item)
{
	m_current = item;
};


template <class T> 
inline void tpListIterator<T>::operator ++ (int)
{
	m_current = (m_current) ? m_current->next : NULL;
};

template <class T> 
inline void tpListIterator<T>::operator -- (int)
{
	m_current = (m_current) ? m_current->prev : NULL;
}

template <class T> 
inline T& tpListIterator<T>::operator->()
{
	return *m_current->value();
};


template <class T> 
inline void tpListIterator<T>::operator = (const tpListIterator<T>& rhs)
{
	m_current = rhs.m_current;
};


template <class T> 
inline T& tpListIterator<T>::operator *() const
{
	return *m_current->get();
}


template <class T>
inline tpBool tpListIterator<T>::operator == (const tpListIterator<T>& rhs) const
{
	return m_current == rhs.m_current;
}

template <class T>
inline tpBool tpListIterator<T>::operator != (const tpListIterator<T>& rhs) const
{
	return m_current != rhs.m_current;
}




// ----------------------------------------------------------------------------

template <class T> inline tpList<T>::tpList() 
	: m_length(0),
	head(NULL),
	tail(NULL)
{
};

template <class T> inline tpList<T>::tpList(const tpList<T>& rhs) :
	head(NULL),
	tail(NULL)
{
	m_length = 0;
	append(rhs);
};


template <class T> inline tpList<T>::~tpList()
{
	// empty();
};

template <class T> inline tpListItem<T>* tpList<T>::add(const T& item)
{
	tpListItem<T>* newtail = new tpListItem<T>(item);

	if ((head == NULL) && (tail == NULL))
	{
		tail = newtail;
		head = newtail;
		// head = tail = newtail;
	} else
	{
	
		newtail->prev = tail;
		newtail->next = NULL;	

		tail->next = newtail;


		tail = newtail;
	};

	m_length++;
	return newtail;
};


template <class T> inline tpListItem<T>* tpList<T>::remove(const T& item)
{
	tpListItem<T>* _l = head;
	while (_l != NULL)
	{
		if (_l->value == item)
		{
			if (_l->next) _l->next->prev = _l->prev;
			if (_l->prev) _l->prev->next = _l->next;
			m_length--;
			return _l;
		};
		_l = _l->next;
	};
	return NULL;	
};



template <class T> inline tpVoid tpList<T>::purge()
{
	tpListItem<T>* _l = head;
	while (_l != NULL)
	{
		tpListItem<T>* _n = _l->next;

		delete _l;

		_l = _n;
	};
	
};


template <class T> inline void tpList<T>::append(const tpList<T>& l)
{
	tpListIterator<T> _i = l.getBegin();
	while (_i.get())
	{
		this->add((*_i));		
		_i++;
	}
};

template <class T> inline void tpList<T>::empty()
{
	tpListItem<T>* _l = tail;
	while (_l != NULL)
	{
		tpListItem<T>* _c = _l;
		_l = _l->prev;
		delete _c;
		// _c = NULL;	// paranoia?
	};
	m_length = 0;	
	tail = head = NULL;
}
	

template <class T> inline tpBool tpList<T>::contains(const T& item)
{
	tpListItem<T>* _l = head;
	while (_l != NULL)
	{
		if (_l->value == item) return TRUE; 
		_l = _l->next;
	};
	
	return FALSE;
};


template <class T> inline tpLong tpList<T>::find(const T& item) const
{
	tpLong ret = 0;		
	tpListItem<T>* _l = head;
	while (_l != NULL)
	{
		if (_l->value == item) return ret;
		_l = _l->next;
		ret++;
	};
	
	return -1;				
};

template <class T> inline T* tpList<T>::getFirst() const
{
	return !head ? NULL : &head->value;
};

template <class T> inline T* tpList<T>::getLast() const 
{
	return !tail ? NULL : &tail->value;
};			
	
template <class T> inline tpListIterator<T> tpList<T>::getBegin() const
{
	tpListIterator<T> _iter(head);
	return _iter;
};

template <class T> inline tpListIterator<T> tpList<T>::getEnd() const
{
	return tail->next;
}

template <class T> inline T& tpList<T>::getItem(tpULong i)
{
	tpListItem<T>* _l = head;
	if (i < m_length) for (tpULong i = 0; i < i; i++) _l = _l->next;
	return _l->value;
}


template <class T>
const T& tpList<T>::getItem( tpULong idx ) const
{
	tpListItem<T>* _l = head;
	if (idx < m_length) for (tpULong i = 0; i < i; i++) _l = _l->next;
	return _l->value;
}

template <class T> inline T& tpList<T>::operator [] (tpULong id)
{
	return getItem(id);
}

template <class T> inline tpULong tpList<T>::getSize() const
{
	return m_length;
}

template <class T> inline void tpList<T>::pop()
{
	tpListItem<T>* _l = tail;
	if (_l != NULL)
	{
		tpListItem<T>* _c = _l;
		_l = _l->prev;
		delete _c;
		m_length--;
	}
}




#endif