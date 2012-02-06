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

#ifndef TP_RENDERABLE_H
#define TP_RENDERABLE_H

#include <tp/node.h>
#include <tp/material.h>
#include <tp/texture.h>

class TP_API tpRenderable : public tpNode
{
public:

	TP_TYPE_DECLARE

	//! c'tor
	tpRenderable(const tpString& name = "");
	//! copy c'tor
	tpRenderable(const tpRenderable&);

	//! cloning interface
	virtual tpObject* clone();

	//! set a material for the regarding geometry
	void setMaterial(tpMaterial* material) { mMaterial = material; }
	//! get a pointer to the material
	const tpMaterial* getMaterial() const { return mMaterial.get(); }
	tpMaterial* getMaterial() { return mMaterial.get(); }

	//! set a texture for the regarding geometry
	void setTexture(tpTexture* texture) { mTexture = texture; }
	//! get a pointer to the texture
	const tpTexture* getTexture() const { return mTexture.get(); }
	tpTexture* getTexture() { return mTexture.get(); }

	//! check if this material has a texture
	bool hasTexture() const { return mTexture.isValid(); }

	//! set if this material is renderable is for culling only
	void setCulling(bool flag) { mCulling = flag; }
	bool getCulling() const { return mCulling; }


protected:

	tpRefPtr<tpMaterial>	mMaterial;
	tpRefPtr<tpTexture>		mTexture;

	bool		mCulling;

};


#endif
