#ifndef STRUCTS_H
#define STRUCTS_H

struct A
{
	int item00;
	int item01;
	int item02;

	void printOffsets(
		void
		);
};

struct B
{
	int item00;
	long long item01;
	int item02;

	void printOffsets(
		void
		);
};

struct C
{
	int item00;
	int item01;
	int item02;
	int item03;
	int item04;

	void printOffsets(
		void
		);
};


#endif //STRUCTS_H
