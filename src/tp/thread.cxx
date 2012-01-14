/*
 * Copyright (C) 1999-2011 Hartmut Seichter
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

#include <tp/thread.h>

// Includes for Windows and *nic'es
#if defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#if !defined(WINCE)
	#include <process.h>
#else
#error "Not Implemented"
#endif

struct tpThreadHandle
{
	DWORD handle;

	static int dispatch(void* arg)
	{
		static_cast<tpThread*>(arg)->run();
		return 0;
	}
};

#elif defined(HAVE_PTHREAD_H)

#include <pthread.h>

struct tpThreadHandle
{
	pthread_t handle;

	static void* dispatch(void* arg)
	{
		static_cast<tpThread*>(arg)->run();
		return (void*)0;
	}
};

#endif

tpThread::tpThread(tpRunnable* runnable)
	: mState(kStateNew)
	, mThreadHandle(0L)
	, mRunnable(runnable)
{
}

tpThread::~tpThread()
{
	stop();
}

void
tpThread::start()
{

#if defined(_WIN32)
	mThreadHandle = new tpThreadHandle();
	CreateThread( 0, 0,
		reinterpret_cast<LPTHREAD_START_ROUTINE>(tpThreadHandle::dispatch),
		this,0,reinterpret_cast<LPDWORD>(mThreadHandle->handle));
#elif defined(HAVE_PTHREAD_H)
	mThreadHandle = new tpThreadHandle();
	pthread_create(&mThreadHandle->handle,0L,tpThreadHandle::dispatch,static_cast<void*>(this));
	mState = kStateRun;
#endif

}

void
tpThread::stop()
{
	mState = kStateStop;
}

void
tpThread::run()
{
	mRunnable->run();
}

void
tpThread::join()
{
#if defined(HAVE_PTHREAD_H)
	void *result = 0;
	pthread_join(mThreadHandle->handle, &result);
#else
#endif
}

void
tpThread::detach()
{
#if defined(HAVE_PTHREAD_H)
	void *result = 0;
	pthread_detach(mThreadHandle->handle);
#else
#endif

}


/* static */
void
tpThread::yield()
{
	sleep(0);
}

/* static */
void
tpThread::sleep(tpULong milliseconds)
{

#if defined(__unix) || defined(__APPLE__)

	timespec tmReq;
	tmReq.tv_sec = (time_t)(milliseconds / 1000);
	tmReq.tv_nsec = (milliseconds % 1000) * 1000 * 1000;

	// we're not interested in remaining time nor in return value
	nanosleep(&tmReq, (timespec *)NULL);
#elif defined(WIN32)
	Sleep(milliseconds);
#endif

}


