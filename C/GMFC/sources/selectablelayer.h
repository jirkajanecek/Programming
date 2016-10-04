#ifndef SELECTABLELAYER_H
#define SELECTABLELAYER_H

#include <qheaderview.h>
#include <qtablewidget.h>
#include <qstandarditemmodel.h>
#include <QModelIndex>
#include <QKeyEvent>
#include <QCoreApplication>

#include "styles_v3.h"

#include <qdebug.h>

class SelectableLayer : public QTableWidget
{
	Q_OBJECT

public:	
	SelectableLayer(QWidget *parent);
	~SelectableLayer();

	void reallocateLayer(int rows);

private:

	int _rows;
	int _cols;
	int _cellsNum;

	int _cellWidth;
	int _cellHeight;

public:
	void configure(int rows, int cols, int cellWidth, int cellHeight);
	QModelIndexList selectedIndexes(void);
	void eraseAllColors(void);

protected:
	//void keyPressEvent(QKeyEvent *ptrEvent);

protected slots:
	void slItemSelectionChanged(void);
	void slSelectionChangedExternaly(QModelIndexList indexes);	

signals:
	void sgSelectionChangedExternaly(QModelIndexList indexes);
};

#endif // SELECTABLELAYER_H
