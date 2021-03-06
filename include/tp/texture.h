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

#ifndef TPTEXTURE_H
#define TPTEXTURE_H


#include <tp/object.h>
#include <tp/image.h>
#include <tp/vec.h>

//enum tpTextureWrap
//{
//	TP_TEXTURE_CLAMP,
//	TP_TEXTURE_REPEAT
//};


class tpTextureObject;

/*!
	\class tpTexture
	\author Hartmut Seichter
	\brief Provides support for bitmap textures.

	Adds support for textures to the scenegraph.

*/


class TP_API tpTexture : public tpObject
{
public:

	TP_TYPE_DECLARE

	enum {
		kWrapModeClamp,
		kWrapModeRepeat
	};

	enum {
		kFilterNearest		= (1 << 1),
		kFilterLinear		= (1 << 2),
		kFilterMipMapNearest= (1 << 3),
		kFilterMipMapLinear	= (1 << 4)
	};

	enum {

		kFormatAlpha,
		kFormatLuminance,
		kFormatRGB,
		kFormatBGR,
		kFormatRGBA,
		kFormatBGRA
	};

	enum {
		kDownSampling  = 0,
		kUpSampling
	};

	tpTexture(const tpString& name = "");

	tpImage* getImage() { return mImage.get(); }
	const tpImage* getImage() const { return mImage.get(); }


	void setImage(tpImage* image);

	void setWrapMode(tpUInt wrap_u,tpUInt wrap_v = kWrapModeClamp, tpUInt wrap_w = kWrapModeClamp);
	tpVec3<tpUInt> getWrapMode() const;

	void setFormat(tpUInt format) { mFormat = format; }
	tpUInt getFormat() const { return mFormat; }

	void setFilter(tpUInt filter,tpUByte updown = kDownSampling) { (updown) ? mMinFilter = filter : mMagFilter = filter; }
	tpUInt getFilter(tpUByte updown = kDownSampling) const { return ((updown) ? mMinFilter : mMagFilter) ; }
	bool hasFilter(tpUInt filter,tpUByte updown = kDownSampling) const
	{ return (0 != (getFilter(updown) & filter)); }

	void setTextureObject(tpTextureObject* object) { mObject = object; }

	const tpTextureObject* getTextureObject() const { return mObject.get(); }
	tpTextureObject* getTextureObject() { return mObject.get(); }

	void setStatic(bool s = true) { mStatic = s; }
	bool getStatic() const { return mStatic; }

protected:

	virtual ~tpTexture();

	tpUInt mFormat;

	tpUInt mMinFilter;
	tpUInt mMagFilter;

	tpVec3<tpUInt> mWrap;
	bool mStatic;

	tpRefPtr<tpImage> mImage;
	tpRefPtr<tpTextureObject> mObject;

};


class tpTextureObject : public tpReferenced {
public:

	virtual void create(tpTexture& texture) = 0;

	virtual void update(tpTexture& texture) = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void destroy() = 0;

	//@ returns the size the machine will understand (such as NPOT)
	virtual tpVec2i getSize() const { return tpVec2i(0,0); }

protected:

	virtual ~tpTextureObject() {}

};


#endif
