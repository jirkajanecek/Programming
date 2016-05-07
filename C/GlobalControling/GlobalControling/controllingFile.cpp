#include "controllingFile.h"

#include <stdlib.h>

ControllingClass::ControllingClass(
	void
	)
{
	m_controlValues[GLOBAL_INDEXES::PRINT_MAIN] = true;
	m_controlValues[GLOBAL_INDEXES::PRINT_CLASS_A] = false;
	m_controlValues[GLOBAL_INDEXES::PRINT_CLASS_B] = true;
}

bool ControllingClass::check(
	GLOBAL_INDEXES index
	)
{
	return (m_controlValues[index] == 0)? false : true;
}