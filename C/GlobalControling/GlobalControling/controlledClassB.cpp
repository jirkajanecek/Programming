#include "controlledClassB.h"

#include <stdio.h>

ControlledClassB::ControlledClassB()
{
}


ControlledClassB::~ControlledClassB()
{
}

void ControlledClassB::print(
	void
	)
{
	if (g_controllingClass.check(ControllingClass::PRINT_CLASS_A))
	{
		printf("Controlled class B\n");
	}

	return;
}
