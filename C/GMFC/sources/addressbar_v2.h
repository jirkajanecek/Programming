#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QWidget>
#include <QHeaderView>

#include <qstandarditemmodel.h>
#include <qtableview.h>

//#include <qdebug.h>

#include "viewerStyles.h"
#include "styles.h"

class AddressBar : public QTableView
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
	AddressBar(QStandardItemModel* ptrModel, QWidget* ptrParent);

	/** 
	 * implicit constructor
	 **/
	~AddressBar();

	/**
	 * Method which, should be called when object is created. It shows entire address 
	 * bar content.
	 * @return nothing
	 **/
	void configure(void);

	/**
	 * Method should be called when object size changes are requested.
	 * @param model with data after resize
	 **/
	void artificialResize(QStandardItemModel* ptrModel);

	/**
	 * Method refreshes bar due to shift change.
	 * @return nothing
	 **/
	void updateView(QStandardItemModel* ptrModel);

private:
	
	QFrame	*_base;		/**< base frame */

	int	_v2_cellHeight;	/**< cell height */
	int	_v2_cellWidth;	/**< cell width */

	int _v2_rows;
	int _v2_cols;

	QStandardItemModel* _v2_ptrModel;	/**< data model */
	
	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	void configureClassSpecifics(void);

	/**
	 * Method configures TableView component for suitable view
	 * @return nothing
	 **/
	void configureView(void);
	
	/**
	 * Method creates view object, which is filled by cells.
	 * @return nothing
	 **/
	void createView(void);	
};

#endif // ADDRESSBAR_H
