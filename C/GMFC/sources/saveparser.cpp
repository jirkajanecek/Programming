#include "saveparser.h"

SaveParser::SaveParserException::SaveParserException(std::string msg) 
	: std::exception(msg.c_str()){};

SaveParser::SaveParser(QObject *parent)
	: QObject(parent){

	_ptrStorage = NULL;
	_ptrStorage = new QList<SaveParser::saveInterval_t>();
}

SaveParser::~SaveParser(){

	if( _ptrStorage != NULL ){
		delete _ptrStorage;
	}
}

QList<SaveParser::saveInterval_t>* SaveParser::Intervals( void ){

	return _ptrStorage;
}

void SaveParser::addSavePattern(QString fName, PEbrowser *ptrPE){

	QFile file;

	//open file
	this->openFile( fName, file);
	
	//parse file and find intervals
	this->parseFile( file );

	//close file
	this->closeFile( file );

	//translate Virtual Address to Physical Address
	this->trVAtoPA( ptrPE );

	return;
}

void SaveParser::openFile( QString fName, QFile &file ){

	//content file handling
	if( fName == "" ){
		
		//file name was not inputed
		throw SaveParser::SaveParserException("File name was not inputed.");
	}
	
	//allocate file handler
	file.setFileName( fName );	

	//open file
	if(!file.open(QIODevice::ReadOnly)){
		
		//file name not exists
		throw SaveParser::SaveParserException("File not exists.");
	}

	return;
}

void SaveParser::parseFile( QFile &file ){

	QString strLine;
	QRegExp regExp( SAVE_PATTERN_INTERVAL);

	while( !file.atEnd() ){
		
		//prepare string and load from file
		strLine = "";
		strLine.append( file.readLine() );
		strLine.trimmed();
		
		//check if line has proper intrval pattern, then parse it
		if(strLine.indexOf( regExp ) >= 0){
			this->parseAndStoreInterval( strLine );			
		}
	}

	return;
}

void SaveParser::closeFile( QFile &file){

	file.close();
	return;
}

void SaveParser::parseAndStoreInterval( QString &interval ){

	interval.remove("[").remove("]");	
	QStringList values = interval.split("-");
	bool ok1 = false, ok2 = false;

	if( values.size() == 2 ){
		
		//create and fill interval structure
		SaveParser::saveInterval_t intrvStruct;
		intrvStruct.start = (values[0]).toInt( &ok1, 16);
		intrvStruct.stop = (values[1]).toInt( &ok2, 16);
		intrvStruct.type = SaveParser::VA;	//setup virtual address

		//convertion error
		if( !ok1 || !ok2 ){
			return;
		}

		//check if storage is empty
		if( _ptrStorage->size() == 0 ){

			//store the first interval
			_ptrStorage->append( intrvStruct );
		}
		//if starage is not empty, store to proper place
		else{

			SaveParser::saveInterval_t tmpIntrv;
			bool stored = false;

			//find proper place for interval storing
			for( int i = 0; i < _ptrStorage->size(); i++ ){

				//download current item
				tmpIntrv = _ptrStorage->at(i);

				//check if start address is smaller then inserted
				if( intrvStruct.start < tmpIntrv.start  ){
					
					//insert item into storage on proper place
					_ptrStorage->insert(i, intrvStruct);

					//set up flag about storaging
					stored = true;

					break;
				}
			}

			//no item in storage is smaller, so put it in the end of storage
			if( !stored ){
				_ptrStorage->append(intrvStruct);
			}
		}
	}

	return;
}

void SaveParser::trVAtoPA( PEbrowser *ptrPE ){

	SaveParser::saveInterval_t sInterval;
	PEbrowser::Section_t sSct;
	long offset1 = -1, offset2 = -1;

	//convertion
	for( int i = 0; i < _ptrStorage->count(); i++ ){

		sInterval = _ptrStorage->at(i);

		switch(sInterval.type ){
			
		case SaveParser::VA:
			
			//start interval
			offset1 = sInterval.start - ptrPE->OH_ImageBase();

			//stop interval
			offset2 = sInterval.stop - ptrPE->OH_ImageBase();
			
			//offsets check
			if( (offset1 >= 0) && (offset2 >= 0)){

				sInterval.start = offset1;
				sInterval.stop = offset2;
				sInterval.type = SaveParser::PA;
				_ptrStorage->replace(i, sInterval);
			}		

			break;

		case SaveParser::PA:
		default:
			break;
		}
	}	

	return;
}