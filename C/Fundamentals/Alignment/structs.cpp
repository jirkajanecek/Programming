#include "structs.h"

#include <stdio.h>

void A::printOffsets(
	void
	)
{
	printf("struct size: 0x%x\n", sizeof(A));
	printf("item00: 0x%x\n", &((A*)0)->item00);
	printf("item01: 0x%x\n", &((A*)0)->item01);
	printf("item02: 0x%x\n", &((A*)0)->item02);
}

void B::printOffsets(
	void
	)
{
	printf("struct size: 0x%x\n", sizeof(B));
	printf("item00: 0x%x\n", &((B*)0)->item00);
	printf("item01: 0x%x\n", &((B*)0)->item01);
	printf("item02: 0x%x\n", &((B*)0)->item02);
}

void C::printOffsets(
	void
	)
{
	printf("struct size: 0x%x\n", sizeof(C));
	printf("item00: 0x%x\n", &((C*)0)->item00);
	printf("item01: 0x%x\n", &((C*)0)->item01);
	printf("item02: 0x%x\n", &((C*)0)->item02);
	printf("item03: 0x%x\n", &((C*)0)->item03);
	printf("item04: 0x%x\n", &((C*)0)->item04);
}