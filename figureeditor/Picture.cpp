#include "Picture.h"



void Picture::selected_SetColor(COLORREF color){
	for (int i = 0; i < selectedIDs.size(); i++){
		figures[selectedIDs[i]]->setColor(color);
	}
}

void Picture::selected_Delete(){
	for ( int i = selectedIDs.size() - 1; i >= 0; i-- ){	//удалять указатели начиная с конца вектора, чтобы не сбилась нумерация
		figures.erase( figures.begin() + selectedIDs[i] );
		selectedIDs.erase( selectedIDs.begin() + i );
	}
}

void Picture::selected_Merge(){
	int count = selectedIDs.size();
	if ( count < 2 ) return;

	ComplexFigure *newCF = new ComplexFigure;
	for ( int i = 0; i < count; i++ )
		newCF->addFigure( figures[ selectedIDs[i] ] );

	this->selected_Delete();

	figures.push_back(newCF);
	this->selectByID( figures.size() - 1 );
}

void Picture::selected_Divide(){
	int count = selectedIDs.size();
	if ( count == 0 ) return;

	int newCount = count; //счётчик, для того чтобы знать сколько новых фигур надо выделить
	for ( int i = 0; i < count; i++ ){	//проход по всем выделенным фигурам
		vector<Figure*>* nestedFigures = figures[ selectedIDs[i] ]->getNestedFigures();	//получение вложенных фигур из выделенной
		int nestedCount = nestedFigures->size(); 	//подсчёт кол-ва получившихся фигур
		if ( nestedCount >= 2){	//если выделенная фигура оказалась сложной
			newCount += nestedCount - 1; //-1 т.к. ID на старую (сложную) фигуру будет удалён
			figures.erase( figures.begin() + selectedIDs[i] );
			//_selectedIDs.erase ( _selectedIDs.begin() + i );
			for ( int j = 0; j < nestedCount; j++)
				figures.push_back( nestedFigures->data()[j] );
		}
	}

	this->unselect();
	for (int j = count - 1; j < newCount; j++ )
		selectedIDs.push_back( j );
	
	//всё равно какие-то нарушения при разделениии большого кол-ва объектов (возможно сложных+простых). Точнее при их последующем выделении
}

void Picture::selectLast(bool add){
	int lastID = figures.size() - 1;
	selectByID(lastID, add);
}

void Picture::selectByID(int id, bool add){
	if ( id < 0 || id >= figures.size() ) return;

	if (add){	//если нужно добавлять id к уже существующему списку:
		for (int i = 0; i < selectedIDs.size(); i++ )
			if ( selectedIDs[i] == id )	//выйти если такой id уже есть в списке
				return;
	}	else
	this->unselect();	//иначе - очистить список выбранных

	selectedIDs.push_back( id );
}

bool Picture::trySelectByRect( Coord r1, Coord r2 ){
	this->unselect();
	bool result = false;
	for ( int i = 0; i < figures.size(); i++ )
		if ( figures[i]->isInRect( r1, r2 ) ){
			selectedIDs.push_back( i );
			result = true;
		}
	return result;
}

Picture::~Picture(){
	figures.clear();
}

void Picture::selected_SetWidth(unsigned value){
	for (int i=0; i< selectedIDs.size(); i++)
		figures[ selectedIDs[i] ]->setWidth(value);	
}

void Picture::selected_Move(double dx, double dy){
	for (int i=0; i< selectedIDs.size(); i++)
		figures[ selectedIDs[i] ]->move(dx, dy);
}

void Picture::unselect(){
	selectedIDs.clear();
}

void Picture::addFigure(Figure* pFigure){
	figures.push_back(pFigure);
}

void Picture::clear(){
	figures.clear();
	selectedIDs.clear();
}

void Picture::draw(HDC hDC) const {
	for (int i = 0; i < figures.size(); i++)
		figures[i]->draw(hDC);

	for (int i = 0; i < selectedIDs.size(); i++)
		figures[ selectedIDs[i] ]->drawSelector( hDC );
}


void Picture::selected_LayerUp(){
	if (selectedIDs.size() != 1) return;
	/*if (selectedIDs.empty()) return;
	if (selectedIDs.size() > 1){ //если выделено более одной фигуры
		vector<int> oldIDs(selectedIDs);
		for (int i = 0; i < selectedIDs.size(); i++ ){
			unselect();
			selectByID(oldIDs[i]);
			selected_LayerUp();
		}
		return true;
	}*/
	if (selectedIDs[0] == figures.size()-1) return; //фигура и так на самом верху

	Figure *tmp = figures[selectedIDs[0]];
	figures[selectedIDs[0]] = figures[selectedIDs[0] + 1];
	figures[selectedIDs[0] + 1] = tmp;

	selectedIDs[0]++;
	return;
}

void Picture::selected_LayerDown(){
	if (selectedIDs.size() != 1) return;
	/*if (selectedIDs.empty()) return;
	if (selectedIDs.size() > 1){
		vector<int> oldIDs(selectedIDs);
		for (int i = 0; i < selectedIDs.size(); i++ ){
			unselect();
			selectByID(oldIDs[i]);
			selected_LayerDown();
		}
		return;
	}*/
	if (selectedIDs[0] == 0) return; //фигура и так в самом низу

	Figure *tmp = figures[selectedIDs[0]];
	figures[selectedIDs[0]] = figures[selectedIDs[0] - 1];
	figures[selectedIDs[0] - 1] = tmp;

	selectedIDs[0]--;
	return;
}

