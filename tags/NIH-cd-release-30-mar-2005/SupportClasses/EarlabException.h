#ifndef __EARLAB_EXCEPTION_H_INCLUDED
#define __EARLAB_EXCEPTION_H_INCLUDED
class EarlabException
{
public:
	EarlabException(const char *FormatString, ...);
	~EarlabException();
	const char *GetExceptionString(void) const;
private:
	char mExceptionString[1024];
};
#endif
