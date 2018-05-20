#include "Picture.h"



void Picture::selected_SetColor(COLORREF color){
	for (int i = 0; i < selectedIDs.size(); i++){
		figures[selectedIDs[i]]->setColor(color);
	}
}

void Picture::selected_Delete(){
	for ( int i = selectedIDs.size() - 1; i >= 0; i-- ){	//������� ��������� ������� � ����� �������, ����� �� ������� ���������
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

	int newCount = count; //�������, ��� ���� ����� ����� ������� ����� ����� ���� ��������
	for ( int i = 0; i < count; i++ ){	//������ �� ���� ���������� �������
		vector<Figure*>* nestedFigures = figures[ selectedIDs[i] ]->getNestedFigures();	//��������� ��������� ����� �� ����������
		int nestedCount = nestedFigures->size(); 	//������� ���-�� ������������ �����
		if ( nestedCount >= 2){	//���� ���������� ������ ��������� �������
			newCount += nestedCount - 1; //-1 �.�. ID �� ������ (�������) ������ ����� �����
			figures.erase( figures.begin() + selectedIDs[i] );
			//_selectedIDs.erase ( _selectedIDs.begin() + i );
			for ( int j = 0; j < nestedCount; j++)
				figures.push_back( nestedFigures->data()[j] );
		}
	}

	this->unselect();
	for (int j = count - 1; j < newCount; j++ )
		selectedIDs.push_back( j );
	
	//�� ����� �����-�� ��������� ��� ����������� �������� ���-�� �������� (�������� �������+�������). ������ ��� �� ����������� ���������
}

void Picture::selectLast(bool add){
	int lastID = figures.size() - 1;
	selectByID(lastID, add);
}

void Picture::selectByID(int id, bool add){
	if ( id < 0 || id >= figures.size() ) return;

	if (add){	//���� ����� ��������� id � ��� ������������� ������:
		for (int i = 0; i < selectedIDs.size(); i++ )
			if ( selectedIDs[i] == id )	//����� ���� ����� id ��� ���� � ������
				return;
	}	else
	this->unselect();	//����� - �������� ������ ���������

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
	if (selectedIDs.size() > 1){ //���� �������� ����� ����� ������
		vector<int> oldIDs(selectedIDs);
		for (int i = 0; i < selectedIDs.size(); i++ ){
			unselect();
			selectByID(oldIDs[i]);
			selected_LayerUp();
		}
		return true;
	}*/
	if (selectedIDs[0] == figures.size()-1) return; //������ � ��� �� ����� �����

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
	if (selectedIDs[0] == 0) return; //������ � ��� � ����� ����

	Figure *tmp = figures[selectedIDs[0]];
	figures[selectedIDs[0]] = figures[selectedIDs[0] - 1];
	figures[selectedIDs[0] - 1] = tmp;

	selectedIDs[0]--;
	return;
}

