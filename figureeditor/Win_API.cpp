#include "Picture.h"
#include "Line.h"
#include "Curve.h"
#include "Ellipse.h"
#include "Arc.h"
#include "IFC.h"
#include "toString.h"
#include "Saver.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define ID_BUTTON_LINE 1100
#define ID_BUTTON_RECTANGLE 1200
#define ID_BUTTON_ELLIPSE 1300
#define ID_BUTTON_ARC 1400
#define ID_BUTTON_CURVE 1500

#define ID_BUTTON_RPLUS 2100
#define ID_BUTTON_RMINUS 2200
#define ID_BUTTON_GPLUS 2300
#define ID_BUTTON_GMINUS 2400
#define ID_BUTTON_BPLUS 2500
#define ID_BUTTON_BMINUS 2600
#define ID_BUTTON_WPLUS 2700
#define ID_BUTTON_WMINUS 2800

#define ID_BUTTON_MERGE 3100
#define ID_BUTTON_DIVIDE 3200
#define ID_BUTTON_DELETE 3300

#define ID_BUTTON_SAVE 4100
#define ID_BUTTON_OPEN 4200

/*
	вынести теги в отдельный класс, пригодится!

	разобраться с хедерами наследников figure чтобы все фигуры друг друга видели

	при нажатии на границу окна - щелчок учитывается и захват мыши в
	этой ситуации не спасает. М.б. нужно отправлять координату в IFC только если она
	на рабочей поверхности. Точно подобрать её координаты с учётом границ окна и т.д.

	*min(), max();
*/

//Globals
unsigned Width  = 1014;
unsigned Height = 700;
Coord MouseCoords;
bool MouseLB = false;
Picture* picture = new Picture();
COLORREF Color = RGB(0,225,0);
unsigned LineWidth = 20;
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	static bool selection = false;
	static Coord selCoord1;
	bool paramsChanged = false;
	short R = GetRValue(Color), G = GetGValue(Color), B = GetBValue(Color);

	switch (msg){
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
			//Обработка кнопок
			if (wParam == ID_BUTTON_LINE)
				IFC::getInst()->setTask(FT_LINE, LineWidth, Color);	
			if (wParam == ID_BUTTON_RECTANGLE)
				IFC::getInst()->setTask(FT_RECTANGLE, LineWidth, Color);
			if (wParam == ID_BUTTON_ELLIPSE)
				IFC::getInst()->setTask(FT_ELLIPSE, LineWidth, Color);
			if (wParam == ID_BUTTON_ARC)
				IFC::getInst()->setTask(FT_ARC, LineWidth, Color);
			if (wParam == ID_BUTTON_CURVE)
				IFC::getInst()->setTask(FT_CURVE, LineWidth, Color);

			if (wParam == ID_BUTTON_RPLUS ){ R += 5; paramsChanged = true; }
			if (wParam == ID_BUTTON_RMINUS ){ R -= 5; paramsChanged = true; }
			if (wParam == ID_BUTTON_GPLUS ){ G += 5; paramsChanged = true; }
			if (wParam == ID_BUTTON_GMINUS ){ G -= 5; paramsChanged = true; }
			if (wParam == ID_BUTTON_BPLUS ){ B += 5; paramsChanged = true; }
			if (wParam == ID_BUTTON_BMINUS ){ B -= 5; paramsChanged = true; }
			if (paramsChanged){
				if (R>255) R=255; if (R<0) R=0;
				if (G>255) G=255; if (G<0) G=0;
				if (B>255) B=255; if (B<0) B=0;
				Color = RGB(R,G,B);
			}
			if (wParam == ID_BUTTON_WPLUS)
				if (LineWidth < 500){
					LineWidth++;
					paramsChanged = true;
				}
			if (wParam == ID_BUTTON_WMINUS)
				if (LineWidth > 1) {
					LineWidth--;
					paramsChanged = true;
				}

			if (wParam == ID_BUTTON_MERGE)
				picture->selected_Merge();	
			if (wParam == ID_BUTTON_DIVIDE)
				picture->selected_Divide();
			if (wParam == ID_BUTTON_DELETE)
				picture->selected_Delete();

			if (wParam == ID_BUTTON_SAVE){	//Сохранение файла
				//диалог
				OPENFILENAME ofn;
			  TCHAR sfile[MAX_PATH];//
			  ZeroMemory(&ofn, sizeof(ofn));
			  ZeroMemory(sfile, sizeof(TCHAR)*MAX_PATH);//
			  ofn.lStructSize = sizeof(ofn);
			  ofn.hwndOwner = NULL;//
				ofn.lpstrFile = sfile;//
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFilter = L"Figure Editor Image(*.fei)\0*.fei\0\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = 0;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.nFileOffset = 0;
				ofn.nFileExtension = 0;
				ofn.lpstrDefExt = L"fei";
				ofn.lCustData = 0;
				ofn.lpfnHook = 0;
				ofn.lpTemplateName = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

				if (GetSaveFileName(&ofn))
					Saver::getInst()->savePicToFile( picture, ofn.lpstrFile );

				break;
			}
			if (wParam == ID_BUTTON_OPEN){ //Открытие файла
				//диалог
				OPENFILENAME ofn;
			  TCHAR sfile[MAX_PATH];//
			  ZeroMemory(&ofn, sizeof(ofn));
			  ZeroMemory(sfile, sizeof(TCHAR)*MAX_PATH);//
			  ofn.lStructSize = sizeof(ofn);
			  ofn.hwndOwner = NULL;//
				ofn.lpstrFile = sfile;//
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFilter = L"Figure Editor Image(*.fei)\0*.fei\0\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = 0;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.nFileOffset = 0;
				ofn.nFileExtension = 0;
				ofn.lpstrDefExt = L"fei";
				ofn.lCustData = 0;
				ofn.lpfnHook = 0;
				ofn.lpTemplateName = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				//загрузка
				if (GetOpenFileName(&ofn))
					picture = Saver::getInst()->loadPicFromFile(ofn.lpstrFile);
				break;
			}
			SetFocus(hWnd); //!!
			break;
		case WM_KEYDOWN:
			//операции над рисунком
			if (wParam == 'M' )
				picture->selected_Merge();
			if (wParam == 'D' )
				picture->selected_Divide();
			if (wParam == VK_ESCAPE)
				DestroyWindow(hWnd);
			if (wParam == VK_BACK)
				picture->clear();
			if (wParam == VK_DELETE)
				picture->selected_Delete();
			if (wParam == VK_PRIOR)
				picture->selected_LayerUp();
			if (wParam == VK_NEXT)
				picture->selected_LayerDown();

			//передвижение
			if (wParam == VK_RIGHT)
				picture->selected_Move(3, 0);
			if (wParam == VK_LEFT)
				picture->selected_Move(-3, 0);
			if (wParam == VK_UP)
				picture->selected_Move(0, -3);
			if (wParam == VK_DOWN)
				picture->selected_Move(0, 3);
			
			//цвет
			if (wParam == VK_NUMPAD7 ){ R+=5; paramsChanged = true; }
			if (wParam == VK_NUMPAD4 ){ R-=5; paramsChanged = true; }
			if (wParam == VK_NUMPAD8 ){ G+=5; paramsChanged = true; }
			if (wParam == VK_NUMPAD5 ){ G-=5; paramsChanged = true; }
			if (wParam == VK_NUMPAD9 ){ B+=5; paramsChanged = true; }
			if (wParam == VK_NUMPAD6 ){ B-=5; paramsChanged = true; }
			if (paramsChanged){
				if (R>255) R=255; if (R<0) R=0;
				if (G>255) G=255; if (G<0) G=0;
				if (B>255) B=255; if (B<0) B=0;
				Color = RGB(R,G,B);
			}

			//толщина линии
			if (wParam == VK_ADD)
				if (LineWidth < 500){
					LineWidth++;
					paramsChanged = true;
				}
			if (wParam == VK_SUBTRACT)
				if (LineWidth > 1) {
					LineWidth--;
					paramsChanged = true;
				}

			//создание (клавишами)
			if (!MouseLB){
				if (wParam == 'C')
					IFC::getInst()->setTask(FT_CURVE, LineWidth, Color);
				if (wParam == 'E')
					IFC::getInst()->setTask(FT_ELLIPSE, LineWidth, Color);
				if (wParam == 'R')
					IFC::getInst()->setTask(FT_RECTANGLE, LineWidth, Color);
				if (wParam == 'L')
					IFC::getInst()->setTask(FT_LINE, LineWidth, Color);
				if (wParam == 'A')
					IFC::getInst()->setTask(FT_ARC, LineWidth, Color);
				}
			break;
		case WM_MOUSEMOVE:
			RECT wr; GetWindowRect(hWnd, &wr);
			POINT mousePoint;
			GetCursorPos(&mousePoint);
			MouseCoords.x = mousePoint.x - wr.left - 3;
			MouseCoords.y = mousePoint.y - wr.top - 25;	//30/25

			IFC::getInst()->updateMouseCoords(MouseCoords.x, MouseCoords.y);

			if ( selection )
				if ( picture->trySelectByRect( selCoord1, MouseCoords )){
					Color = picture->figures[ picture->selectedIDs.back() ]->getColor();
					LineWidth = picture->figures[ picture->selectedIDs.back() ]->getWidth();
				}
			break;
		case WM_LBUTTONDOWN:
			MouseLB = true;
			SetCapture(hWnd);	//без захвата мыши WM_LBUTTONUP вне окна не поступает!!
			//
			picture->unselect();
			if ( IFC::getInst()->isInProcess() ){
				Figure* newFigure = IFC::getInst()->fixCurrentCoord();
					if (newFigure != 0){
						picture->addFigure(newFigure);
						IFC::getInst()->setTask(FT_LAST, LineWidth, Color);
					}
			} else {
				selection = true;
				selCoord1.x = MouseCoords.x;
				selCoord1.y = MouseCoords.y;
			}
			break;
		case WM_LBUTTONUP:
			MouseLB = false;
			selection = false;
			ReleaseCapture();
			//
			if ( IFC::getInst()->isInProcess() ){
				Figure* newFigure = IFC::getInst()->fixCurrentCoord();
				if ( newFigure != 0 ){
					picture->addFigure(newFigure);
					IFC::getInst()->setTask(FT_LAST, LineWidth, Color);
				}
			}
			break;
		case WM_RBUTTONDOWN:
			IFC::getInst()->clearTask();
			picture->unselect();
			SetFocus(hWnd);
			break;
		case WM_PAINT:
			PAINTSTRUCT ps;//
			HDC hDC = BeginPaint(hWnd, &ps);

				HDC hMemoryDC = CreateCompatibleDC(hDC);
				HBITMAP hMemoryBmp = CreateCompatibleBitmap(hDC, Width, Height);
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemoryDC, hMemoryBmp);

				RECT wndRect;
				GetClientRect(hWnd, &wndRect);
				
				HBRUSH background = CreateSolidBrush(0x00ffffff);
				FillRect(hMemoryDC, &wndRect, background);
				DeleteObject(background);
				////
				
				picture->draw(hMemoryDC);
				IFC::getInst()->displayIntermediateRes(hMemoryDC);
				if ( selection ){
					F_Rectangle selectionRect( selCoord1.x, selCoord1.y, MouseCoords.x, MouseCoords.y, 1, 0, 1);
					selectionRect.draw( hMemoryDC );
				}

				//надписи
				SetBkMode(hMemoryDC, TRANSPARENT);

				for (int i = 0; i < picture->selectedIDs.size(); i++){	//подписи фигур
					int id = picture->selectedIDs[i];
					wstring figureName = L"[ Figure " + toWStr( id ) + L" ]";
					Coord center = picture->figures[id]->calculateCenter();
					TextOut(hMemoryDC, center.x - 35, center.y - 7, figureName.c_str(), figureName.length());
				}

				wstring mousePosString = L"[ " + toWStr(MouseCoords.x) + L", " + toWStr(MouseCoords.y) + L" ]";
				TextOut(hMemoryDC, 900, 20, mousePosString.c_str(), mousePosString.length());

				wstring widthString = L"/ " + toWStr(LineWidth) + L" /";
				TextOut(hMemoryDC, 597, 575, widthString.c_str(), widthString.length());
				
				wstring colorStr = L"< " +	toWStrHEX(R/16) + toWStrHEX(R%16) + L"." +
														toWStrHEX(G/16) + toWStrHEX(G%16) + L"." +
														toWStrHEX(B/16) + toWStrHEX(B%16) + L" >";
				SetTextColor(hMemoryDC, Color);
				TextOut(hMemoryDC, 423, 575, colorStr.c_str(), colorStr.length());

				BitBlt(hDC, 0, 0, Width, Height, hMemoryDC, 0, 0, SRCCOPY);

				SelectObject(hMemoryDC, hOldBmp);
				DeleteObject(hMemoryBmp);
				DeleteDC(hMemoryDC);

			EndPaint(hWnd, &ps);
			break;
		}

	//обновление кисти
	if (paramsChanged){
		picture->selected_SetColor(Color);
		picture->selected_SetWidth(LineWidth);
		if ( IFC::getInst()->isInProcess() ){
			IFC::getInst()->clearTask();
			IFC::getInst()->setTask(FT_LAST, LineWidth, Color);
			if (MouseLB)
				IFC::getInst()->fixCurrentCoord();
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND CreateAppWindow(HINSTANCE hInstance){
  WNDCLASS wc;
  wc.style=CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc=WndProc;
  wc.cbClsExtra=0;
  wc.cbWndExtra=0;
  wc.hInstance=hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hbrBackground = WHITE_BRUSH;
  wc.lpszMenuName=0;
  wc.lpszClassName = L"Window_class";

  RegisterClass(&wc);

  return CreateWindow(L"Window_class", L"Figure Editor",
                      WS_SYSMENU|WS_MINIMIZEBOX,
                      CW_USEDEFAULT, CW_USEDEFAULT, Width, Height,
                      NULL, NULL, hInstance, NULL);
}

int StartMsgLoop(HWND hWnd){
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			RECT workspace;
			workspace.left = 0;
			workspace.top = 0;
			workspace.right = Width;
			workspace.bottom = Height - 100;
			InvalidateRect(hWnd, &workspace, true);
			//Sleep(100);
		}
  }

  return msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance,
									HINSTANCE prevInstance, 
								  PSTR cmdLine,
									int showCmd){
	HWND hWnd=CreateAppWindow(hInstance);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//
	HWND buttonLine = CreateWindow( L"button", L"[L]ine", WS_VISIBLE|WS_CHILD, 72*0, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_LINE, NULL, NULL );
	UpdateWindow(buttonLine);
	HWND buttonRectangle = CreateWindow( L"button", L"[R]ect", WS_VISIBLE|WS_CHILD, 72*1, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_RECTANGLE, NULL, NULL );
	UpdateWindow(buttonRectangle);
	HWND buttonEllipse = CreateWindow( L"button", L"[E]llipse", WS_VISIBLE|WS_CHILD, 72*2, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_ELLIPSE, NULL, NULL );
	UpdateWindow(buttonEllipse);
	HWND buttonArc = CreateWindow( L"button", L"[A]rc", WS_VISIBLE|WS_CHILD, 72*3, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_ARC, NULL, NULL );
	UpdateWindow(buttonArc);
	HWND buttonCurve = CreateWindow( L"button", L"[C]urve", WS_VISIBLE|WS_CHILD, 72*4, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_CURVE, NULL, NULL );
	UpdateWindow(buttonCurve);

	HWND buttonRPlus = CreateWindow( L"button", L"R + ", WS_VISIBLE|WS_CHILD, 72*5, 600, 72, 36, hWnd, (HMENU)ID_BUTTON_RPLUS, NULL, NULL );
	UpdateWindow(buttonRPlus);
	HWND buttonRMinus = CreateWindow( L"button", L"R - ", WS_VISIBLE|WS_CHILD, 72*5, 636, 72, 36, hWnd, (HMENU)ID_BUTTON_RMINUS, NULL, NULL );
	UpdateWindow(buttonRMinus);
	HWND buttonGPlus = CreateWindow( L"button", L"G + ", WS_VISIBLE|WS_CHILD, 72*6, 600, 72, 36, hWnd, (HMENU)ID_BUTTON_GPLUS, NULL, NULL );
	UpdateWindow(buttonGPlus);
	HWND buttonGMinus = CreateWindow( L"button", L"G - ", WS_VISIBLE|WS_CHILD, 72*6, 636, 72, 36, hWnd, (HMENU)ID_BUTTON_GMINUS, NULL, NULL );
	UpdateWindow(buttonGMinus);
	HWND buttonBPlus = CreateWindow( L"button", L"B + ", WS_VISIBLE|WS_CHILD, 72*7, 600, 72, 36, hWnd, (HMENU)ID_BUTTON_BPLUS, NULL, NULL );
	UpdateWindow(buttonBPlus);
	HWND buttonBMinus = CreateWindow( L"button", L"B - ", WS_VISIBLE|WS_CHILD, 72*7, 636, 72, 36, hWnd, (HMENU)ID_BUTTON_BMINUS, NULL, NULL );
	UpdateWindow(buttonBMinus);
	HWND buttonWPlus = CreateWindow( L"button", L"Width+", WS_VISIBLE|WS_CHILD, 72*8, 600, 72, 36, hWnd, (HMENU)ID_BUTTON_WPLUS, NULL, NULL );
	UpdateWindow(buttonWPlus);
	HWND buttonWMinus = CreateWindow( L"button", L"Width-", WS_VISIBLE|WS_CHILD, 72*8, 636, 72, 36, hWnd, (HMENU)ID_BUTTON_WMINUS, NULL, NULL );
	UpdateWindow(buttonWMinus);

	HWND buttonMerge = CreateWindow( L"button", L"[M]erge", WS_VISIBLE|WS_CHILD, 72*9, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_MERGE, NULL, NULL );
	UpdateWindow(buttonMerge);
	HWND buttonDivide = CreateWindow( L"button", L"[D]ivide", WS_VISIBLE|WS_CHILD, 72*10, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_DIVIDE, NULL, NULL );
	UpdateWindow(buttonDivide);
	HWND buttonDelete = CreateWindow( L"button", L"[Delete]", WS_VISIBLE|WS_CHILD, 72*11, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL );
	UpdateWindow(buttonDelete);

	HWND buttonSave = CreateWindow( L"button", L"Save", WS_VISIBLE|WS_CHILD, 72*12, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL );
	UpdateWindow(buttonSave);
	HWND buttonOpen = CreateWindow( L"button", L"Open", WS_VISIBLE|WS_CHILD, 72*13, 600, 72, 72, hWnd, (HMENU)ID_BUTTON_OPEN, NULL, NULL );
	UpdateWindow(buttonOpen);
	//

	StartMsgLoop(hWnd);

	IFC::destroy();

	return 0;
}







