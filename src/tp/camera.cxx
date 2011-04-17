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
#include "tp/camera.h"

tpCamera::tpCamera() : tpReferenced()
{
	m_projection.setIdentity();
	m_view.setIdentity();

}

void tpCamera::setViewLookAt( const tpVec3r& eye, const tpVec3r& target, const tpVec3r& up )
{
	m_view.lookAt(eye,target,up);
}

void tpCamera::setProjectionPerspective( const tpReal& fov, const tpReal& aspect, const tpReal& n, const tpReal& f )
{
	m_projection.setPerspective(fov,aspect,n,f);
	m_projection.transpose();
	m_projection.invert();
}

void tpCamera::setProjectionFrustum( const tpReal& l, const tpReal& r, const tpReal& b, const tpReal& t, const tpReal& n, const tpReal& f )
{
	m_projection.setFrustum(l,r,b,t,n,f);
	m_projection.transpose();
	m_projection.invert();
}

TP_TYPE_REGISTER(tpCamera,tpReferenced,Camera);


