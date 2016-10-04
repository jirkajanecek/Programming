#ifndef CELL_H
#define CELL_H

#include <QLabel>
#include <QFont>
#include <QString>

#include "viewerStyles.h"

/**
 * Cell class
 * Class provides label object, which is place as icon into Viewer. 
 **/
class Cell : public QLabel
{
	public:
		
		/**
		 * Explicit constructor version 1
		 * @param icon text
		 * @param icon width
		 * @param icon height
		 * @param parent object
		 * @param point of placement
		 * @param background color
		 **/
		Cell( const QString &text,int width,int height,QWidget *parent,QPoint *pos );

		/**
		 * Explicit constructor version 2
		 * @param icon width
		 * @param icon height
		 * @param parent object
		 * @param point of placement
		 * @param background color
		 **/
		Cell( int width, int height, QWidget *parent, QPoint *pos );

		/**
		 * Method changes label of inputed Cell cell.
		 * @param inputed Cell cell
		 * @param new text
		 * @return nothing
		 **/
		void setText( QString text );

		/**
		 * Method colorize byteItem object.
		 * Color code. Possibly set to #xxxxxx, rgb() or rgba().
		 * @return nothing
		 **/
		void colorize( QString color );
};

#endif //CELL