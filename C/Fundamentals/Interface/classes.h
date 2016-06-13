#ifndef CLASSES_H
#define CLASSES_H

class IFace
{

public: 

	virtual ~IFace(
		void
		);

	virtual void fce(
		void
		) = 0;
};


class ImplA :
	public IFace
{

public:

	ImplA(
		void
		);

	virtual ~ImplA(
		void
		);

	virtual void fce(
		void
		);
};

#endif //CLASSES_H
