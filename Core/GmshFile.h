#ifndef GMSHFILE_H
#define GMSHFILE_H


#include "IMeshRead.h"
class GmshFile :public IMeshRead
{
public:
	GmshFile(){};
	virtual ~GmshFile(){};
	bool Read(const char* fileName, Mesh* mesh)override;
	bool Write(const char* fileName, Mesh* mesh)override;

};

#endif