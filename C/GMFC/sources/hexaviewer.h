/**
 * HexaViewer ver 1.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef HEXAVIEWER_H
#define HEXAVIEWER_H

//read data block
#define DATA_BLOCK 4096

#include <QtGui/QWidget>
#include <QCoreApplication>

#include "viewer.h"

class HexaViewer : public Viewer
{
	Q_OBJECT

public:
	
	/**
	 * Explicit constructor
	 * @param file name for view
	 * @param Qt parent widget
	 * @param Qt flags
	 **/
	HexaViewer(QString filename = "",
				int rows = INIT_ROWS,
				int cols = INIT_COLS,
				QWidget *parent = 0,
				Qt::WFlags flags = 0);
	
	/**
	 * Destructor
	 **/
	~HexaViewer();

	/**
	 * Method search binary pattern [typed as hexa string] in file. If pattern is
	 * located address of this pattern is returned, otherwise -1.
	 * @param string typed hexa pattern
	 * @return offset to file, where finded pattern started, or -1 if pattern is not
	 * contained in file.
	 **/
	virtual int binarySearch( QString pattern );
	
protected:

	//PROPERTY functions

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics( void );
	
	//CONTENT functions
	
	/**
	 * Method used for translation from char into hexa number of inputed char.
	 * @param inputed char [byte]
	 * @return char hexa value in string type.
	 **/
	virtual QString charToCellLabel(unsigned char byte );

	/**
	 * Method computes sample's md5 hash and store it into internal variable
	 * @return nothing
	 **/
	virtual void computeMD5( void );	
};

#endif // HEXAVIEWER_H
