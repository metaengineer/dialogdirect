#include "VarStorage.h"

SVarStorage::SVarStorage(UINT qu)
{
	varq=0;
	init=FALSE;
	if((qu>0)&&(qu<=1000))
	{
		store=new float[qu];
		for(UINT yu=0; yu<qu; yu++)
		{
			store[yu]=0.0f;
		}
		varq=qu-1;
		init=TRUE;
	}
}

BOOL SVarStorage::CheckInit()
{
	return init;
}

BOOL SVarStorage::CheckVar(UINT no)
{
	return (init&&(varq>=no));
}

BOOL SVarStorage::SetVar(UINT no, float dat)
{
	if(init&&(varq>=no))
	{
		store[no]=dat;
		return TRUE;
	}
	return FALSE;
}

float SVarStorage::GetVar(UINT no)
{
	if(init&&(varq>=no))
	{
		return store[no];
	}
	return 0.0f;
}

SVarStorage::~SVarStorage()
{
	if(init)
	{
		delete[] store;
		init=FALSE;
	}
}
