#ifndef SAVEPARSER_H
#define SAVEPARSER_H

//save pattern interval regular expression
#define SAVE_PATTERN_INTERVAL "\\[([0-9A-Fa-f]{8})-([0-9A-Fa-f]{8})\\]"

#include <QStringList>
#include <QRegExp>
#include <QObject>
#include <QFile>
#include <QList>
#include <qdebug.h>

#include "PEbrowser.h"

class SaveParser : public QObject
{
	Q_OBJECT

public:

	///PUBLIC STRUCTURES & ENUMS

	/**
	 * Address type 
	 **/
	typedef enum address_e{
		PA,	/**< physical address */
		VA	/**< virtual address */
	};

	/**
	 * Save pattern interval structure
	 **/
	typedef struct saveInterval_t{
		int						start;	/**< start address of interval */
		int						stop;	/**< stop address of interval */
		SaveParser::address_e	type;	/**< type of address */
	};

	///PUBLIC EXCEPTIONS

	/**
	 * SavePattern's exception class. Derived from standard C++ exception class.
	 **/
	class SaveParserException : public std::exception{
	public:
		SaveParserException( std::string msg );
	};

	/**
	 * constructor
	 **/
	SaveParser(QObject *parent = NULL);
	
	/**
	 * destructor
	 **/
	~SaveParser();

	///PUBLIC PROPERTIES

	QList<SaveParser::saveInterval_t>* Intervals(void);

	///PUBLIC METHODS

	void addSavePattern( QString fName, PEbrowser *ptrPE );

private:
	
	///PRIVATE VARIABLES

	QList< SaveParser::saveInterval_t>	*_ptrStorage;		/**< interval storage */
	
	///PRIVATE METHODS

	void openFile( QString fName , QFile &file );

	void parseFile( QFile &file );

	void closeFile( QFile &file );

	void parseAndStoreInterval( QString &interval );

	void trVAtoPA( PEbrowser *ptrPE );	
};

#endif // SAVEPARSER_H
