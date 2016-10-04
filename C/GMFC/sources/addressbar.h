#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QtGui/QWidget>
#include <Qlist>
#include <QPoint>
#include <QTableWidget>
#include <QHeaderView>
#include <QResizeEvent>
#include <vector>

#include <qdebug.h>

#include "viewerStyles.h"
#include "styles.h"
#include "PEbrowser.h"

#define ADDR_WIDTH 8	//number of chars in address
#define ADDR_STEP 16
#define ADDR_NUM_DFT 64

class AddressBar : public QTableWidget
{
	Q_OBJECT

public:

	/**
	 * explicit constructor
	 * @param parent's widget
	 * @param base address [PE file only]
	 * @param section structures [PE file only]
	 * @param flags
	 **/
	AddressBar(int startAddr = 0,
			int rows = ADDR_NUM_DFT,
			QWidget *parent = 0,
			PEbrowser *ptrPE = NULL,
			Qt::WFlags flags = 0);

	/** 
	 * implicit constructor
	 **/
	~AddressBar();

	/**
	 * Method which, should be called when object is created. It shows entire address 
	 * bar content.
	 * @return nothing
	 **/
	void configure( void );

	/**
	 * Method print message into console output. It should be used
	 * only for debug intentions.
	 * @param message for printing
	 * @return nothing
	 **/
	void Output(QString msg);

	/**
	 * Method returns address based on sequential index from 
	 * viewer.
	 * @param sequntial index
	 * @return QString typed formated address or empty string if sequential 
	 * address is not correct.
	 **/
	QString getAddressFromSeqIndex( int seqIdx);

	void artificialResize(QResizeEvent* ptrEv, int startAddr);

public slots:

	/**
	 * Method [slot], which responds to viewer changes - shift version.
	 * @param cell shift
	 * @param position in file
	 * @param viewer offset
	 **/
	void viewerChanged(int shift);

	/**
	 * Method [slot], which respond viewer changes - absolute version.
	 * @param absolute value
	 * @param position in file
	 * @param viewer offset
	 **/
	void viewerChangedAbs(int start);

	/**
	 * Method [slot] switch values in addressBar between zero ofset and
	 * image base address.
	 * @return nothing
	 **/
	void slBaseImageAddress( void );

private:
	
	QList<int>		*_addresses;	/**< list of addresses */
	QFrame			*_base;			/**< base frame */

	int _shift;			/**< shift emitted by viewers */
	int _startAddr;		/**< start address of address bar */

	//PE file variable
	bool		_baseAddrEn;	/**< flag base image address enabled */
	PEbrowser	*_ptrPE;		/**< pointer to PE object */
	
	bool _configExists;	/**< configuration file exists */

	int _appWidth;		/**< application width */
	int _appHeight;		/**< application height */
	int _addrWidth;		/**< one address width */
	int _addrHeight;	/**< one address height */
	int _addrVertical;	/**< number of addresses vetically */
	
	//PROPERTY functions

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	void configureClassSpecifics(void);

	void configureTWidget(void);
	
	//UI functions

	/**
	 * Method set up base frame.
	 * @return nothing
	 **/
	void setupBaseFrame( void );

	//CONTENT functions
	
	/**
	 * Method creates addresses array, which holds addresses in bar.
	 * @return nothing
	 **/
	void createAddresses( void );

	/**
	 * Method refreshes address aray due to shift change.
	 * @return nothing
	 **/
	void refreshAddresses(void);

	/**
	 * Method reallocate number of cells in address array
	 **/
	void reallocateAddresses(void);

	/**
	 * Method creates bar object, which is filled by cells.
	 * @return nothing
	 **/
	void createBar( void );

	/**
	 * Method refreshes bar due to shift change.
	 * @return nothing
	 **/
	void refreshBar( void );

	/**
	 * Method refresh bar content after shift is proceeded.
	 * @return nothing
	 */
	void propagateStep( void );

	/**
	 * Method computes position for new cell.
	 * @param row, where cell should be positioned
	 * @return pointer to position
	 **/
	QPoint* computePosition( int row );

	/**
	 * Method used for translation from int into hexa number of address.
	 * @param inputed int [address]
	 * @return address hexa value in string type.
	 **/
	QString numToCellLabel( unsigned int addr );

	/**
	 * Method used for translation from int into hexa number of address.
	 * @param inputed int [address]
	 * @return address hexa value in string type.
	 **/
	QString numToHexString( unsigned int addr );

	unsigned long trToImgAddr( unsigned long physAddr );
};

#endif // ADDRESSBAR_H
