#include "stdafx.h"
#include <exception>

#ifndef ENGINEEXCEPTIONS_H
#define ENGINEEXCEPTIONS_H

class AutoStopException : public std::exception
{
public:
	int curs;
	AutoStopException(int c) : std::exception(){curs=c;};
};

#endif