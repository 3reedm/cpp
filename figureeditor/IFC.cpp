#include "IFC.h"


/****************** IFC (InteractiveFigureConstructor) ********************/
/**************************** (Singletone) ********************************/

IFC* IFC::_selfPointer = NULL;
FigureType IFC::_lastFigureType = FT_NULL;

void IFC::setTask(FigureType FT, unsigned width, COLORREF color){
	clearTask();
	if (FT == FT_LAST) 
		_task = _lastFigureType;
	else {
		_task = FT;
		_lastFigureType = FT;
	}
	_width = width;
	_color = color;

	updateFigureParts();
}

void IFC::updateMouseCoords(double x, double y){
	_mouseCoords.x = x;
	_mouseCoords.y = y;

	if (isInProcess())	//���� ����������� ���������� � �������� �������� ������
		updateFigureParts();	//�������� ������������� ����������� ��������� (�������������)
}

void IFC::updateFigureParts(){
	if (_coords.size() == 0){	//���� �� ���� ���������� ��� �� ���� �������������, �������� ����� �� ������� ����������� ����
		if (_tmpFigureParts.size() == 0)
			_tmpFigureParts.push_back( new F_Line( _mouseCoords.x, _mouseCoords.y, _mouseCoords.x, _mouseCoords.y, _width, _color ) );
		else 
			_tmpFigureParts.back() = new F_Line( _mouseCoords.x, _mouseCoords.y, _mouseCoords.x, _mouseCoords.y, _width, _color );
	} 
	else
	{	//���� ��� ���� ������������� ���� �� 1 ���������� - ������������ ������� �� ���� ����������� ������
		switch (_task){
			case FT_LINE:
				if ( _coords.size() == 1)
					_tmpFigureParts.back() = new F_Line( _coords[0].x, _coords[0].y, _mouseCoords.x, _mouseCoords.y, _width, _color );
				break;
			case FT_CURVE:
				if (_additionalCoords.size() == 0){
					_additionalCoords.push_back( Coord((int)_coords[0].x, (int)_coords[0].y) );
					_tmpFigureParts.back() = new F_Curve(_additionalCoords, _width, _color);
				} 
				else
					if (_mouseCoords.x != _additionalCoords.back().x || _mouseCoords.y != _additionalCoords.back().y ){	//����� ����������� ������ ������,
						_additionalCoords.push_back(Coord((int)_mouseCoords.x, (int)_mouseCoords.y));																//����� �� ����������� ������ �������
						_tmpFigureParts.back() = new F_Curve(_additionalCoords, _width, _color);
					}
				break;
			case FT_ELLIPSE:
				if ( _coords.size() == 1)
					_tmpFigureParts.back() = new F_Ellipse( _coords[0].x, _coords[0].y, _mouseCoords.x, _mouseCoords.y, _width, _color );
				break;
			case FT_RECTANGLE:
				if ( _coords.size() == 1)
					_tmpFigureParts.back() = new F_Rectangle( _coords[0].x, _coords[0].y, _mouseCoords.x, _mouseCoords.y, _width, _color );
				break;
			case FT_ARC:
				if ( _coords.size() == 1)
					_tmpFigureParts.back() = new F_Ellipse( _coords[0].x, _coords[0].y, _mouseCoords.x, _mouseCoords.y, 1, 0x00000000, true );
				
				if ( _coords.size() == 3){
					Coord eCenter( (_coords[0].x + _coords[1].x)/2, (_coords[0].y + _coords[1].y)/2 );
					if ( _tmpFigureParts.size() == 1)
						_tmpFigureParts.push_back( new F_Arc( _coords[0], _coords[1], _coords[2], _mouseCoords, _width, _color ) );
					else
						_tmpFigureParts.back() = new F_Arc( _coords[0], _coords[1], _coords[2], _mouseCoords, _width, _color );
				}

				break;
		}
	}
}

bool IFC::isInProcess() const {
	if (_task == FT_NULL)
		return false;
	return true;
}

void IFC::clearTask(){
	_task = FT_NULL;
	_coordsLeft = 0;
	_coords.clear();
	_additionalCoords.clear();
	_tmpFigureParts.clear();
	_width = 0;
	_color = 0;
}

IFC* IFC::getInst(){
	if (_selfPointer == NULL)
		_selfPointer = new IFC();
	return _selfPointer;
}

void IFC::initialize(){
	_selfPointer = NULL;
}

void IFC::destroy(){
	delete _selfPointer;
	initialize();
}

IFC::IFC(){
	clearTask();
	_mouseCoords.x = 0;
	_mouseCoords.y = 0;
}

IFC::~IFC(){
	_tmpFigureParts.clear();
}

Figure* IFC::fixCurrentCoord(){
	_coords.push_back( _mouseCoords );

	switch (_task){
		case FT_CURVE:
			_coordsLeft = 2 - _coords.size();
			if (_coordsLeft == 0){
				Figure* result = new F_Curve(_additionalCoords, _width, _color);
				clearTask();
				return result;
			}
			break;
		case FT_ELLIPSE:
			_coordsLeft = 2 - _coords.size();
			if (_coordsLeft == 0){
				Figure* result = new F_Ellipse(_coords[0].x, _coords[0].y, _coords[1].x, _coords[1].y, _width, _color);
				clearTask();
				return result;
			}
			break;
		case FT_RECTANGLE:
			_coordsLeft = 2 - _coords.size();
			if (_coordsLeft == 0){
				Figure* result = new F_Rectangle(_coords[0].x, _coords[0].y, _coords[1].x, _coords[1].y, _width, _color);
				clearTask();
				return result;
			}
			break;
		case FT_LINE:
			_coordsLeft = 2 - _coords.size();
			if (_coordsLeft == 0){
				Figure* result = new F_Line(_coords[0].x, _coords[0].y, _coords[1].x, _coords[1].y, _width, _color);
				clearTask();
				return result;
			}
			break;
		case FT_ARC:
			_coordsLeft = 4 - _coords.size();
			if (_coordsLeft == 0){
				Figure* result = new F_Arc( _coords[0], _coords[1], _coords[2], _coords[3], _width, _color );
				clearTask();
				return result;
			}
			break;
	}

	return 0;
}

void IFC::displayIntermediateRes(HDC hDC){
	for (int i = 0; i < _tmpFigureParts.size(); i++)
		_tmpFigureParts[i]->draw(hDC);
}