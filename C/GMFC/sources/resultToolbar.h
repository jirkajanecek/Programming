#include "toolbar.h"

class ResultToolbar : public Toolbar{

public:

	/**
	 * Explicit constructor
	 **/
	ResultToolbar( int w, int h, QWidget* parent = 0 );

	/**
	 * Destructor
	 **/
	~ResultToolbar();

	/**
	 * Virtual method builds icon set into toolbar.
	 * @return nothing
	 **/
	virtual void buildIcons( void );

};