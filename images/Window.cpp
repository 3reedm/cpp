#include "Window.h"

#define CM_FILE_OPEN 1001
#define CM_FILE_SAVE 1002
#define CM_FILE_QUIT 1003
#define CM_FILTER_HF 2001
#define CM_HISTOGRAM 3001
#define CM_HISTSTRETCH 3002
#define WIDTH 600
#define HEIGHT 400

typedef enum _ParamsFileWin{_FOR_OPEN, _FOR_SAVE} ParamsFileWin;
wchar_t* path=new wchar_t[10];

BOOL RegClass(WNDPROC,LPCWSTR,UINT);
LRESULT CALLBACK PaintWndProc(HWND,UINT,UINT,LONG);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){

 MSG Msg;
 HWND hWnd;
 RECT rect;
 int sHeight, sWidth;

 GetWindowRect(GetDesktopWindow(),&rect);
 sHeight=rect.bottom/4, sWidth=rect.right/4;
 

 if(!RegClass(PaintWndProc, L"WndPaintClass", COLOR_WINDOW)){
  MessageBox(NULL, L"Cannot register class", L"Error", MB_OK);
  return FALSE;
 }
 
 hWnd=CreateWindow(L"WndPaintClass", L"Обработка изображений", WS_SYSMENU|WS_MINIMIZEBOX|WS_CAPTION|WS_OVERLAPPED, sWidth, sHeight, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

 if(!hWnd){
  MessageBox(NULL, L"Cannot create window", L"Error", MB_OK);
  return FALSE;
 }

 ShowWindow(hWnd,nCmdShow);
 UpdateWindow(hWnd);

 while(GetMessage(&Msg,NULL,0,0)){
  TranslateMessage(&Msg);
  DispatchMessage(&Msg);
 }

 return Msg.wParam;
}

BOOL RegClass(WNDPROC WndProc, LPCWSTR szName, UINT brBackground){
 WNDCLASSEX WndClass;

 WndClass.cbSize=sizeof(WndClass);
 WndClass.style=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
 WndClass.lpfnWndProc=WndProc;
 WndClass.cbClsExtra=0;
 WndClass.cbWndExtra=0;
 WndClass.hInstance=GetModuleHandle(NULL);
 WndClass.hIcon=LoadIcon(NULL, IDI_APPLICATION);
 WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
 WndClass.hbrBackground=NULL;
 WndClass.lpszMenuName=NULL;
 WndClass.lpszClassName=szName;
 WndClass.hIconSm=LoadIcon(NULL, IDI_APPLICATION);
 return (RegisterClassEx(&WndClass)!=0);
}

BOOL CreateMenuItem(HMENU hMenu, LPWSTR str, UINT uIns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType){
 MENUITEMINFO MenuItemInfo;

 MenuItemInfo.cbSize=sizeof(MENUITEMINFO);
 MenuItemInfo.fMask=MIIM_STATE|MIIM_TYPE|MIIM_SUBMENU|MIIM_ID;
 MenuItemInfo.fType=fType;
 MenuItemInfo.fState=MFS_ENABLED;
 MenuItemInfo.dwTypeData=str;
 MenuItemInfo.cch=sizeof(str);
 MenuItemInfo.wID=uCom;
 MenuItemInfo.hSubMenu=hSubMenu;
 return InsertMenuItem(hMenu, uIns, flag, &MenuItemInfo);
}

wchar_t* GetFileNameW(const ParamsFileWin attrib){
  static wchar_t file_name[1024]={0};
  static OPENFILENAMEW file_params={0};

  file_params.lStructSize=sizeof(OPENFILENAME);
  file_params.hwndOwner=NULL;
  file_params.lpstrFilter=L"Все файлы (*.*)\0*.*\0";
  file_params.lpstrFile=file_name;
  file_params.nMaxFile=1024;
  file_params.lpstrInitialDir=0;
  file_params.lpstrFileTitle=0;
  file_params.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_NOTESTFILECREATE;

  switch(attrib){
    case _FOR_OPEN:
     if(GetOpenFileNameW(&file_params))
       return file_params.lpstrFile;      
     break;
    case _FOR_SAVE:
     if(GetSaveFileNameW(&file_params))
       return file_params.lpstrFile;
     break;
  }  
 return 0;
}

LRESULT CALLBACK PaintWndProc(HWND hWnd,UINT Message,UINT wParam,LONG lParam){ 
 HMENU hMainMenu, hFileMenu, hFilterMenu, hHistogramMenu;

 static HRGN hRgnWnd;
 static PAINTSTRUCT ps;
 static HDC hdc;

 static DIB bmp;
 static BOOL isFileLoaded, isHistogramCreate;
 int dX, dY, ws, hs, wd, hd;
 RECT rect;

 int i;

 switch(Message){
  case WM_CREATE:
   hMainMenu=CreateMenu();
   hFileMenu=CreatePopupMenu();
   hFilterMenu=CreatePopupMenu();
   hHistogramMenu=CreatePopupMenu();
   i=0;
   CreateMenuItem(hFileMenu, L"&Открыть", i++, CM_FILE_OPEN, NULL, FALSE, MFT_STRING);
   CreateMenuItem(hFileMenu, L"&Сохранить", i++, CM_FILE_SAVE, NULL, FALSE, MFT_STRING);
   CreateMenuItem(hFileMenu, NULL, i++, 0, NULL, FALSE, MFT_SEPARATOR);
   CreateMenuItem(hFileMenu, L"&Выход", i++, CM_FILE_QUIT, NULL, FALSE, MFT_STRING);
   i=0;
   CreateMenuItem(hFilterMenu, L"&Высоких частот", i++, CM_FILTER_HF, NULL, FALSE, MFT_STRING);
   i=0;
   CreateMenuItem(hHistogramMenu, L"&Построение гистограммы", i++, CM_HISTOGRAM, NULL, FALSE, MFT_STRING);
   CreateMenuItem(hHistogramMenu, L"&Выравнивание гистограммы", i++, CM_HISTSTRETCH, NULL, FALSE, MFT_STRING);
   i=0;
   CreateMenuItem(hMainMenu, L"&Файл", i++, 0, hFileMenu, FALSE, MFT_STRING);
   CreateMenuItem(hMainMenu, L"Ф&ильтры", i++, 0, hFilterMenu, FALSE, MFT_STRING);
   CreateMenuItem(hMainMenu, L"&Гистограмма", i++, 0, hHistogramMenu, FALSE, MFT_STRING);
   SetMenu(hWnd, hMainMenu);
   DrawMenuBar(hWnd);
   break;
  case WM_COMMAND:
   switch(LOWORD(wParam)){
    case CM_FILE_OPEN:
	 path=GetFileNameW(_FOR_OPEN);
	 if(path){
	  isFileLoaded=bmp.LoadFromFile(path);
	  if(!isFileLoaded){
	   MessageBoxA(hWnd, "Файл не загружен.", "Error", MB_OK);
	   MessageBoxA(hWnd, bmp.GetError(), "Error", MB_OK);
	   break;
	  }
	 }
	 else
	  break;

	 //Подогнать размер окна программы под размер растра bmp
	 GetClientRect(hWnd, &rect);
	 dX=bmp.GetWidth()-rect.right;
	 dY=bmp.GetHeight()-rect.bottom;
	 GetWindowRect(hWnd, &rect);
	 InflateRect(&rect, dX/2, dY/2);
	 MoveWindow(hWnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, TRUE);

	 InvalidateRgn(hWnd,hRgnWnd,FALSE);
     break;
	case CM_FILE_SAVE:
	 path=GetFileNameW(_FOR_SAVE);
	 if(path!=NULL)
	  if(bmp.GetInfo())
	   bmp.SaveToFile(path);
	 break;
	case CM_FILE_QUIT:
     DestroyWindow(hWnd);
	 break;     
	case CM_FILTER_HF:
	 bmp.FilterHF();
	 InvalidateRgn(hWnd,hRgnWnd,FALSE);
	 break;
	case CM_HISTOGRAM:
	 if(bmp.GetInfo()){
	  //Подогнать размер окна для гистограммы
	  GetClientRect(hWnd, &rect);
	  dX=512-rect.right;
	  dY=600-rect.bottom;
	  GetWindowRect(hWnd, &rect);
	  InflateRect(&rect, dX/2, dY/2);
	  MoveWindow(hWnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, TRUE);

	  //Построить гистограмму
	  bmp.Hist();
	  isHistogramCreate=TRUE;
	  InvalidateRgn(hWnd,hRgnWnd,FALSE);
	 }
     break;
	case CM_HISTSTRETCH:
     if(bmp.GetHistInfo()){
	  bmp.HistStretch();
	  InvalidateRgn(hWnd,hRgnWnd,FALSE);
	 }
	 break;
   }
   break;
  case WM_SIZE:
   ps.rcPaint.right=LOWORD(lParam);
   ps.rcPaint.bottom=HIWORD(lParam);
   hRgnWnd=CreateRectRgn(0,0,ps.rcPaint.right,ps.rcPaint.bottom);
   break;
  case WM_PAINT:
   hdc=BeginPaint(hWnd,&ps);
   if(isFileLoaded && !isHistogramCreate){
    wd=ws=bmp.GetWidth()+1;
	hd=hs=bmp.GetHeight()+1;
	BitBlt(hdc, 0, 0, wd, hd, NULL, ws, hs, WHITENESS);
    bmp.Draw(hdc, 0, 0, wd, hd, 0, 0, ws, hs, SRCCOPY);
   }
   else if(isHistogramCreate){
    wd=ws=512;
	hd=hs=600;
	BitBlt(hdc, 0, 0, wd, hd, NULL, ws, hs, WHITENESS);
    bmp.DrawHist(hdc);
   }
   else if(!isFileLoaded && !isHistogramCreate)
    BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, NULL, 0, 0, WHITENESS);
   EndPaint(hWnd,&ps);
   break;
  case WM_DESTROY:
   PostQuitMessage(0);
   return 0;
  default:
   return DefWindowProc(hWnd,Message,wParam,lParam);
 }
 return 0;
}