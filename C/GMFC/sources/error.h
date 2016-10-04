#ifndef ERROR_H
#define ERROR_H

#include <QMessageBox>
#include <QIcon>

class Error{

	public:
		
		static void alert( QString title, QString desc );
};

#endif //ERROR_H