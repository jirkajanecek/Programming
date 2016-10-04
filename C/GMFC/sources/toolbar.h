#ifndef TOOLBAR_H
#define TOOLBAR_H

#define USABLE_END_STUFF 5

#include <QToolbar>
#include <QString>
#include <QFrame>
#include <QGridLayout>
#include <QHash>
#include <QIcon>
#include <QWindowsVistaStyle>
#include <QAction>
#include <QResizeEvent>

#include <bitset>

#include "styles_v3.h"

#include <QDebug>

class Toolbar : public QToolBar
{
	Q_OBJECT

public:

	//ENUMS
	
	/**
	 * Enum group of posible file types
	 **/
	typedef enum  OPTIONS{
		PE = 0,			/**< PE file */
		PE_OVERLAY,		/**< overlay in PE file exists */
		SUM
	};

	/**
	 * Enum group of possible actions
	 **/
	typedef enum ACTIONS{
		DIRECT_ADDRES = 0,		/**< jump to direct address action */
		DIRECT_ADDRESS_GLOBAL,	/**< jump to direct address action */
		ENTRY_POINT,			/**< jump to entry point action */
		PE_SECTION,				/**< jump to PE section action */
		GEN_DESC,				/**< general description */
		CLOSE,					/**< close action */
		RES_CLIP,				/**< xdef to clipboard */
		RES_CLIP_LIGHT,			/**< xdef to clipboard light */
		RES_CHECK,				/**< result widget check code */
		RES_SKIP,				/**< result widget skip */
		RES_SEQ,				/**< result widget sequence */
		RES_DEFAULT,			/**< result widget default view */
		RES_DEFAULT_ALL,		/**< result widget default view for all selections */
		GMFC_ADD,				/**< add new sample */
		GMFC_FILELIST,			/**< list of opened file */
		XDEF_FILE,				/**< xdef to file */
		XDEF_FILE_LIGHT,		/**< xdef to file light */
		JMP_ROW_U,				/**< jump to row up */
		JMP_ROW_D,				/**< jump to row down */
		JMP_PAGE_U,				/**< jump to page up */
		JMP_PAGE_D,				/**< jump to page down */
		JMP_OPT,				/**< jump to optionally */
		JMP_BEG,				/**< jump to begin */
		JMP_END,				/**< jump to end */
		JMP_LEFT,				///jump left by one
		JMP_RIGHT,				///jump right by one
		OVERLAY,				/**< jump to overlay section */
		IMAGE_BASE,				/**< switch on/off showing of image base addresses */
		BIN_SEARCH,				/**< binary search */
		TXT_SEARCH,
		BIN_SEARCH_GLOBAL,		/**< binary search silent type */
		TXT_SEARCH_GLOBAL,		/// text search global
		SAVE_PATTERN,			/**< save pattern */
		SAVE_PATTERN_FORWARD,	/**< save pattern forward */
		SAVE_PATTERN_BACKWARD,	/**< save pattern backward */
		DISABLE					///disable sample 		
	} ACTIONS;

	//PUBLIC PROPERTIES

	QHash<QAction*, Toolbar::ACTIONS>* Actions(void);

	void Actions(QHash<QAction*, Toolbar::ACTIONS>* actions){this->_actions = actions;};
	
	//CONSTRUCTORS & DESTRUCTOR
	
	/**
	 * Explicit constructor
	 * @param width
	 * @param height
	 * @param parent widget
	 */
	Toolbar(int w,
			int h,
			QWidget *parent = 0);
	
	/**
	 * Implicit destructor
	 **/
	~Toolbar();

	//PUBLIC METHODS

	/**
	 * Method configures itself and one's internal components.
	 * @param toolbar options, which control proper icons set
	 * @return nothing.
	 **/
	void configure( std::bitset<Toolbar::SUM> options );

	/**
	 * Method returns size of usable area. Usable area is ment to space, which
	 * is filled by icons and delimiters. Remain of that space is remain of toolbar's
	 * frame.
	 * @return Size of usable area.
	 **/
	QSize usableSize(void);

	void artificialResize(QResizeEvent* ptrEv );

protected:

	QHash<QAction*, Toolbar::ACTIONS> *_actions;	/**< hash table of posible actions */
	QList<QAction*> _separators;					/**< list of used separators */
	int		_height;								/**< toolbar height */
	int		_width;									/**< toolbar width */
	QFrame*	_base;									/**< base frame */

private:

	//PRIVATE VARIABLES
	std::bitset<Toolbar::SUM> _options;				/**< input toolbar options */

	//PRIVATE METHODS

	/**
	 * Method sets up base frame.
	 * @return nothing
	 **/
	void setupBaseFrame( void );

	/**
	 * Method builds icon set into action array.
	 * @return nothing
	 **/
	virtual void buildIcons( void );
};

#endif // TOOLBAR_H
