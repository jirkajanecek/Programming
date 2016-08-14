#include "initializer_list.h"



InitializerList::InitializerList()
{
}



InitializerList::~InitializerList()
{
}


void InitializerList::process(
	void
)
{
	oldStyle();
	newStyle();
}


InitializerList::SequenceClass::SequenceClass(
	void
)
{

}


InitializerList::SequenceClass::SequenceClass(
	std::initializer_list<int> initList
)
{
	m_holder.insert(m_holder.end(), initList.begin(), initList.end());
}


void InitializerList::SequenceClass::append(
	std::initializer_list<int> payload
)
{
	m_holder.insert(m_holder.end(), payload.begin(), payload.end());
}


void InitializerList::oldStyle(
	void
)
{
	Payload scalar = { 1.0f, 1 };
	Payload array[] = { {1.0f, 1}, {2.0f, 2} };
}

void InitializerList::newStyle(
	void
)
{
	//classic declaration
	InitializerList::SequenceClass sequenceClass;

	//declaration with list
	InitializerList::SequenceClass sequenceClassWithList = { 1,2,3,4,5 };
	sequenceClassWithList.append({ 6,7,8,9 });

	//init list declaration in std vector
	std::vector<std::string> strings = { "Text1", "Text2", "Text3" };
}