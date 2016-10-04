/* colors */
#define CELL_CLR "#ffffff"							//default color of cell
#define CELL_BRD "#ffffff"							//default color of cell border
#define CELL_DRG_CLR "rgb( 0, 153, 255)"			//color of dragged cell
#define CELL_DRG_BRD "#000000"						//dragged cell border color
#define FRAME_BRD "#000000"

/* fonts */
#define CODE_FONT "Lucida Console"
#define CODE_FONT_SIZE 10
#define CODE_FONT_W 6
#define CODE_FONT_H 10

/* styles */

// base frame css style
#define FRAME_STYLE "background-color:" CELL_CLR "; "\
						"border-style:solid; "\
						"border-width: 1px; "\
						"border-color:" FRAME_BRD ";"

#define FRAME_STYLE_EMP "background-color:" CELL_CLR ";"					

//pixmap css style
#define PIXMAP_STYLE "background-color: " CELL_DRG_CLR ";"\
						"border-color: " CELL_DRG_BRD ";"\
						"border-width: 1px;"
						
//normal cell css style
#define CELL_STYLE "background-color: " CELL_CLR ";"\
						"border-width:0px;"