#pragma once
#ifndef _CONTROLLED_CLASS_B_H_
#define _CONTROLLED_CLASS_B_H_

#include "controllingFile.h"
#include "controlledClassA.h"

class ControlledClassB :
	public ControlledClassA
{

public:
	
	ControlledClassB(
		void
		);

	~ControlledClassB(
		void
		);
};

#endif //_CONTROLLED_CLASS_B_H_

