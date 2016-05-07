#ifndef _CONTROLLING_FILE_H_
#define _CONTROLLING_FILE_H_

class ControllingClass
{
public:

	enum GLOBAL_INDEXES
	{
		PRINT_MAIN = 0,
		PRINT_CLASS_A,
		PRINT_CLASS_B,
		INDEXES_SIZE
	};

private:

	long long m_controlValues[GLOBAL_INDEXES::INDEXES_SIZE];

public:

	ControllingClass(
		void
		);

	bool check(
		GLOBAL_INDEXES index
		);
};

static ControllingClass g_controllingClass;

#endif //_CONTROLLING_FILE_H_