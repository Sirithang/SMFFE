#ifndef SMFFE_JOYPADINPUT_H
#define SMFEE_JOYPADINPUT_H

#include "commandinput.h"

class JoypadInput :
	public CommandInput
{
protected:
	void pollImplementation();

public:
	JoypadInput(int pNumber);
	~JoypadInput(void);
};

#endif