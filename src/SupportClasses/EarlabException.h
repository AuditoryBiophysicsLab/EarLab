#ifndef __EARLAB_EXCEPTION_H_INCLUDED
#define __EARLAB_EXCEPTION_H_INCLUDED
#include <iostream>
#include <exception>

class EarlabException: public std::exception
{
public:
	EarlabException(const char *FormatString, ...);
	~EarlabException() throw() {}
	const char *GetExceptionString(void) const;
	virtual const char* what() const throw()
	{
		return mExceptionString;
	}

private:
	char mExceptionString[1024];
};
#endif
