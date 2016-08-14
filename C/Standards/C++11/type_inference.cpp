#include "type_inference.h"
#include <iostream>

TypeInference::TypeInference()
{
	for (size_t i = 0; i < 10; ++i)
	{
		m_ints.push_back((int)i);
	}
}


TypeInference::~TypeInference()
{
}


void TypeInference::process(
	void
)
{
	TypeInference typeInference;
	typeInference.vectorUsage();
}


void TypeInference::vectorUsage(
	void
)
{
	//old fashioned access to vector
	for (std::vector<int>::iterator it = m_ints.begin(); it != m_ints.end(); ++it)
	{
		std::cout << *it << std::endl;
	}

	//auto version
	for (auto it = m_ints.begin(); it != m_ints.end(); ++it)
	{
		std::cout << *it << std::endl;
	}

	//most compact way for container with begin and end
	for (auto &it : m_ints)
	{
		std::cout << it << std::endl;
	}
}


void TypeInference::variableUsage(
	void
)
{
	//inference from value
	auto autoIntVariable = 5;

	//inference from previously declared variable
	int intVariable;
	decltype(intVariable) anotherIntVariable = 5;

	//inference from template
	std::vector<int> ints = { 1,2 };
	auto i = ints[0];
}