#ifndef RESULTVIEWER_H
#define RESULTVIEWER_H

#include <QtAlgorithms>
#include <QtGui/QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QHash>
#include <QDebug>

#include "styles.h"
#include "viewer.h"

class ResultViewer : public QTableWidget
{
	Q_OBJECT

public:

	//STRUCTS & ENUMS

	/**
	 * Enum for equality list. It has three states.
	 **/
	typedef enum t_eqState{

		EMPTY,	/**< cell is empty */
		EQ,		/**< cell is equal */
		NEQ		/**< cell is not equal */
	};

	typedef enum XDFblock_t{
		
		CC,		/**< cell is CHECK CODE block */
		SEQ,	/**< cell is SEQuence block */
		SK,		/**< cell is SKip block */
		NS		/**< cell is not selected */
	};

protected:

	QHash<QString, QList<int>*> *_viewers;	/**< storage for external viewers [for comparison] */
	
	int		_rowNb;			/**< number of rows */
	int		_colNb;			/**< number of cols */
	int		_cellsNum;		/**< number of all cells */

	int		_byteWidth;		/**< cell width in pixels */
	int		_byteHeight;	/**< cell height in pixels */

	bool	_configExists;	/**< config file flag */
	
	int		_appWidth;		/**< application [widget] width in pixels */
	int		_appHeight;		/**< application [widget] height in pixels */

	float	_succRate;		/**< success rate - percentage of successfull agreement */
	int		_eqNb;			/**< number of equal cells */
	int		_neqNb;			/**< number of not equal cells */
	int		_nemNb;			/**< number of not empty cells */

	QString	_idxShSmplStr;	/**< index of shown sample [from viewer array] */

	QFrame	*_base;			/**< base frame */

	QList<ResultViewer::t_eqState>	*_eqFlags;		/**< equality buffer list */
	QList<ResultViewer::XDFblock_t> *_xdfFlags;		/**< xDEF state list */

public:

	//EXCEPTION

	/**
	 * Class used as exception type in ResultViewer class.
	 **/
	class ResultViewerException : public std::exception{

		public:

			/**
			 * Implicit construcotr
			 **/
			ResultViewerException();

			/**
			 * Explicit constructor
			 * @param message
			 **/
			ResultViewerException(std::string msg);
	};
	
	//CONSTRUCTORS & DESTRUCTORS
	
	/**
	 * Constructor
	 **/
	ResultViewer(int rows = INIT_ROWS,
					int cols = INIT_COLS,
					QWidget *parent = 0,
					Qt::WFlags flags = 0);
	
	/**
	 * Destructor
	 **/
	~ResultViewer();	

	//PUBLIC PROPERTIES
		
	/**
	 * Method returns currently selected items.
	 * @return list of selected items
	 **/
	QList<QTableWidgetItem*> SelectedItems( void );

	/**
	 * Method returns text content of entire viewer.
	 * @return list of string contained in ResultViewer.
	 **/
	QList<QString> TextContent( void );

	/**
	 * Method return xdf blocks of entire viewer.
	 * @return list of xdf blocks from ResultViewer.
	 **/
	QList<ResultViewer::XDFblock_t> XdfBlocks( void );

	/**
	 * Method returns number of rows in result viewer.
	 * @return number of rows
	 **/
	int Rows( void );

	/**
	 * Method returns number of columns in result viewer.
	 * @return number of columns
	 **/
	int Columns( void );

	//PUBLIC METHODS

	/**
	 * Method adds new file viewer into compared array.
	 * When inputed viewer has not have enought cells EXCEPTION ResultViewerException
	 * is thrown.
	 * @param viewer data array
	 **/
	void addViewer( QString key, QList<int>* viewer );

	/**
	 * Method rewrites (or Add if not already exists) viewer in internal array.
	 * When update is complete, refreshViewer is called and result viewer is 
	 * refreshed.
	 * When inputed viewer has not have enought cells EXCEPTION ResultViewerException
	 * is thrown.
	 * @param key for inputed viewer
	 * @param inputed viewer array
	 * @return nothing
	 **/
	void updateViewer( QString key, QList<int>* viewer );

	/**
	 * Method rewrites (or Add if not already exists) viewer in internal array.
	 * When update is complete, refreshViewer is called and result viewer is 
	 * refreshed.
	 * When inputed viewer has not have enought cells EXCEPTION ResultViewerException
	 * is thrown.
	 * @param key for inputed viewer
	 * @param inputed viewer array
	 * @return nothing
	 **/
	void updateViewerSilent(QString key, QList<int>* ptrViewer);

	/** 
	 * Method removes viewer from viewer storage
	 * @param viewer key
	 * @return nothing
	 **/
	void removeViewer( QString key );

	/**
	 * Method checks if viewer by key is already contained in viewer array.
	 * @param key into viewer array
	 * @return true if viewer is contained in viewer array, false if otherwise.
	 **/
	bool checkViewer( QString key );

	/**
	 * Method which, should be called when object is created. It shows entire viewer
	 * content.
	 * @return nothing
	 **/
	void configure( void );

	/**
	 * Method write texts into Output bookmark. It should be used for DEBUG
	 * @param output text
	 * @return nothing
	 */
	void Output( QString msg );

	/**
	 * Method returns percentage success rate or 1 if rate can not be computed.
	 * @return percentage success rate [0.00 - 100.00] or -1 if rat can not be computed.
	 **/
	QString getSuccessRate( void );

	/**
	 * Method directly adjust size of widget by inputed QResizeEvent.
	 * This method should be used when size adjustion is requested and
	 * and programer doesn't able to guarantee spontaneous event. Event
	 * should be accepted in upper level. Method doesn't care if event was
	 * accepted or not. This should be used for force resize.
	 * @param received resize event.
	 * @return nothing
	 **/
	void artificialResize( QResizeEvent *pEv );

	void forceRefresh(void);

signals:

	/**
	 * Signal is emited when selection in viewer is changed. Usually it is
	 * used for communication between text and hexa result viewer.
	 * @param list of selected indexes
	 * @return nothing
	 **/
	void selectionChangedSG( QModelIndexList idxs );

public slots:

	/** 
	 * Slot is activated when selection in viewer was changed. Usually
	 * this method is used for communication between text and hexa result
	 * viewer.
	 * @param list of selected indexes
	 * @return nothing
	 **/
	void selectionChangedSL( QModelIndexList idxs );

	/**
	 * Slot is activated when selection in viewer was changed. It is used
	 * in communication between user [QTableWidget] and result viewer.
	 * @return nothing
	 **/
	void selectionChanged();

	/**
	 * Slot is activated when colorization is required.
	 * @param background color
	 * @param foreground color [text]
	 * @return nothing
	 **/
	void colSelectionSL( QColor colB, QColor colF );

	/**
	 * Slot is activated when rollback action (uncolorization)
	 * is requiered.
	 * @return nothing
	 **/
	void slRollbackSelection( void );

	/**
	 * Slot is activated when complete rollback action (uncolorization)
	 * is requiered.
	 * @return nothing
	 **/
	void slRollbackAll( void );

protected:

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics( void );

	/**
	 * Method configures QTableWidget object.
	 * @return nothing
	 **/
	void confTWidget( void );

	/**
	 * Method set up base frame.
	 * @return nothing
	 **/
	void setupBaseFrame( void );

	/**
	 * Method renders compared results into base frame. Information is loaded
	 * from viewers arrays.
	 * @return nothing
	 **/
	void createViewer( void );

	/**
	 * Method refresh content of viewer array base on inputed viewers.
	 * @return nothing
	 **/
	void refreshViewer( void );

	/**
	 * Method based on previously computed new number of cells [] allocates or
	 * deallocates cells in viewer. This method was created due to need of resizing.
	 * @param difference in entire cells
	 * @return nothing
	 **/
	void reallocViewer( void );

	/**
	 * Method goes through all inserted viewers and determines, bytes equality.
	 * @param compared index through inputed lists.
	 * @param if elements on inputed index are equal, byte will contains their
	 * value. If otherwise, byte will have undetermine value.
	 * @param if elements on inputed index are eual, empty param will be true, if
	 * there is no value of byte in unputed arrays [empty cell in viewer]. Otherwise
	 * will be false.s
	 * @return true when compared bytes throught all viewers are equal, false is
	 * returned otherwise.
	 **/
	bool isEq( int idx, unsigned char* byte, bool *empty );

	/**
	 * Method, which determines if curent equality state can be colorized by inputed
	 * color [col]. If cell can not be colorize by inputed color, method prepares
	 * color sugestion for colorizing [sColB, sColF].
	 * @param current cell state
	 * @param requested color
	 * @param sugested background color
	 * @param sugested foreground color
	 * @return true if colorization is permited, false if otherwise.
	 **/
	bool colPermited( t_eqState cState, QColor col, QColor *sColB, QColor *sColF );
	
	//CONTENT functions
	
	/**
	 * Method used for translation from char into hexa number of inputed char.
	 * @param inputed char [byte]
	 * @return char hexa value in string type.
	 **/
	virtual QString charToCellLabel(unsigned char byte );

	/**
	 * Method connects signals and slots inside ResultViewer.
	 * @return nothing
	 **/
	void connectComps( void );

	/**
	 * Static sort function for comparison between two inputed indexes.
	 * @param the first inputed index
	 * @param the second inputed index
	 * @return true, if the first index is lower placed then the second one,
	 * false is returned otherwise.
	 **/
	static bool sortFunction( QModelIndex i1, QModelIndex i2 );

	/**
	 * Method translates background color to xdf enum item.
	 * @param background color
	 * @return xdf enum item
	 **/
	ResultViewer::XDFblock_t colToXdf( QColor col );

	void rollback( QList<QTableWidgetItem*> items );

protected:

	virtual void resizeEvent( QResizeEvent *pEv );
};

#endif // RESULTVIEWER_H
