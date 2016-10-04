#include "viewerselectable_v3.h"

ViewerSelectable::ViewerSelectable(
	QString* ptrLabelForm,
	QList<QString>* ptrDataForm,
	QString fName,
	QWidget* ptrParent)
	: Viewer(ptrLabelForm, ptrDataForm, fName, ptrParent){

	try{
		_ptrSelectableLayer = new SelectableLayer(this);
	}
	catch(std::bad_alloc){

		QMessageBox msgBox;
		msgBox.setWindowIcon(QIcon::fromTheme("Error Icon",
						QIcon(":/Global/Resources/error.svg")));
		msgBox.setWindowTitle(tr("Fatal Error"));
		msgBox.setText(tr("Application can not start due to memory allocation error."));
		msgBox.setIcon( QMessageBox::Critical );
		msgBox.exec();
	}
}

ViewerSelectable::~ViewerSelectable()
{
	if(_ptrSelectableLayer != NULL){
		delete _ptrSelectableLayer;
	}
}

void ViewerSelectable::configureClassSpecifics(void){
	return;
}

void ViewerSelectable::clearSelection(void){
	_ptrSelectableLayer->clearSelection();	
}

void ViewerSelectable::selectedCellsInterval(signed int &start, signed int &stop)
{
	//init to default [unselected]
	start = -1; stop = -1;
	signed int tmpStart = -1, tmpStop = -1;

	std::vector<int> *pIndexes = new std::vector<int>();

	//get all selcted indexes and recompute to sequence
	QModelIndexList modelIndexes = _ptrSelectableLayer->selectedIndexes();
	for(size_t i = 0; i < modelIndexes.size(); i++){
		pIndexes->push_back((modelIndexes[i].row() * _v2_cols) + modelIndexes[i].column());
	}

	if(pIndexes->size() == 0){
		return;
	}

	//sort in array
	std::sort(pIndexes->begin(), pIndexes->end());

	//find the forst no-ending sequence
	for(size_t i = 0; i < pIndexes->size(); i++)
	{
		if(i == 0){
			start = pIndexes->at(i);
		}
		else if((pIndexes->at(i-1)+1) == pIndexes->at(i)){
			continue;
		}
		else
		{
			stop = pIndexes->at(i-1);
			break;
		}
	}

	//if selection is to the end of viewer
	if(stop == -1){
		stop = pIndexes->at(pIndexes->size()-1);
	}
	return;
}

SelectableLayer* ViewerSelectable::selectableLayer(void){
	return _ptrSelectableLayer;
}

void ViewerSelectable::artificialResize(QString *ptrLabelForm, QList<QString> *ptrDataForm){
	
	Viewer::artificialResize(ptrLabelForm, ptrDataForm);
	_ptrSelectableLayer->reallocateLayer(_v2_rows);
	return;
}

void ViewerSelectable::keyPressEvent(QKeyEvent *ptrEvent){

	switch(ptrEvent->key()){

		///send to parent object
		case Qt::Key_Escape:
		case Qt::Key_Tab:
			ptrEvent->ignore();
			break;

		default:
			ptrEvent->accept();
			break;
	}
	
	return;
}