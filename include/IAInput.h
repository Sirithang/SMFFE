#ifndef SMFFE_IAINPUT_H
#define SMFFE_IAINPUT_H

#include "commandinput.h"

class IAInput :
	public CommandInput
{
protected:
	void pollImplementation();

public:
	IAInput(void);
	~IAInput(void);
};

#endif

