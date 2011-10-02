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

#include "tp/camera.h"
#include "tp/matop.h"

tpCamera::tpCamera() : tpReferenced()
{
	m_projection.identity();
	m_view.identity();
}

void tpCamera::setViewLookAt( const tpVec3r& eye, const tpVec3r& target, const tpVec3r& up )
{
	tpMat44Op::lookAt(eye,target,up,m_view);
}

void tpCamera::setProjectionPerspective( const tpReal& fov, const tpReal& aspect, const tpReal& n, const tpReal& f )
{
	tpMat44Op::perspective(fov,aspect,n,f,m_projection);
}

void tpCamera::setProjectionFrustum( const tpReal& l, const tpReal& r, const tpReal& b, const tpReal& t, const tpReal& n, const tpReal& f )
{
	tpMat44Op::frustum(l,r,b,t,n,f,m_projection);
}

TP_TYPE_REGISTER(tpCamera,tpReferenced,Camera);


