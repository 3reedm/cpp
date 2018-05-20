#include <windows.h>
#include <fstream>
#include <string>

class DIB{
 public:
  DIB();
  ~DIB();
  BOOL CreateDib24(int, int, wchar_t*);
  BOOL LoadFromFile(wchar_t*);
  void FilterHF();
  void Hist();
  void HistStretch();
  void StoreDib24();
  void SaveToFile(wchar_t*);
  void SetPixel(int, int, COLORREF);
  int Draw(HDC, int, int, int, int, int, int, int, int, DWORD);
  void DrawHist(HDC hdc);
  int GetWidth(){return width;}
  int GetHeight(){return height;}
  const char* GetError(){return error.c_str();}
  BOOL GetInfo(){if(aBitmapBits) return TRUE; else return FALSE;}
  BOOL GetHistInfo(){if(aHistogramBits) return TRUE; else return FALSE;}

 private:
  int width;
  int height;
  int bytesPerLine;
  BITMAPFILEHEADER fileHead;
  BITMAPINFOHEADER infoHead;
  BITMAPINFOHEADER *pInfoHead;
  BYTE *aBitmapBits;
  BYTE **aHistogramBits;
  int fileHeadSize;
  int infoHeadSize;
  int imageSize;
  std::string error;
  std::ifstream inFile;
  std::ofstream outFile;
};


