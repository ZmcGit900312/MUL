///////////////////////////////////////////////////////////
//  IMeshRead.h
//  Implementation of the Interface IMeshRead
//  Created on:      26-11ÔÂ-2018 15:53:51
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#ifndef IMESHREAD_H
#define IMESHREAD_H

#include "Mesh.h"

namespace Core
{
	__interface IMeshRead
	{
		virtual bool Read(const char* fileName, Mesh* mesh) =0;
		virtual bool Write(const char* fileName, Mesh* mesh) =0;
	};


}
#endif