#include "controlledClassA.h"

#include <stdio.h>

extern long long g_controls[];

ControlledClassA::ControlledClassA(
	void
	)
{
}

ControlledClassA::~ControlledClassA(
	void
	)
{
}

void ControlledClassA::print(
	void
	)
{
	if (g_controllingClass.check(ControllingClass::PRINT_CLASS_A))
	{
		printf("Controlled class A\n");
	}

	return;
}
