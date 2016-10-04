#ifndef RESULTTEXTVIEWER_H
#define RESULTTEXTVIEWER_H

#include "resultviewer.h"

class ResultTextViewer : public ResultViewer{

	Q_OBJECT

public:
	
	ResultTextViewer(int rows = INIT_ROWS,
						int cols = INIT_COLS,
						QWidget *parent = 0,
						Qt::WFlags flags = 0);
	
	~ResultTextViewer();

private:

	QString _substChar;

protected:

	/**
	 * Method used for translation from char into hexa number of inputed char.
	 * @param inputed char [byte]
	 * @return char hexa value in string type.
	 **/
	virtual QString charToCellLabel(unsigned char byte );

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics( void );
};

#endif // RESULTTEXTVIEWER_H
