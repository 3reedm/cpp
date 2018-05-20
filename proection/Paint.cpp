#include "Paint.h"

gengine* gdiplus=new gengine_plus;

BOOL RegClass(WNDPROC,LPCWSTR,UINT);
LRESULT CALLBACK PaintWndProc(HWND,UINT,UINT,LONG);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){

 MSG Msg;
 HWND hWnd;
 gdiplus->start();

 if(!RegClass(PaintWndProc, L"WndPaintClass", COLOR_WINDOW)){
  MessageBox(NULL, L"Cannot register class", L"Error", MB_OK);
  return FALSE;
 }
 
 hWnd=CreateWindow(L"WndPaintClass", L"Перспективная проекция", WS_POPUP, 200, 100, 1009, 488, NULL, NULL, hInstance, NULL);

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
 
 gdiplus->stop();
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

LRESULT CALLBACK PaintWndProc(HWND hWnd,UINT Message,UINT wParam,LONG lParam){  
 static BOOL bTracking=FALSE, bCube=TRUE, bPiramid=FALSE; 
 static int sx, sy;
 static PAINTSTRUCT ps;
 static apex p;
 static figure* f=new figure[2];
 static std::vector<figure> mf;
 static HRGN hRgn;
 HBITMAP hBitmap;
 HDC hdcMem;
 HDC hdc;


 switch(Message){
  case WM_CREATE:
   f[0].add(apex(-0.5,-0.5,-0.5)); 
   f[0].add(apex(-0.5,-0.5,0.5));
   f[0].add(apex(-0.5,0.5,-0.5));
   f[0].add(apex(-0.5,0.5,0.5));
   f[0].add(apex(0.5,-0.5,-0.5));
   f[0].add(apex(0.5,-0.5,0.5));
   f[0].add(apex(0.5,0.5,-0.5));
   f[0].add(apex(0.5,0.5,0.5));
   f[0].create();
   mf.push_back(f[0]);
   f[1].add(apex(-0.5,-0.5,0.5)); 
   f[1].add(apex(0.5,-0.5,0.5));
   f[1].add(apex(-0.5,-0.5,-0.5));
   f[1].add(apex(0.5,-0.5,-0.5));
   f[1].add(apex(0,0.5,0));
   f[1].create();
   mf.push_back(f[1]);
   break;
  case WM_SIZE:
   sx=LOWORD(lParam);
   sy=HIWORD(lParam);
   hRgn=CreateRectRgn(0,0,sx,sy);
   break;
  case WM_KEYDOWN:
   switch(wParam){
     case VK_UP:
	  if(bCube==TRUE)
	   mf[0].twist(0);
	  if(bPiramid==TRUE)
	   mf[1].twist(0);
	  InvalidateRgn(hWnd,hRgn,FALSE);
	 case VK_DOWN:
      if(bCube==TRUE)
	   mf[0].twist(1);
	  if(bPiramid==TRUE)
	   mf[1].twist(1);
	  InvalidateRgn(hWnd,hRgn,FALSE);
	  break;
	 case VK_LEFT:
	  if(bCube==TRUE)
	   mf[0].twist(2);
	  if(bPiramid==TRUE)
	   mf[1].twist(2);
	  InvalidateRgn(hWnd,hRgn,FALSE);
	  break;
	 case VK_RIGHT:
	  if(bCube==TRUE)
	   mf[0].twist(2);
	  if(bPiramid==TRUE)
	   mf[1].twist(2);
	  InvalidateRgn(hWnd,hRgn,FALSE);
	  break;
	 case VK_PRIOR:
      if(bCube==TRUE)
	   mf[0].distance(0);
	  if(bPiramid==TRUE)
	   mf[1].distance(0);
	  InvalidateRgn(hWnd,hRgn,FALSE);
	  break;
	 case VK_NEXT:
      if(bCube==TRUE)
	   mf[0].distance(1);
	  if(bPiramid==TRUE)
	   mf[1].distance(1);
	  InvalidateRgn(hWnd,hRgn,FALSE);
	  break;
   }
   break;
  case WM_CHAR:
   switch(wParam){
   case 'c':
	if(bCube==FALSE){
	 bPiramid=FALSE;
     bCube=TRUE;
	}
	break;
   case 'p':
	if(bPiramid==FALSE){
	 bCube=FALSE;
	 bPiramid=TRUE;
	}
   }
   break;
  case WM_LBUTTONDOWN:
   bTracking=TRUE;
   break;
  case WM_MOUSEMOVE:
   if(bTracking){
	p.x=LOWORD(lParam)-sx/2;
    p.y=HIWORD(lParam)-sy/2;
	if(p.x>=0){
	 if(bCube==TRUE)
	  mf[0].twist(0);
	 if(bPiramid==TRUE)
	  mf[1].twist(0);
	}
	if(p.x<0 && p.y>=0){
	 if(bCube==TRUE)
	  mf[0].twist(1);
	 if(bPiramid==TRUE)
	  mf[1].twist(1);
	}
	if(p.x<0 && p.y<0){
	 if(bCube==TRUE)
	  mf[0].twist(2);
	 if(bPiramid==TRUE)
	  mf[1].twist(2);
	}
	InvalidateRgn(hWnd,hRgn,FALSE);
   }
   break;
  case WM_LBUTTONUP:
   if(bTracking){
	bTracking=FALSE;
	InvalidateRgn(hWnd,hRgn,FALSE);
   }
   break;
  case WM_PAINT:
   hdc=BeginPaint(hWnd, &ps);
   SetMapMode(hdc, MM_ANISOTROPIC);
   hdcMem=CreateCompatibleDC(hdc);
   hBitmap=CreateCompatibleBitmap(hdc, sx, sy);
   SelectObject(hdcMem, hBitmap);
   BitBlt(hdcMem,0,0,sx,sy,NULL,0,0,WHITENESS);
   gdiplus->getHDC(hdcMem);
   gdiplus->getO(sx/2,sy/2);
   if(mf.size()!=NULL)
    for(unsigned i=0; i<mf.size(); ++i)
     mf[i].draw(gdiplus);
   BitBlt(hdc,0,0,sx,sy,hdcMem,0,0,SRCCOPY);
   SelectObject(hdcMem, hBitmap);
   DeleteObject(hBitmap);
   DeleteDC(hdcMem);
   EndPaint(hWnd, &ps);
   break;
  case WM_DESTROY:
   DeleteObject(hRgn);
   PostQuitMessage(0);
   return 0;
  default:
   return DefWindowProc(hWnd,Message,wParam,lParam);
 }
 return 0;
}