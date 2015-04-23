#include "stdafx.h"

#ifndef VARSTORAGE_H
#define VARSTORAGE_H

class SVarStorage
{
private:
	float *store;
	UINT varq;
	BOOL init;
public:
	SVarStorage(UINT qu);
	BOOL SetVar(UINT no, float dat);
	BOOL CheckVar(UINT no);
	BOOL CheckInit();
	float GetVar(UINT no);
	~SVarStorage();
};

#endif