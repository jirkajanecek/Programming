#pragma once

#include <initializer_list>
#include <vector>

class InitializerList
{
public:

	struct Payload
	{
		float fVariable;
		int iVariable;
	};

	class SequenceClass
	{
	private:

		std::vector<int> m_holder;

	public:

		SequenceClass(
			void
		);

		//constructor with initializer list
		//see usage in InitializerList::newStyle
		SequenceClass(
			std::initializer_list<int> initList
		);

		//std::initializer_list is real type so it can be used
		//in method
		void append(
			std::initializer_list<int> payload
		);
	};
	
	InitializerList();
	
	~InitializerList();

	static void process(
		void
	);

private:

	static void oldStyle(
		void
	);

	static void newStyle(
		void
	);
};

