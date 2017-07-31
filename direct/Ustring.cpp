#include "Ustring.h"

std::string GlobalDoubleAsString(double m, unsigned short precision)
{
	bool bNegative=false;
	const int maxintlength = 8;
	if(precision>20)
		return std::string("");
	long long mvalue=1;
	unsigned int ef;
	for (ef=1;ef<=precision;ef++)
		mvalue=mvalue*(long long)10;
	long long indata=long long(m*mvalue);
	long long a=indata;
	if(a<0)
	{
		bNegative=true;
		a=-a;
	}
	int b=0;
	char *buffer;
	char *strint;
	buffer = new char[2];
	buffer[1] = '\0';
	strint = new char[precision+maxintlength+1];
	strint[precision+maxintlength]='\0';
	short int i=1;
	while(a!=0)
	{
		b=int(a%10);
		a=a/10;
		strint[i]=char(b+48);
		if(i>=precision+maxintlength)
			break;
		i++;
	}
	while(i<precision+1)
	{
		strint[i]=char(48);
		i++;
	}
	short int p=0;
	short int erp;
	while(p<=precision)
	{
		erp=p;
		strint[erp]=strint[++p];
	}
	strint[precision]=char(46);
	if(bNegative)
	{
		strint[i]=char(45);
		i++;
	}
	std::string alpha = "";
	while(i>0)
	{
		i--;
		buffer[0]=strint[i];
		alpha.append(buffer);
	}
	delete[] buffer;
	delete[] strint;
	return alpha;
}

std::string GlobalUINTAsString(UINT indata)
{
	UINT s=indata;
	UINT lng=s?0:1;
	while(s!=0)
	{
		s=s/10;
		lng++;
	}
	UINT a=indata;
	UINT b=0;
	char *strint = new char[lng+1];
	strint[lng]='\0';
	UINT i=lng-1;
	while(a)
	{
		b=a%10;
		a=a/10;
		strint[i]=char(b+48);
		if((!i)&&a)
		{
			delete[] strint;
			return std::string("");
		}
		i--;
	}
	if(!indata)
		strint[0]='0';
	std::string jh = std::string(strint);
	delete[] strint;
	return jh;
}
