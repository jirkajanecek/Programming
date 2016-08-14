#include <iostream>

#include "rValue_moveConstructor.h"
#include "constexpr.h"
#include "initializer_list.h"
#include "type_inference.h"

int main(
	int argc, 
	char** argv
)
{
	RValueAndMoveConstructor::process();
	Constexpr::process();
	InitializerList::process();
	TypeInference::process();
	return EXIT_SUCCESS;
}