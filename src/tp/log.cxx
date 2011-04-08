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
#include <tp/log.h>
#include <tp/referenced.h>
#include <tp/version.h>

#include <tp/string.h>
#include <tp/system.h>


#include <stdarg.h>
#include <stdio.h>


class tpFileLog : public tpReferenced {
	FILE* m_file;
public:
	tpFileLog() : m_file(0) {
		m_file = fopen("twistedpair.txt","a+");
	}

	~tpFileLog() {
		if (m_file) fclose(m_file);		
	}

	void log(const char* stuff) {
		if (m_file) fprintf(m_file,"%s",stuff);
	}
	
	static tpFileLog& get() {
		static tpFileLog thefilelog;
		return thefilelog;
	}

};

void tpFileLogFunction(const char* stuff) {
	tpFileLog::get().log(stuff);
}



void tpDefaultLogFunction(const char* stuff) {	
	fprintf(stdout,"%s",stuff);
}

void tpSetGlobalNotifyLevel(tpUInt level)
{
	tpLog::get().setLevel(level);
}


#if defined(_MSC_VER)
#define		_CRT_SECURE_NO_WARNINGS 1
#endif

#if defined(WIN32) || defined(WINCE)
    #if defined (GNUWIN32)
    #define tpVSNPRINTF vsnprintf
    #else
    #define tpVSNPRINTF _vsnprintf
    #endif
#elif defined(ANDROID)
	#define tpVSNPRINTF snprintf
#elif defined(__unix) || defined(__APPLE__)
    #define tpVSNPRINTF vsnprintf
#elif defined(__SYMBIAN32__)
	#define tpVSNPRINTF snprintf
#endif

#define LOG_BUFFER_SIZE   (8192)

static char s_szBuf[LOG_BUFFER_SIZE];




void tpLogNotify(const char* szFormat, ...) {

   va_list argptr;

   va_start(argptr, szFormat);
   tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

   va_end(argptr);

   tpLog::get().log(TP_LOG_NOTIFY,s_szBuf);

}


void tpLogMessage(const char* szFormat, ...) {

   va_list argptr;
   va_start(argptr, szFormat);
   tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

   va_end(argptr);

   tpLog::get().log(TP_LOG_MESSAGE,s_szBuf);

}


void tpLogError(const char* szFormat, ... ) {

    va_list argptr;
    va_start(argptr, szFormat);
    tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

    va_end(argptr);

    tpLog::get().log(TP_LOG_ERROR,s_szBuf);
}


void tpLogProgress(const char* szFormat, ... ) {

    va_list argptr;
    va_start(argptr, szFormat);
    tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

    va_end(argptr);

    tpLog::get().log(TP_LOG_NOTIFY,s_szBuf,0);
}

////////////////////////////////////////////////////////////////////////////////

tpLog::tpLog() :
	m_level(TP_LOG_NOTIFY),
	m_func(0)
{
	m_func = 
#if defined(WINCE)
		&tpFileLogFunction;
#else
		&tpDefaultLogFunction;
#endif
	
	this->log(TP_LOG_NOTIFY,tpGetVersionString(), 1);
	
}

tpLog::~tpLog() {
}

void tpLog::log(tpLogLevelType logtype, const char* buf, int endline)
{
	if (logtype > m_level) return;
	
	if (m_func) {
		tpString t = tpSystem::get()->getTime();
		m_func(t.c_str());
		m_func(" ");
		m_func(buf);
		if (endline) m_func("\n");
	}
}

void tpLog::printf(tpUInt logtype, const char* szFormat, ...) 
{

	if (logtype >= m_level) return;

   va_list argptr;
   va_start(argptr, szFormat);
   tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

   va_end(argptr);

   tpLog::get().log((tpLogLevelType)logtype,s_szBuf);

}



/* static */
tpLog& tpLog::get() {
	static tpLog the_log;
	return the_log;	
}

void tpLog::setBackend( tpLogFunc func) {
	m_func = func;
}
