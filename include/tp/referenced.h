#ifndef TPREFERENCED_H
#define TPREFERENCED_H


#include <tp/globals.h>
#include <tp/types.h>
#include <tp/rtti.h>


class TP_API tpReferenced { 
public:

	tpReferenced();

	inline tpReferenced& operator = (const tpReferenced&);

	tpVoid popRef() const;
	tpVoid pushRef() const;

	tpSizeT getReferenceCount() const;

	TP_TYPE_DECLARE;

protected:

	virtual ~tpReferenced();

	mutable tpSizeT m_refcount;
};


inline tpReferenced& tpReferenced::operator = (const tpReferenced&) {
    return *this;
}

#endif