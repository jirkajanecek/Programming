/**
 * Viewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QLabel>
#include <QDragEnterEvent>
#include <QByteArray>
#include <QString>
#include <QPixmap>
#include <QHeaderView>

#include <qmap.h>
#include <qvariant.h>

#include <qtableview.h>
#include <qstandarditemmodel.h>

#include <qcoreapplication.h>

#include <QDebug>

#include "viewerStyles.h"
#include "styles.h"

class Viewer : public QTableView{

	Q_OBJECT

public:

	//EXCEPTIONS

	/**
	 * Exception class for Viewer class.
	 **/
	class ViewerException : public std::exception{
		public:
			/**
			 * implicit constructor
			 **/
			ViewerException( void );

			/**
			 * explicit constructor
			 * @param message
			 **/
			ViewerException( std::string msg );
	};

	/**
	 * Viewer::IndexItem_t structure defines indexes into viewer, file and
	 * bytes array. 
	 */
	typedef struct{
		int viewer;			/**< index into viewer */
		int file;			/**< index into file */
		int bytes;			/**< index into bytes */
	}Viewer::IndexItem_t;

	/**
	 * Explicit constructor
	 * @param data model
	 * @param Qt parent widget
	 **/
	Viewer(QStandardItemModel* ptrModel, QString fName, QWidget* ptrParent = 0);

	/**
	 * Destructor
	 **/
	~Viewer();	

	/**
	 * Method which, should be called when object is created. It shows entire viewer
	 * content.
	 * @return nothing
	 **/
	void configure(void);

	void artificialResize(QStandardItemModel* ptrModel);

	/**
	 * Method refreshes values in viewer. Generaly, it changes only
	 * labels. Nothing new is created.
	 * @param silent flag - no eminent recomputation is requested
	 * @return nothing
	 **/
	void updateView(QStandardItemModel* ptrModel);

signals:

	/**
	 * Signal is emitted when shift exists, and it is necessary recompute model.
	 * @param shift
	 * @return nothing
	 **/
	void sgViewChanged(int shift, int drag);

protected:
	
	//model & view components
	QStandardItemModel*	_v2_ptrModel;	/**< model component */
	
	//darg and drop variables
	int _drag;		/**< index to indexes array */	
	int _shift;		/**< curent shift - based on move */

	int _v2_cellHeight;		/**< byte cell height */
	int _v2_cellWidth;		/**< byte cell width */
	int _v2_rows;
	int _v2_cols;

	QString _v2_fName;		/**< file name for specific mime type */
	
	QString _cellMimetype;	/**< cell mime type */

	//PROPERTY functions

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics(void);

	/**
	 * Method configures internal table widget, which affect application design.
	 * @return nothing
	 **/
	void configureView(void);
	
	/**
	 * Method renders bytes into base frame. Information are loaded from
	 * data and indexes arrays.
	 * @return nothing
	 **/
	void createView(void);

	/**
	 * Method translates inputed point into accord sequence number in
	 * viewer array.
	 * @param inputed point 
	 * @return sequence number
	 **/
	int translatePointToSequence(QPoint point);

	/**
	 * Method validates position (point) in buffer viewer. If position
	 * is outside of buffer point is changed into the most outer position.
	 * @param position in buffer
	 * @return nothing - point is changed itself (pointer)
	 **/
	//void validateBufferPosition(QPoint* point);

	/**
	 * Method determines if shift is processed or not.
	 * @return true if shift is neccessary, false if otherwise.
	 **/
	bool shiftRequested(QPoint pos);

	/**
	 * Method computes shift from current position and original drag position.
	 * This result is stored into internal variable _shift.
	 * @param current position - point
	 * @return nothing
	 **/
	void computeShift(QPoint pos);

	/**
	 * Method is activated when mouse press into base frame.
	 * It is used as the first function in drag and droj
	 * procedure.
	 * @return nothing
	 **/
	virtual void mousePressEvent(QMouseEvent *event);

	/**
	 * Method is activated when dragged object entering into
	 * new area [in app enter to new cell].
	 * @param drag enter event
	 * @return nothing
	 **/
	virtual void dragEnterEvent(QDragEnterEvent *event);

	/**
	 * Method is activated when drag move is processed.
	 * @param move event
	 * @return nothing
	 **/	
	virtual void dragMoveEvent(QDragMoveEvent *event);

	/**
	 * Method is activated when drop action is proceeded.
	 * @param drop event
	 * @return nothing
	 **/
	virtual void dropEvent(QDropEvent *event);
};

#endif // VIEWER_H