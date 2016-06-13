#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

int main(
	int argc,
	char** argv
	)
{
	A().printOffsets();
	B().printOffsets();
	C().printOffsets();

	C serialized[3];
	for (int i = 0; i < 3; ++i)
	{
		printf("0x%llx ", (long long)&serialized[i]);
	}
	printf("\n");

	return EXIT_SUCCESS;
}