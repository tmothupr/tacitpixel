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


#include <tp/imagehandler.h>
#include <tp/image.h>

#include <tp/log.h>
#include <tp/module.h>
#include <tp/version.h>


class tpImageHandler_BMP : public tpImageHandler {
public:

    TP_TYPE_DECLARE

	bool getCapability(tpUInt capability,const tpString& name);

	tpImageHandler_BMP();

    virtual tpImage* read(const tpString& name) const;
};

//////////////////////////////////////////////////////////////////////////

tpImageHandler_BMP::tpImageHandler_BMP() : tpImageHandler()
{
	tpLogNotify("%s BMP image support",tpGetVersionString());
}

bool tpImageHandler_BMP::getCapability( tpUInt capability,const tpString& name )
{
	switch (capability)
	{
		case kCanRead:
			return name.afterLast('.') == "bmp";
			break;
		default:
			break;
	}
	return false;
}

tpImage* tpImageHandler_BMP::read( const tpString& name ) const
{
	tpUInt _uival;
	tpUShort _usval;

	tpUInt width(0),height(0);
	tpUInt buffersize;
	tpUByte* buffer;

	FILE* _file = fopen(name.c_str(),"rb");

	if (!_file) {
		tpLogError("File %s does not exist",name.c_str());
		return 0;
	};

	fseek(_file,18,SEEK_SET);

	fread((char*)&_uival,4,1,_file);	
	width = _uival;

	fread((char*)&_uival,4,1,_file);	
	height = _uival;

	buffersize = 3 * width * height;
	buffer = (tpUByte*)malloc(buffersize);

	fread((char*)&_usval,1,2,_file);

	if (_usval != 1)
	{
		tpLogError("File has %s unsupported %d planes (%dx%d)\n",
			name.c_str(),
			_usval,
			width,
			height);

		fclose(_file);

		return 0;
	};


	fread((char*)&_usval,1,2,_file);

	if (24 != _usval) {


		tpLogError("File has %s unsupported %d bpp (%dx%d)\n",
			name.c_str(),
			_usval,
			width,
			height);

		fclose(_file);
		return 0;
	};

	fseek(_file,24,SEEK_CUR);

	tpUInt _res = fread(buffer,1,buffersize,_file);

	if (_res != buffersize) 
	{
		fclose(_file);
		return 0;
	};

	for (tpUInt i = 0; i < buffersize; i += 3)
	{
		tpUInt temp = buffer[i];
		buffer[i] = buffer[i+2];
		buffer[i+2] = temp;
	}

	fclose(_file);

	tpImage* img = new tpImage();
	img->allocate(width,height,tpPixelFormat::kRGB_888);
	img->assign(buffer);

	free(buffer);

	return img;
}


TP_TYPE_REGISTER(tpImageHandler_BMP,tpImageHandler,ImageFactoryBMP);

tpModuleInitializer<tpImageHandler_BMP> g_imagefactory_bmp;

