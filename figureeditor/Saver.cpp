#include "Saver.h"

Saver* Saver::_selfPointer = NULL;

Saver* Saver::getInst(){
	if (_selfPointer == NULL){
		_selfPointer = new Saver();
		_selfPointer->_picture = new Picture();
		_selfPointer->_creatingComplex = false;
	}
	return _selfPointer;
}

void Saver::initialize(){
	_selfPointer = NULL;
}

void Saver::destroy(){
	delete _selfPointer;
	initialize();
}

void Saver::savePicToFile(Picture* picture, LPCWSTR fileName) const {
	DWORD nb = 0;
	HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE,	
									FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	string fString = "<picture>\n";
	for (int i=0; i < picture->figures.size(); i++){
			fString += picture->figures[i]->toString() + "\n";
			WriteFile(hFile, fString.c_str(), fString.size(), &nb, NULL);
			fString.clear();
		}
	fString = "</picture>";
	WriteFile(hFile, fString.c_str(), fString.size(), &nb, NULL);

	CloseHandle(hFile);
}

void Saver::fillCoords(string coordsString){	//������ �� ������ � ������������
	_coords.clear();
	string::size_type iCoordStart = 0, iCoordEnd = -1;
	for (;;){
		iCoordStart = coordsString.find("(", iCoordEnd + 1);
		if (iCoordStart == string::npos)	//���� ����������� ������ ������ �� ������� - ��������� ���������� ���������
			break;
		iCoordEnd = coordsString.find(")", iCoordStart + 1);

		_coords.push_back( Coord(coordsString.substr(iCoordStart, iCoordEnd - iCoordStart + 1)) );
		iCoordStart = iCoordEnd;
	}
}

bool Saver::executeTag(string text, TAG_NAME name) const {
	string mainTag = ""; //��������, "���������" ��� (��� ���������)
	wstring msg;

	/****************** 1: ����� ��������� ����� (1 �������� = 1 ���) *****************/
	string nestedOpening, nestedClosing, nestedTag;	//�����������/����������� ��������� � ��� ���������� ���
	string::size_type iStart = 0, iEnd = -1, iTagStart = 0, iTagEnd = 0;

	for (;;){
		nestedOpening.clear();
		nestedClosing.clear();
		nestedTag.clear();

		iStart = text.find("<", iEnd + 1);	//����� ������������ ���������
		if (iStart == string::npos){	//��������� ����� ������ �� �������
			mainTag += text.substr(iEnd + 1, text.size() - iEnd - 1);	//�������� ��� += �� ��������� ������� �� ����� ������
			break;
		}
		mainTag += text.substr(iEnd + 1, iStart - iEnd - 1);	//�������� ��� += �� ��������� ������� �� ������ ���������� ����������

		iEnd = text.find(">", iStart + 1);
		nestedOpening = text.substr(iStart + 1, iEnd - iStart - 1);	//��������� ��������
		iTagStart = iEnd + 1;	//���������� ������ ���������� ����

		getInst()->preventiveTagActions( getTagName(nestedOpening) );	//��������� ���� ������� ������������ ������ ��������, �� ��������� ������������� ���������

		while (nestedClosing != "/" + nestedOpening){	//����� ������������ ���������
			iStart = text.find("<", iEnd + 1);
			iEnd = text.find(">", iStart + 1);
			nestedClosing = text.substr(iStart + 1, iEnd - iStart - 1);
		}
		iTagEnd = iStart - 1; //�������� ����� ���������� ����
		
		nestedTag = text.substr(iTagStart, iTagEnd - iTagStart + 1);	//������� ��� ��������� ���

		// ���������� � ��������� �������� ������:
		iTagStart = 0;
		iTagEnd = 0;
		iStart = iEnd;

		// ����� ���������� ���������� ����
		executeTag( nestedTag, getTagName(nestedOpening) );
	}

	//��������� ����� �� �������� => ��������� ��������, ���������
	switch (name){
	case (TG_NULL):
		return true;
	case (TG_PICTURE): 
		break;
	case (TG_COMPLEX): 
		getInst()->_picture->selected_Merge();
		getInst()->_picture->unselect();
		getInst()->_creatingComplex = false;
		break;
	case (TG_LINE):
		// ���������� ������� ��������� (�� ��������� ������ ����)
		getInst()->fillCoords( mainTag );
		// ���������� �����
		getInst()->_picture->addFigure( new F_Line(_coords[0].x, _coords[0].y, _coords[1].x, _coords[1].y, _width, _color) );
		// ���� ��� ������� �������� ������� ������ - �������� ����� ����� � ���� ������
		if (_creatingComplex) _picture->selectLast(true);
		break;
	case (TG_RECTANGLE):
		getInst()->fillCoords( mainTag );
		getInst()->_picture->addFigure( new F_Rectangle(_coords[0].x, _coords[0].y, _coords[1].x, _coords[1].y, _width, _color) );
		if (_creatingComplex) _picture->selectLast(true);
		break;
	case (TG_ELLIPSE):
		getInst()->fillCoords( mainTag );
		getInst()->_picture->addFigure( new F_Ellipse(_coords[0].x, _coords[0].y, _coords[1].x, _coords[1].y, _width, _color) );
		if (_creatingComplex) _picture->selectLast(true);
		break;
	case (TG_ARC):
		getInst()->fillCoords( mainTag );
		getInst()->_picture->addFigure( new F_Arc(_coords[0], _coords[1], _coords[2], _coords[3], _width, _color) );	
		if (_creatingComplex) _picture->selectLast(true);
		break;
	case (TG_CURVE):
		getInst()->fillCoords( mainTag );
		getInst()->_picture->addFigure( new F_Curve(_coords, _width, _color) );
		if (_creatingComplex) _picture->selectLast(true);
		break;
	case (TG_WIDTH):
		getInst()->_width = atoi ( mainTag.c_str() );
		break;
	case (TG_COLOR):
		getInst()->_color = atoi ( mainTag.c_str() );
		break;
	}
}

Picture* Saver::loadPicFromFile(LPCWSTR fileName) const {	//�������� ������� �� �����
	/*************** �������� ����� ***************/
	DWORD nb = 0;
	HANDLE hFile = CreateFileW(fileName,
														GENERIC_READ,
														FILE_SHARE_READ,	//��� ����������� ������
														NULL,	//������ �� ���������
														OPEN_EXISTING, //������ ������������
														FILE_ATTRIBUTE_NORMAL,	//������� ����
														NULL);	//��������� ������� ���

	if (hFile == INVALID_HANDLE_VALUE){ 
    MessageBox(NULL, L"������ �������� �����!", L"Error", MB_OK);
    return 0;
	}

	/**************** ������ ����� ****************/
	DWORD dwBR;
	char ch;
	string file;
	while (ReadFile(hFile, &ch, 1, &dwBR, NULL) && dwBR)
		file += ch;
	CloseHandle(hFile);

	/************* ������ ����������� *************/

	_picture->clear();
	executeTag(file, TG_NULL);

	return _picture;
}


/*********************** ���� ***********************/
TAG_NAME Saver::getTagName(string caption){
	if (caption == "picture")
		return TG_PICTURE; else
	if (caption == "complex")
		return TG_COMPLEX; else
	if (caption == "line")
		return TG_LINE; else
	if (caption == "rectangle")
		return TG_RECTANGLE; else
	if (caption == "ellipse")
		return TG_ELLIPSE; else
	if (caption == "arc")
		return TG_ARC; else
	if (caption == "curve")
		return TG_CURVE; else
	if (caption == "w")
		return TG_WIDTH; else
	if (caption == "c")
		return TG_COLOR;

	return TG_NULL;
}

void Saver::preventiveTagActions( TAG_NAME tagName){
	if ( tagName == TG_COMPLEX )
		_creatingComplex = true;
}