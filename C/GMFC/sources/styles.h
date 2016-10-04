#ifndef STYLES_H
#define STYLES_H

#define SCRL_STYLE "background-color:#ffffff;"

///////////////////////////////////////////////////////////////////////////////

/* Viewers */
#define INIT_COLS 16
#define INIT_ROWS 64

#define TVIEWER_COLS 16
#define HVIEWER_COLS 16
#define AVIEWER_COLS 1

#define TVIEWER_W 192
#define HVIEWER_W 288
#define AVIEWER_W 60

#define VIEWER_STYLE "QFrame{ background-color: #ffffff; }"

///////////////////////////////////////////////////////////////////////////////

/* FileWidget */
#define FWGT_FOC_COL "QFrame{ background-color: #0099ff; }"
#define FWGT_BLUR_COL "QFrame{ background-color: #a0a0a0; }"

/* ScrollBar */
#define SCROLL_H 15

/* toolbar */
#define TOOL_HEIGHT 28
#define TOOL_TITLE "toolbar"
#define TOOL_ICON_HEIGHT 20
#define TOOL_ICON_WIDTH 20

/* title */
#define TITLE_HEIGHT 20
#define TITLE_STYLE "background-color: #707070;" \
						"color: #ffffff;"

/* target address dialog */
#define TA_STYLE "background-color:#a0a0a0;"

//save pattern color
#define SAVE_COLB QColor( 0xff, 0xff, 0x33 )
#define SAVE_COLF QColor( 0x00, 0x00, 0x00 )

///////////////////////////////////////////////////////////////////////////////

/* Result widget */
#define RWIDGET_STYLE "background-color: #a0a0a0;" \
						"border-width:0px;"
/* XDF defines */
#define XDF_HEAD	"*NAME \" \"\n" \
					"\n"

#define	XDF_BEGIN	"*HEAL ALL:DEL\n" \
					"*BEGIN\n"

#define XDF_MD5		";MD5 of compared samples:\n";

#define XDF_END		"*END\n"

#define XDF_ADDR_LEN 4

/* color style for blocks [SQ, CC, SK]  */ 
#define SQ_COLB QColor( 0x66, 0x99, 0xff )
#define SQ_COLF QColor( 0x00, 0x00, 0x00 )
#define SQ_TEXT QString("*SEQ")

#define CC_COLB QColor( 0x00, 0x00, 0x99 )
#define CC_COLF QColor( 0xff, 0xff, 0xff )
#define CC_TEXT QString("*CHECK CODE")

#define SK_COLB QColor( 0xff, 0xff, 0x33 )
#define SK_COLF QColor( 0x00, 0x00, 0x00 )
#define SK_TEXT QString("*SKIP")

#define EMPTY_COLB QColor( 0xff, 0xff, 0xff )
#define EMPTY_COLF QColor( 0x00, 0x00, 0x00 )

#define EQ_COLB QColor( 0xff, 0xff, 0xff )
#define EQ_COLF QColor( 0x00, 0x00, 0x00 )

#define NEQ_COLB QColor( 0xff, 0x00, 0x33 )
#define NEQ_COLF QColor( 0x00, 0x00, 0x00 )

///////////////////////////////////////////////////////////////////////////////

/* samples viewer */
#define SMPLS_H 791
#define SMPLS_W 1138

/*#define SMPLS_H 500
#define SMPLS_W 500*/

/* one sample - file widget */
#define SMPL_H 772
#define SMPL_W 564

//ensure visibility margins
#define X_VIS_MARGIN 15
#define Y_VIS_MARGIN 15

#define CELL_H 11

///////////////////////////////////////////////////////////////////////////////

/* InfoBar */
#define IB_FONT "Lucida Console"

#define IB_DA_HEIGHT 16		/**< info bar DA cell height */
#define IB_DA_WIDTH 220		/**< info bar DA cell width */
#define IB_DA_TEXT "Dragged Address: "
#define IB_DA_VAL_DFT "-"

#define IB_SR_HEIGHT 16		/**< info bar SR cell height */
#define IB_SR_WIDTH 200		/**< info bar SR cell width */
#define IB_SR_TEXT "Success Rate: "
#define IB_SR_VAL_DFT "N/A"

#define IB_CELL_STYLE "QLabel{ background-color:#707070; color:#ffffff; }"

#define BAR_HEIGHT 22		/**< info bar height */
#define BAR_STYLE "QFrame{background-color:#a0a0a0}"

#endif
 
