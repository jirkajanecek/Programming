#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QClipboard>
#include <QApplication>
#include <QFileDialog>

#include <qtextcursor.h>

#include "notImplemented.h"
#include "error.h"

#include "hexaviewerselectable_v3.h"
#include "textviewerselectable_v3.h"
#include "resultToolbar.h"
#include "filewidget_v3.h"
#include "styles_v3.h"

class ResultWidget : public QWidget
{
	Q_OBJECT

public:
	
	/**
	 * Enum for equality list. It has three states.
	 **/
	typedef enum equalState_t{
		EMPTY,	/**< cell is empty */
		EQ,		/**< cell is equal */
		NEQ		/**< cell is not equal */
	};

	typedef enum xdfBlock_t{		
		CC,		/**< cell is CHECK CODE block */
		SEQ,	/**< cell is SEQuence block */
		SK,		/**< cell is SKip block */
		NS		/**< cell is not selected */
	};

	struct colorBlock_t{
		int start;
		int end;
		xdfBlock_t type;
	};
	
	/**
	 * Implicit constructor
	 **/
	ResultWidget(QWidget *parent = 0, Qt::WFlags flags = 0);

	/**
	 * Implicit destructor
	 **/
	~ResultWidget();

	/**
	 * Method configures widget (cocretely it configures base frame). 
	 * @return nothing
	 **/
	void configure( void );

	/**
	 * Method directly adjust size of widget by inputed QResizeEvent.
	 * This method should be used when size adjustion is requested and
	 * and programer doesn't able to guarantee spontaneous event. Event
	 * should be accepted in upper level. Method doesn't care if event was
	 * accepted or not. This should be used for force resize.
	 * @param received resize event.
	 * @return nothing
	 **/
	void artificialResize(int diffH);

	void copySelection(void);

signals:

	/**
	 * Method colorizes selected items by inputed color. Method can colorize
	 * background and foreground.
	 * @param background color
	 * @param foreground color [font color]
	 * @return nothing
	 **/
	void sgColSelection(QColor colB, QColor colF);

	/**
	 * Method [signal] reverts colorization of selected items.
	 * @return nothing
	 **/
	void sgRollbackSelection( void );

	/**
	 * Method [signal] reverts colorization of all items.
	 * @return nothing
	 **/
	void sgRollbackAll(void);

	/**
	 * Signal is emited when viewer was changed.
	 * @param string typed succes rate
	 * @return nothing
	 **/
	void sgSuccRateUpdate( QString rate );

public slots:

	/**
	 * Method contains functionality when toolbar is activated. Generally this
	 * method runs proper method base on acepted action from toolbar.
	 * @param action defines from toolbar
	 * @return nothing
	 **/
	void slTBarActivated(QAction* act);

	void updateCompare(QString key, QList<int>* dataContent );

	void updateCompareSilent(QString key, QList<int>* ptrDataContent);

	/**
	 * Method adds new array to comparison. Key should be filename with file path.
	 * Method invokes viewers (table and text viewer) refresh.
	 * @param key for identification
	 * @param content for comparison
	 * @return nothing
	 **/
	void slAddToCompare(QString key, QList<int>* vCont, QString md5);

	void slRemoveCompare(QString key, bool silent = false);

	void slFinalResizeRefresh(void);

private:	

	HexaViewerSelectable*	_v3_ptrHexaViewer;	/**< hexa selectable viewer */
	TextViewerSelectable*	_v3_ptrTextViewer;	/**< text selectable viewer */

	QString	_v3_hexaLabelForm;
	QString	_v3_textLabelForm;

	QList<QString> _v3_hexaDataForm;
	QList<QString> _v3_textDataForm;

	QString _referenceSampleKey;	/**< key to sample storage - reference file for viewer */

	float	_v3_successRate;
	int		_v3_equalCells;
	int		_v3_notEqualCells;
	int		_v3_notEmptyCells;

	int		_v3_rows;
	int		_v3_cols;
	int		_v3_cellsNum;

	QList<ResultWidget::xdfBlock_t>*	_v3_ptrXdfs;
	QList<ResultWidget::equalState_t>*	_v3_ptrEquals;
	QHash<QString, QList<int>*>*		_v3_ptrSampleStorage;

	QFrame*						_base;		/**< base frame */
	ResultToolbar*				_tBar;		/**< result viewer toolbar */
	std::bitset<Toolbar::SUM>	_tBarOpt;	/**< options comming from toolbar */
	QHash<QString,QString>*		_ptrMD5s;	/**< md5 hashes storage [fileName, md5] */	

	/**
	 * Method configures base frame.
	 * @return nothing
	 **/
	void setupBaseFrame( void );

	/**
	 * Method places components of widget into proper position.
	 * @return nothing
	 **/
	void placeComponents( void );	

	/**
	 * Method connects component's signal and slots.
	 * @return nothings
	 **/
	void connectComponents( void );

	/**
	 * Method process selected [colorize] input into xDEF. Generally it is
	 * envelope for more specific methods. Also method copies assemblied xDEF 
	 * into application clipboard.
	 * @return nothing
	 **/
	void procResToClip( bool lightVer = false );

	/**
	 * Method assemblies xDEF from inputed parameters.
	 * @param text content list
	 * @param hexa content list
	 * @param xDef blocks list
	 * @return asseblied xDEF
	 **/
	QString asmXdf( 
			QList<QString>* ptrTextContent,
			QList<QString>* ptrHexaContent,
			QList<ResultWidget::xdfBlock_t>* ptrXdfs );

	/**
	 * Method assemblies xDEF from inputed parameters.
	 * @param hexa content list
	 * @param xDef blocks list
	 * @return asseblied xDEF
	 **/
	QString asmXdfLight(
			QList<QString>* ptrHexaContent,
			QList<ResultWidget::xdfBlock_t>* ptrXdfs);

	/**
	 * Method translates numeric address into string form. Also method aligns
	 * legnth of address to predefined size [XDF_ADDR_LEN].
	 * @param numeric address
	 * @return string address
	 **/
	QString trAddrToStr( int addr );

	/**
	 * Method appends one row from xDEF into def parameter.
	 * @param xDEF definition
	 * @param string form of address
	 * @param numeric index for next address [it is forward generated]
	 * @param row of hexa data
	 * @param row of text data
	 * @return nothing, because new row is appended into def parameter.
	 **/
	void publishRow(
			QString &definition,
			QString &address,
			int idx,
			QString &hexaTmp,
			QString &textTmp);

	void saveXdf(bool lightVer = false);

	void v3_createModels(void);	

	QString v3_successRate(void);

	bool v3_isEqual(int index, unsigned char* ptrByte, bool* ptrEmpty);

	void v3_colorizeSelection(ResultWidget::xdfBlock_t);

	void v3_eraseXdfSelection(void);
};

#endif // RESULTWIDGET_H
