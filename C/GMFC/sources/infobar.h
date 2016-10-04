#ifndef INFOBAR_H
#define INFOBAR_H

#include <QtGui/QWidget>
#include <QLabel>
#include <QFont>
#include <QDebug>

#include "styles.h"

class InfoBar : public QFrame
{
	Q_OBJECT

public:
	
	InfoBar(QWidget *parent = 0, Qt::WFlags flags = 0);
	
	~InfoBar();

	void setDA( QString addr = "" );

	QString DA( void );

	void setSR( QString rate = "" );

	QString SR( void );

	void configure(int w );	

private:

	QLabel *_ptrDA;	/**< draged address label */
	QLabel *_ptrSR;	/**< success rate */
};

#endif // INFOBAR_H
