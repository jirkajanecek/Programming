#include <iostream>

#include "rValue_moveConstructor.h"
#include "constexpr.h"

int main(
	int argc, 
	char** argv
)
{
	RValueAndMoveConstructor::process();
	Constexpr::process();	
	return EXIT_SUCCESS;
}