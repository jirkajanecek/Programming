#pragma once

#include <vector>

class TypeInference
{
private:

	std::vector<int> m_ints;

public:
	
	TypeInference(
		void
	);
	
	
	~TypeInference(
		void
	);

	static void process(
		void
	);

private:

	void vectorUsage(
		void
	);

	void variableUsage(
		void
	);
};

