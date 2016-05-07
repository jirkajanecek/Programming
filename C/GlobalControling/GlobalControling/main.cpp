#include <stdio.h>
#include <stdlib.h>

#include "controllingFile.h"
#include "controlledClassA.h"
#include "controlledClassB.h"

int main(int args, char** argv)
{
	if (g_controllingClass.check(ControllingClass::PRINT_MAIN))
	{
		printf("main stuff\n");
	}

	ControlledClassA* pA = new ControlledClassA();
	pA->print();

	ControlledClassB* pB = new ControlledClassB();
	pB->print();

	return 0;
}

