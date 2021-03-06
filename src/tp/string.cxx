/*
 * Copyright (C) 1999-2013 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <tp/string.h>
#include <tp/hash.h>
#include <tp/log.h>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

tpInt tpString::kNotFound = -1;

//
// C'tors
//
tpString::tpString()
{
}

tpString::tpString(const char* str)
{
    set( str, tpStrLen(str) );
}

tpString::tpString(const tpString& str)
{
	if (str.getLength()) set( str.c_str() );
}


tpString::~tpString()
{
}

//////////////////////////////////////////////////////////////////////////

tpString&
tpString::set( const char* buffer, tpSizeT size )
{
	if (buffer && size)
	{
		_assign(buffer);
		_truncate(size);
	}
	return *this;
}

tpString&
tpString::set( const char* str )
{
	_assign(str);
	return *this;
}


tpString&
tpString::_assign(const char* str )
{
	if (str)
	{
		mBuffer.copy(str,tpStrLen(str)+1,0);
	} else {
		_truncate(0);
	}
	return *this;
}

tpString&
tpString::_append(const char* str )
{
	tpSizeT input_length = tpStrLen(str);

	if (input_length)
	{
		tpSizeT local_length = getLength();

		input_length++;

		mBuffer.resize(local_length + input_length);

		for (tpSizeT i = 0; i < input_length; ++i)
		{
			mBuffer[i + local_length] = str[i];
		}
	}

	return *this;
}

tpString&
tpString::_truncate(tpSizeT pos)
{
	if (pos < mBuffer.getSize())
	{
		mBuffer[pos] = '\0';
	}

	return *this;
}

tpString&
tpString::truncate( tpSizeT pos )
{
	return this->_truncate(pos);
}


//////////////////////////////////////////////////////////////////////////

tpString&
tpString::substitute( const tpChar& c, const tpChar& substc )
{
	tpChar* cptr =  this->c_str();
	for (tpSizeT i = 0; i < getLength();i++)
	{
		if (*cptr == c) *cptr = substc; cptr++;
	}

	return *this;
}

bool
tpString::isEmpty() const
{
	return ( 0 == getLength() );
}

void
tpString::empty()
{
	mBuffer.clear();
	if (mBuffer.getSize()) mBuffer[0] = '\0';
}

void
tpString::toPascal( char** buffer ) const
{
	if (*buffer == 0)
	{
		*buffer[0] = static_cast<unsigned char>(getLength());
		strncpy(buffer[1],c_str(),getLength());
	}
}

tpSizeT
tpString::getLength() const
{
	return (mBuffer.getSize()) ? tpStrLen(c_str()) : 0;
}

tpString&
tpString::append(const tpString& other)
{
	return _append( other.c_str() );
}

tpString&
tpString::append(const char* other)
{
	return _append( other);
}

tpString&
tpString::append(const char& c)
{
	if (mBuffer.getSize() == 0) {
		mBuffer.add(c);
	} else {
		mBuffer[mBuffer.getSize()-1] = c;
	}
	mBuffer.add('\0');

	return *this;
}

tpString&
tpString::prepend(const char* other)
{
	tpString result(other);
	result.append(*this);
	*this = result;
	return *this;
}

int
tpString::find(const char& c, bool fromright) const
{
	if ( isEmpty() ) return kNotFound;
	const char* _pc = (fromright) ? tpStrRChr(c_str(),c) : tpStrChr(c_str(),c);
	return (_pc) ? _pc - (const char*)c_str() : tpString::kNotFound;
}

tpString& tpString::removeFrom(const char& end, bool fromright)
{
	int idx = find(end,fromright);
	if (idx >= 0) {
		 this[idx] = '\0';
	}
	return *this;
}


bool
tpString::isUTF8(const char* str)
{
	if(!str)
		return 0;

	const unsigned char * bytes = (const unsigned char *)str;
	while(*bytes)
	{
		if(     (// ASCII
				 bytes[0] == 0x09 ||
				 bytes[0] == 0x0A ||
				 bytes[0] == 0x0D ||
				 (0x20 <= bytes[0] && bytes[0] <= 0x7E)
				 )
		   ) {
			bytes += 1;
			continue;
		}

		if(     (// non-overlong 2-byte
				 (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
				 (0x80 <= bytes[1] && bytes[1] <= 0xBF)
				 )
		   ) {
			bytes += 2;
			continue;
		}

		if((// excluding overlongs
				 bytes[0] == 0xE0 &&
				 (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
				 (0x80 <= bytes[2] && bytes[2] <= 0xBF)
				 ) ||
		   (// straight 3-byte
			((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
			 bytes[0] == 0xEE ||
			 bytes[0] == 0xEF) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			) ||
		   (// excluding surrogates
			bytes[0] == 0xED &&
			(0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			)
		   ) {
			bytes += 3;
			continue;
		}

		if((// planes 1-3
			bytes[0] == 0xF0 &&
			(0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
				 ) ||
		   (// planes 4-15
			(0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			) ||
		   (// plane 16
			bytes[0] == 0xF4 &&
			(0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			)
		   ) {
			bytes += 4;
			continue;
		}

		return 0;
	}

	return 1;
}

int
tpString::find(const char* sub) const
{
	const char *pf = tpStrStr(c_str(),(char*)sub);
	return (pf) ? pf - (const char*)c_str() : kNotFound;
}


tpString
tpString::afterLast(const char& c) const
{
	tpString _ret;
	int _pos = find(c,true);

	if (kNotFound == _pos) return *this;

	_ret = (const char*)c_str() + _pos + 1;

	return _ret;
}

tpString
tpString::afterFirst(const char& c) const
{
	tpString _ret;
	int _pos = find(c,false);

	if (0 > _pos) return *this;

	_ret = (const char*)c_str() + _pos + 1;

	return _ret;
}

tpString
tpString::beforeFirst(const char& c) const
{
	tpString _ret;
	int pos = find(c,false);
	if (pos != tpString::kNotFound) _ret.set(mBuffer.getData(),pos);
	return _ret;
}


tpString
tpString::beforeLast(const char& c) const
{
	tpString _ret;

	int pos = find(c,true);

	if (pos != tpString::kNotFound) _ret.set(mBuffer.getData(),pos);

	return _ret;
}


tpString
tpString::format(const char* format,...)
{
	tpString res;
	tpChunk buffer;

	va_list argptr;
	va_start(argptr, format);

	int count = vsnprintf(0,0,format,argptr);

	va_end(argptr);


	buffer.setSize(count + 1);

	va_start(argptr, format);

	vsnprintf(buffer.ptr<tpChar>(),count + 1,format, argptr);

	buffer.at<tpChar>(count) = '\0';

	res.set(buffer.ptr<tpChar>());

	va_end(argptr);

	return res;
}

float tpString::toFloat() const
{
	return ::atof(this->c_str());
}


int tpString::toInt() const {
	return ::atoi(this->c_str());
}

long tpString::toLong() const {
	return ::atol(this->c_str());
}

tpString
tpString::join(const tpArray<tpString> &strarr,const tpString& sep)
{
	tpString result;
    for ( tpArray<tpString>::const_iterator it = strarr.begin();
		 it != strarr.end();
		 ++it)
	{
		if ((it) != strarr.begin()) {
			result += sep;
		}
		result += (*it);
	}
	return result;
}

#if 0


tpString& tpString::append(const tpString& str)
{
	return append(str.c_str());
}

tpString& tpString::removeAfter(const tpChar& end)
{
	if (!isEmpty())
	{
		tpInt idx = find(end,false);
		if (idx > TP_NOTFOUND)
		{
			m_stringbuffer.resize(idx);
		}
	}
	return *this;
}

tpString& tpString::append(const char* rs)
{
	if (rs)
	{
		removeAfter('\0'); // sanity check - in case an intermediate '0' has been added
		const tpChar* cptr = &rs[0];
		while (cptr && *cptr) {
			m_stringbuffer.add(*cptr); cptr++;
		}
		m_stringbuffer.add(0);
	}
	return *this;
}

unsigned long tpString::getHash() const
{
	return (getLength()) ? tpGetHash((tpUChar*)m_stringbuffer.getData(),getLength()) : 0;
}










tpString tpString::between(char leftc, char rightc) const
{
	tpString _ret;

	int _rpos = find(rightc,TRUE);
	int _lpos = find(leftc,FALSE);


	// copy if right and left are not the same
	_ret = sub(_lpos + 1,_rpos - _lpos);

	return _ret;

};


void tpString::format(const char* format,...)
{
	va_list argptr;
	va_start(argptr, format);

	char _buf[TP_MAXBUFSIZE];

#ifndef _WIN32
	vsnprintf(&_buf[0],TP_MAXBUFSIZE,format, argptr);
#elif (__SYMBIAN32__)
	snprintf(&_buf[0],TP_MAXBUFSIZE,format, argptr);
#else
	_vsnprintf(&_buf[0],TP_MAXBUFSIZE,format, argptr);
#endif

	set(_buf);

	va_end(argptr);

};


tpString& tpString::operator = (const char* rs)
{
	set(rs);
	return *this;
};

tpString& tpString::operator = (const tpString& rs)
{
	return set(rs.c_str());
}

tpString& tpString::operator=( const tpWChar* rs )
{
	return set(rs);
}

tpString& tpString::operator << (const tpString& str)
{
	append(str);
	return *this;
};

tpString& tpString::operator << (int i)
{
	tpString _str;
	_str.format("%d",i);
	append(_str);
	return *this;
};


tpString& tpString::operator << (long i)
{
	tpString _str;
	_str.format("%d",i);
	append(_str);
	return *this;
};


tpString& tpString::operator << (float r)
{
	tpString _str;
	_str.format("%f",r);
	append(_str);
	return *this;
};

tpString& tpString::operator << (double r)
{
	tpString _str;
	_str.format("%lf",r);
	append(_str);
	return *this;
};


tpString& tpString::operator << (const char c)
{
	tpString _str;
	_str.format("%c",c);
	append(_str);
	return *this;
};


int tpString::find(const char* sub) const
{
	const char *pf = tpStrStr(c_str(),(char*)sub);

	return (pf) ? pf - (const char*)c_str() : TP_NOTFOUND;

}


bool tpString::contains(const tpString& str) const
{
	return find(str.c_str()) > TP_NOTFOUND;
};


tpString tpString::sub(int pos,
					   int length) const
{

	tpString _res = c_str() + pos;
	_res.m_stringbuffer[length] = '\0';

	return _res;
};

tpArray<tpString> tpString::split(const tpString& separator) const
{

	tpArray<tpString> _res;

	tpStringTokenizer _tok(*this,separator);

	while (_tok.hasTokens()) _res += _tok.nextToken();

	return _res;
}

const wchar_t* tpString::wc_str() const
{
	return m_wcharcopy.getData();
	//return 0;
}




tpVoid tpString::__verbose_dump() const
{
	tpLogMessage("%s - length: %d, size: %d, max_size: %d",
		__FUNCTION__,getLength(),m_stringbuffer.getSize(),m_stringbuffer.getMaxSize());

	const tpChar* cptr = m_stringbuffer.getData();
	for (tpSizeT i = 0; i < getLength() + 1; i++ )
	{
		tpLogMessage("%s - 0x%x '%c'",__FUNCTION__,(long)*cptr,*cptr ? *cptr : '@'); cptr++;
	}
}

void tpString::syncCStr2WStr()
{
	tpLocale2WChar( m_stringbuffer.getData(), m_wcharcopy );
}

void tpString::syncWStr2CStr()
{
	tpWChar2Locale( m_wcharcopy.getData(), m_stringbuffer );
}



//bool tpString::operator==( const tpString& rs )
//{
//	if (isEmpty() && rs.isEmpty()) return true;
//	return (0 == tpStrCmp(c_str(),rs.c_str()));
//}
// Utils //

//tpString& tpString::join(int unused,va_arg arg)
//{
//	va_list vl;
//	va_start(vl,unused);
//
//}
//

// ---------------------------------------------------------------------

/*
const char* tpStrStr(const char* hay, char* needle)
{
	return strstr(hay, needle);
};
*/



tpStringTokenizer::tpStringTokenizer(const tpString& str,
									 const tpString& delim):
	m_string(str),
	m_delim(delim)
{
};

tpString tpStringTokenizer::nextToken()
{

	tpString _res;

	int _pos = m_string.find(m_delim.c_str());

	if (_pos > TP_NOTFOUND)
	{
		_res = m_string.sub(0,_pos);

		m_string = m_string.sub(_pos + m_delim.getLength(),
			m_string.getLength() - _pos - m_delim.getLength());

	} else
	{
		_res = m_string.sub(0,m_string.getLength());
		m_string = "";
	}

	return _res;
};

bool tpStringTokenizer::hasTokens() const
{
	return !m_string.isEmpty();
}


#endif

#if defined(__SYMBIAN32__) && defined(__GCCE__)
#	include <staticlibinit_gcce.h>
#endif


TP_API tpString tpStringFormat( const tpString& format, ... )
{

	va_list argptr;
	va_start(argptr, format);

	tpArray<char> buffer;
	buffer.resize(1024);

#ifndef _WIN32
	vsnprintf(&buffer[0],buffer.getSize(),format.c_str(), argptr);
#elif (__SYMBIAN32__)
	snprintf(&buffer[0],buffer.getSize(),format.c_str(), argptr);
#else
	_vsnprintf(&buffer[0],buffer.getSize(),format.c_str(),argptr);
#endif
	va_end(argptr);

	return tpString(buffer.getData());

}

///*static*/ tpString
//tpString::format(const char *format, ... )
//{
//	va_list argptr;
//	va_start(argptr, format);

//	tpArray<char> buffer;
//	buffer.resize(1024);

//#ifndef _WIN32
//	vsnprintf(&buffer[0],buffer.getSize(),format, argptr);
//#elif (__SYMBIAN32__)
//	snprintf(&buffer[0],buffer.getSize(),format, argptr);
//#else
//	_vsnprintf(&buffer[0],buffer.getSize(),format,argptr);
//#endif
//	va_end(argptr);

//	return tpString(buffer.getData());
//}

tpString
tpString::substr(tpSizeT pos, tpSizeT len) const
{
	tpString result;
	result.set(&this->at(pos),len);
	return result;
}
