#ifndef _CONTROLLING_FILE_H_
#define _CONTROLLING_FILE_H_

enum GLOBAL_INDEXES
{
	FIRST_ATTRIBUTE = 0,
	SECOND_ATTRIBUTE = 1,
	INDEXES_SIZE
};

//main controlling array
static long long g_controls[INDEXES_SIZE];

void initGlobalControls(
	void
	);

#endif //_CONTROLLING_FILE_H_
