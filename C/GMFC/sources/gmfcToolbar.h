#ifndef GMFCTOOLBAR_H
#define GMFCTOOLBAR_H

#include <QWidget>

#include "toolbar.h"

class GMFCToolbar : public Toolbar{

	Q_OBJECT

private:

	int _floatResBufferH;

public:

	GMFCToolbar(	
				int w,					/**< toolbat width */
				int h,					/**< toolbar height */
				QWidget *parent = 0 );	/**< toolbar's parent */

	~GMFCToolbar();

	/**
	 * Method builds icons into proper set of this toolbar.
	 * @return nothing
	 **/
	virtual void buildIcons( void );

	void artificialResize(QResizeEvent* ptrEv);
};

#endif