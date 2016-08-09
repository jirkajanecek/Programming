#pragma once

//TODO: move constructor

//rvalue reference article: http://thbecker.net/articles/rvalue_references/section_01.html
//other description to performace issues during returning value https://en.wikipedia.org/wiki/Return_value_optimization

class RValueAndMoveConstructor
{
public:

	struct Helper
	{
		int tmp;
	};

	RValueAndMoveConstructor(
		void
	);
	
	~RValueAndMoveConstructor(
		void
	);

	static void process(
		void
	);

	static void rValueAndlValue(
		void
	);

	static void rValueReference(
		void
	);

	static void helper(
		Helper& helper
	);

	static void helper(
		Helper&& helper
	);

	static Helper foobar(
		void
	);

};

