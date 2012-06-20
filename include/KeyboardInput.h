#ifndef SMFFE_KEYBOARDINPUT_H
#define SMFFE_KEYBOARDINPUT_H

#include "CommandInput.h"
#include <map>

class KeyboardInput : public CommandInput
{
protected:
	void pollImplementation();

public:
	KeyboardInput();
	~KeyboardInput();
};

#endif

