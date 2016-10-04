#include "hexaviewer.h"

HexaViewer::HexaViewer(QString filename, int rows, int cols, QWidget *parent, Qt::WFlags flags)
	: Viewer(filename, rows, cols, parent, flags){}

HexaViewer::~HexaViewer(){}

int HexaViewer::binarySearch( QString pattern ){

	//size check
	if( (pattern.size() % 2) != 0 ){
		return -1;
	}

	unsigned char byte = '\0';
	QString byteCouple = "";
	QByteArray bytePattern;
	bool ok;

	for( int i = 0; i < pattern.size(); i+=2 ){

		byteCouple = pattern[i];
		byteCouple.append( pattern[i+1]);

		byte = (char)byteCouple.toInt(&ok, 16);

		//conversion failed
		if( !ok ){
			return -1;
		}

		bytePattern.append(byte);
	}
	
	int seek = 0, idx = 0;
	_contentFile->seek(seek);
	QByteArray read, flowWindow;
	
	//go through file
	while( !_contentFile->atEnd() ){
		
		//read from file
		read = _contentFile->read( DATA_BLOCK );

		//empty buffer
		if( read.size() <= 0 ){
			return -1;
		}

		//append data into flow window
		flowWindow.append( read );

		//cut begin of flow window
		if(flowWindow.size() > 2*DATA_BLOCK){
			flowWindow.remove(0, DATA_BLOCK);
		}		

		//check if pattern is contained
		if( (idx = flowWindow.indexOf(bytePattern)) >= 0 ){

			//begin of file
			if( seek == 0 ){
				return idx;
			}
			//second data block from flowing window
			else if( idx > DATA_BLOCK ){
				return seek - DATA_BLOCK + idx;
			}
			//first data block from flowinf window
			else{				
				return seek + idx;				
			}			
		}

		//setup new seek point
		seek += read.size();
	}

	//reset to previous seeked position
	_contentFile->seek( _file );
	
	return -1;
}

QString HexaViewer::charToCellLabel(unsigned char byte)
{
	//translation char to hexa number
	QString hexaNum = "";
	bool append = false;
	if( byte < 0x10 )
	{
		append = true;
	}
	hexaNum = (hexaNum.setNum(byte,0x10)).toUpper();
	if(append)
	{
		hexaNum = "0" + hexaNum;
	}	
	return hexaNum;
}

void HexaViewer::configureClassSpecifics( void )
{
	//mime type
	_cellMimetype = "application/hexa-dragged-byte";
	_cellMimetype += _fName;

	this->setAcceptDrops(true);
			
	if(!_configExists)
	{
		//config essential
		_byteWidth = 18;
		_byteHeight = CELL_H;
		_appWidth = _cellHorizontal * _byteWidth;
		_appHeight = _cellVertical * _byteHeight;
	}
	
	return;	
}

void HexaViewer::computeMD5( void ){

	//md5 variables
	MD5_CTX md5ctx;
	unsigned char md5bts[16] = "\0";
	
	//buffer variables
	unsigned int read = 0;
	const unsigned int bufferLng = 4096;
	unsigned char buffer[bufferLng] = "\0";	

	QString tmpByte = "";

	//clear md5 hash
	_md5 = "";

	//opened file exists
	if( !_contentFile->isOpen() ){
		return;
	}

	//initialize md5 structure
	MD5Init(&md5ctx);

	//update/compute md5
	while( !_contentFile->atEnd() ){
		
		read = _contentFile->read( (char*)buffer, bufferLng);

		MD5Update( &md5ctx, buffer, read);
	}

	//convert context to md5 "byte array"
	MD5Final( md5bts, &md5ctx);

	//translation into string
	for( int i = 0 ; i < 16; i++ ){

		//convertion uses method for label text creation
		_md5.append( this->charToCellLabel(md5bts[i]));
	}

	//seek file descriptor to begin
	_contentFile->seek(0);
	
	return;
}