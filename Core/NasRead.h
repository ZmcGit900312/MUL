///////////////////////////////////////////////////////////
//  NasRead.h
//  Implementation of the Class NasRead
//  Created on:      26-11ÔÂ-2018 15:53:59
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#ifndef NASREAD_H
#define NASREAD_H

#include "IMeshRead.h"

namespace Core
{
	class NasRead : public IMeshRead
	{
		public:
		virtual ~NasRead() = default;
		bool Read(const char* fileName, Mesh* mesh)override;
		bool Write(const char* fileName, Mesh* mesh)override;

	};

}
#endif
