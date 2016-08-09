#pragma once
class Constexpr
{
public:
	
	Constexpr(
		void
	);
	
	~Constexpr(
		void
	);

	static void process(
		void
	);

	static constexpr int getConst(void) {return 5;}
};

