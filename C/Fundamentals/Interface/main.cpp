#include <stdio.h>
#include <stdlib.h>

#include "classes.h"

void allocate(
	IFace* &pContainer,
	size_t expectedSize
	)
{
	pContainer = new ImplA[3];
	return;
}

///////////////////////////////////////////////////////////////////////////////

void release(
	IFace* pContainer
	)
{
	delete[] pContainer;
	return;
}

///////////////////////////////////////////////////////////////////////////////

int main(
	int,
	char**
	)
{
	IFace* pContainer = nullptr;
	allocate(pContainer, 3);

	for (size_t i = 0; i < 3; ++i)
	{
		pContainer[i].fce();
	}

	release(pContainer);

	return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////