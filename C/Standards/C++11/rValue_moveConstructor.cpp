#include "rValue_moveConstructor.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////

RValueAndMoveConstructor::RValueAndMoveConstructor()
{
}

///////////////////////////////////////////////////////////////////////////////

RValueAndMoveConstructor::~RValueAndMoveConstructor()
{
}

///////////////////////////////////////////////////////////////////////////////

void RValueAndMoveConstructor::process(
	void
)
{
	rValueAndlValue();
}

///////////////////////////////////////////////////////////////////////////////

void RValueAndMoveConstructor::rValueAndlValue(
	void
)
{
	/*
	"An lvalue is an expression e that may appear on the left 
	or on the right hand side of an assignment, whereas an 
	rvalue is an expression that can only appear on the right
	hand side of an assignment"
	*/
	
	//move semantics
	int a = 42;
	int b = 43;

	//C - lvalue, (a+b) - rvalue
	int c = a + b;	
}

///////////////////////////////////////////////////////////////////////////////

void RValueAndMoveConstructor::rValueReference(
	void
)
{
	Helper obj;
	
	RValueAndMoveConstructor::helper(obj);	//lValue reference
	RValueAndMoveConstructor::helper(RValueAndMoveConstructor::foobar());	//rValue reference
}

///////////////////////////////////////////////////////////////////////////////

void RValueAndMoveConstructor::helper(
	Helper& helper
) 
{
	std::cout << "lValue reference is called";	
}

///////////////////////////////////////////////////////////////////////////////

void RValueAndMoveConstructor::helper(
	Helper&& helper
) 
{
	std::cout << "rValue reference is called";
}

///////////////////////////////////////////////////////////////////////////////

RValueAndMoveConstructor::Helper RValueAndMoveConstructor::foobar(
	void
)
{
	Helper tmp;
	return tmp;
}
