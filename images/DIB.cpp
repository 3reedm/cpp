#include "DIB.h"

//Конструктор
DIB::DIB(){
 fileHeadSize=sizeof(BITMAPFILEHEADER);
 fileHead.bfType=0x4d42;
 aBitmapBits=NULL;
}

//Деструктор
DIB::~DIB(){
 if(pInfoHead) delete[] pInfoHead;
 if(aBitmapBits) delete[] aBitmapBits;
 if(outFile) outFile.close();
}

//Создание DIB-секции (в 24-разрядном представлении)
BOOL DIB::CreateDib24(int w, int h, wchar_t* fileName){
 width=w;
 height=h;
 bytesPerLine=((width*24+31)/32)*4;
 imageSize=bytesPerLine*height;

 infoHeadSize=sizeof(BITMAPINFOHEADER);
 fileHead.bfSize=fileHeadSize+infoHeadSize+bytesPerLine*height;
 fileHead.bfOffBits=fileHeadSize+infoHeadSize;

 infoHead.biSize=infoHeadSize;
 infoHead.biWidth=width;
 infoHead.biHeight=height;
 infoHead.biPlanes=1;
 infoHead.biBitCount=24;
 infoHead.biCompression=BI_RGB;
 infoHead.biSizeImage=imageSize;

 aBitmapBits=new BYTE[imageSize];
 memset(aBitmapBits, 0, imageSize);

 outFile.open(fileName, std::ios::out|std::ios::binary|std::ios::trunc);
 if(!outFile) return FALSE;
 else return TRUE;
}

//Загрузка растра из файла
BOOL DIB::LoadFromFile(wchar_t* fileName){
 inFile.open(fileName, std::ios::in|std::ios::binary);
 if(!inFile){
  error="Неверное имя файла или каталога!";
  return FALSE;
 }
 inFile.read((char*)&fileHead, fileHeadSize);
 if(fileHead.bfType!=0x4d42){
  error="Это не BMP-файл";
  inFile.close();
  return FALSE;
 }
 infoHeadSize=fileHead.bfOffBits-fileHeadSize;
 int fileSize=fileHead.bfSize;
 imageSize=fileSize-(fileHeadSize+infoHeadSize);

 pInfoHead=(BITMAPINFOHEADER*)(new BYTE[infoHeadSize]);
 inFile.read((char*)pInfoHead, infoHeadSize);

 width=pInfoHead->biWidth;
 height=pInfoHead->biHeight;

 aBitmapBits=new BYTE[imageSize];
 inFile.read((char*)aBitmapBits, imageSize);
 inFile.close();
 return TRUE;
}

//Вывод на экран изображения
int DIB::Draw(HDC hdc, int xDst, int yDst, int wDst, int hDst, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop){
 return StretchDIBits(hdc, xDst, yDst, wDst, hDst, xSrc, ySrc, wSrc, hSrc, aBitmapBits, (CONST BITMAPINFO*)pInfoHead, DIB_RGB_COLORS, rop);
}

//Вывод по пикселам
void DIB::SetPixel(int x, int y, COLORREF color){
 int row=y;
 int col=3*x;
 
 aBitmapBits[row*bytesPerLine+col]=GetBValue(color);
 aBitmapBits[row*bytesPerLine+col+1]=GetGValue(color);
 aBitmapBits[row*bytesPerLine+col+2]=GetRValue(color);
}

//Сохранение в файл
void DIB::StoreDib24(){
 //Запись заголовка BMP-файла
 outFile.write((char*)&fileHead,fileHeadSize);
 outFile.write((char*)&infoHead,infoHeadSize);
 //Запись массива пикселов
 outFile.write((char*)aBitmapBits,imageSize);
 outFile.close();
}

void DIB::SaveToFile(wchar_t* fileName){
 //Открытие файла
 outFile.open(fileName, std::ios::out|std::ios::binary|std::ios::trunc);
 //Запись заголовка BMP-файла
 outFile.write((char*)&fileHead,fileHeadSize);
 outFile.write((char*)pInfoHead,infoHeadSize);
 //Запись массива пикселов
 outFile.write((char*)aBitmapBits,imageSize);
 outFile.close();
}


//Фильтр
void DIB::FilterHF(){
 bytesPerLine=((width*24+31)/32)*4;
 BYTE *aBitmapBitsTmp=new BYTE[imageSize];
 int weight[9];
 int x,y,row,col; 

 //Заполнение матрицы весов
 weight[0]=16;
 weight[1]=8;
 weight[2]=16;
 weight[3]=8;
 weight[4]=4;
 weight[5]=8;
 weight[6]=16;
 weight[7]=8;
 weight[8]=16;

 //Обнуление массива, в который будут записываться пикселы с уже изменёнными цветами (чтобы не было муссора)
 for(y=0; y<height; ++y){
  row=y;
  for(x=0; x<width; ++x){
   col=3*x;
   aBitmapBitsTmp[row*bytesPerLine+col]=0;
   aBitmapBitsTmp[row*bytesPerLine+col+1]=0;
   aBitmapBitsTmp[row*bytesPerLine+col+2]=0;
  }
 }

 //Заполнение этого массива
 for(y=0; y<height; ++y){
  row=y;
  for(x=0; x<width; ++x){
   col=3*x;
	
   if((row-1)>0 && row*bytesPerLine+col-3>=row*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col-3]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col-2]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col-1]/weight[0];
   }
   else if((row-1)>0 && row*bytesPerLine+col-1<row*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col+3]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col+4]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col+5]/weight[2];
   }
   else if((row-1)<=0 && row*bytesPerLine+col-3>=row*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col-3]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col-2]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col-1]/weight[6];
   }
   else if((row-1)<=0 && row*bytesPerLine+col-1<row*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col+3]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col+4]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col+5]/weight[8];
   }

   if((row-1)>0)
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col]/weight[1];
   else
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col]/weight[7];
   if((row-1)>0)
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col+1]/weight[1];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col+1]/weight[7];
   if((row-1)>0)
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col+2]/weight[1];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col+2]/weight[7];
	
   if((row-1)>0 && row*bytesPerLine+col+5<(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col+3]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col+4]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col+5]/weight[2];
   }
   else if((row-1)>0 && row*bytesPerLine+col+3>=(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col-3]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col-2]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col-1]/weight[0];
   }
   else if((row-1)<=0 && row*bytesPerLine+col+5<(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col+3]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col+4]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col+5]/weight[8];
   }
   else if((row-1)<=0 && row*bytesPerLine+col+3>=(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col-1]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col-3]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col-2]/weight[6];
   }

   if(row*bytesPerLine+col-3>=row*bytesPerLine)
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[row*bytesPerLine+col-3]/weight[3]; 
   else
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[row*bytesPerLine+col+3]/weight[5];
   if(row*bytesPerLine+col-2>row*bytesPerLine)
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[row*bytesPerLine+col-2]/weight[3];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[row*bytesPerLine+col+4]/weight[5];
   if(row*bytesPerLine+col-1>row*bytesPerLine)
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[row*bytesPerLine+col-1]/weight[3];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[row*bytesPerLine+col+5]/weight[5];

   aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[row*bytesPerLine+col]/weight[4];
   aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[row*bytesPerLine+col+1]/weight[4];
   aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[row*bytesPerLine+col+2]/weight[4];

   if(row*bytesPerLine+col+3<row*bytesPerLine+3*(width-1))
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[row*bytesPerLine+col+3]/weight[5];
   else
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[row*bytesPerLine+col-3]/weight[3];
   if(row*bytesPerLine+col+4<row*bytesPerLine+3*(width-1))
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[row*bytesPerLine+col+4]/weight[5];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[row*bytesPerLine+col-2]/weight[3];
   if(row*bytesPerLine+col+5<row*bytesPerLine+3*(width-1))
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[row*bytesPerLine+col+5]/weight[5];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[row*bytesPerLine+col-1]/weight[3];

   if((row+1)<(height-1)*bytesPerLine && row*bytesPerLine+col-3>=row*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col-3]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col-2]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col-1]/weight[6];
   }
   else if((row+1)<(height-1)*bytesPerLine && row*bytesPerLine+col-1<row*bytesPerLine){
    aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col+3]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col+4]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col+5]/weight[8];
   }
   else if((row+1)>=(height-1)*bytesPerLine && row*bytesPerLine+col-3>=row*bytesPerLine){
    aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col-3]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col-2]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col-1]/weight[0];
   }
   else if((row+1)>=(height-1)*bytesPerLine && row*bytesPerLine+col-1<row*bytesPerLine){
    aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col+3]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col+4]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col+5]/weight[2];
   }   

   if((row+1)<(height-1)*bytesPerLine)
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col]/weight[7];
   else
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col]/weight[1];
   if((row+1)<(height-1)*bytesPerLine)
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col+1]/weight[7];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col+1]/weight[1];
   if((row+1)<(height-1)*bytesPerLine)
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col+2]/weight[7];
   else
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col+2]*weight[1];
	
   if((row+1)<(height-1)*bytesPerLine && row*bytesPerLine+col+5<(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col+3]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col+4]/weight[8];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col+5]/weight[8];
   }
   else if((row+1)<(height-1)*bytesPerLine && row*bytesPerLine+col+3>=(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row+1)*bytesPerLine+col-3]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row+1)*bytesPerLine+col-2]/weight[6];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row+1)*bytesPerLine+col-1]/weight[6];
   }
   else if((row+1)>=(height-1)*bytesPerLine && row*bytesPerLine+col+5<(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col+3]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col+4]/weight[2];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col+5]/weight[2];
   }
   else if((row+1)>=(height-1)*bytesPerLine && row*bytesPerLine+col+3>=(row+1)*bytesPerLine){
	aBitmapBitsTmp[row*bytesPerLine+col]+=aBitmapBits[(row-1)*bytesPerLine+col-3]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+1]+=aBitmapBits[(row-1)*bytesPerLine+col-2]/weight[0];
	aBitmapBitsTmp[row*bytesPerLine+col+2]+=aBitmapBits[(row-1)*bytesPerLine+col-1]/weight[0];
   }
  }
 }
 
 //Обратное присваивание (хотя можно оставить две картинки)
 for(y=0; y<height; ++y){
  row=y;
  for(x=0; x<width; ++x){
   col=3*x;
   aBitmapBits[row*bytesPerLine+col]=aBitmapBitsTmp[row*bytesPerLine+col];
   aBitmapBits[row*bytesPerLine+col+1]=aBitmapBitsTmp[row*bytesPerLine+col+1];
   aBitmapBits[row*bytesPerLine+col+2]=aBitmapBitsTmp[row*bytesPerLine+col+2];
  }
 }
}

void DIB::Hist(){
 int i,j,row,col;
 bytesPerLine=((width*24+31)/32)*4;
 aHistogramBits=new BYTE*[3];

 //Заполнение элементов гистограммы 0
 for(i=0; i<3; ++i){
  aHistogramBits[i]=new BYTE[256];
  for(j=0; j<256; ++j)
   aHistogramBits[i][j]=0;
 }

 //Построение гистограммы исходного изображения
 for(j=0; j<height; ++j){
  row=j;
  for(i=0; i<width; ++i){
   col=3*i;
   aHistogramBits[0][aBitmapBits[row*bytesPerLine+col+2]]++;
   aHistogramBits[1][aBitmapBits[row*bytesPerLine+col+1]]++;
   aHistogramBits[2][aBitmapBits[row*bytesPerLine+col]]++;
  }
 }
}

//Выравнивание гистограммы
void DIB::HistStretch(){
 int i,j,row,col,max[3],min[3];
 double a[3],b[3];
 bytesPerLine=((width*24+31)/32)*4;

 //Выбор максимального и минимального уровней яркости и нахождение коэффициентов новой яркости
 for(i=0; i<3; ++i){
  max[i]=aBitmapBits[i];
  min[i]=aBitmapBits[i];
 }
 for(i=0; i<height; ++i){
  row=i;
  for(j=0; j<width; ++j){
   col=3*j;
   if(aBitmapBits[row*bytesPerLine+col]>max[0])
	max[0]=aBitmapBits[row*bytesPerLine+col];
   if(aBitmapBits[row*bytesPerLine+col]<min[0])
	min[0]=aBitmapBits[row*bytesPerLine+col];
   if(aBitmapBits[row*bytesPerLine+col+1]>max[1])
	max[1]=aBitmapBits[row*bytesPerLine+col+1];
   if(aBitmapBits[row*bytesPerLine+col+1]<min[1])
	min[1]=aBitmapBits[row*bytesPerLine+col+1];
   if(aBitmapBits[row*bytesPerLine+col+2]>max[2])
	max[2]=aBitmapBits[row*bytesPerLine+col+2];
   if(aBitmapBits[row*bytesPerLine+col+2]<min[2])
	min[2]=aBitmapBits[row*bytesPerLine+col+2];
  }
 }
 for(i=0; i<3; ++i){
  b[i]=255./(max[i]-min[i]);
  a[i]=(-b[i])*min[i];
 }

 //Обнуление гистограммы
 for(i=0; i<3; ++i)
  for(j=0; j<256; ++j)
   aHistogramBits[i][j]=0;

 //Построение изменённой гистограммы изображения
 for(j=0; j<height; ++j){
  row=j;
  for(i=0; i<width; ++i){
   col=3*i;
   aBitmapBits[row*bytesPerLine+col]=a[0]+b[0]*aBitmapBits[row*bytesPerLine+col];
   aBitmapBits[row*bytesPerLine+col+1]=a[1]+b[1]*aBitmapBits[row*bytesPerLine+col+1];
   aBitmapBits[row*bytesPerLine+col+2]=a[2]+b[2]*aBitmapBits[row*bytesPerLine+col+2];
   aHistogramBits[0][aBitmapBits[row*bytesPerLine+col+2]]++;
   aHistogramBits[1][aBitmapBits[row*bytesPerLine+col+1]]++;
   aHistogramBits[2][aBitmapBits[row*bytesPerLine+col]]++;
  }
 }
}


//Вывод на экран гистограммы
void DIB::DrawHist(HDC hdc){
 int i,j,k,max,h=200,step=2,color=255;
 HPEN hPen;

 //Построение трёх гистограмм (R,G,B)
 for(k=0; k<3; ++k){

  //Выбор цвета
  if(k==0)
   hPen=CreatePen(PS_SOLID,1,RGB(color,0,0));
  if(k==1)
   hPen=CreatePen(PS_SOLID,1,RGB(0,color,0));
  if(k==2)
   hPen=CreatePen(PS_SOLID,1,RGB(0,0,color));
  SelectObject(hdc, hPen);

  //Нахождение максимального элемента гистограммы
  max=aHistogramBits[k][0];
  for(i=1; i<256; ++i)
   if(aHistogramBits[k][i]>max)
    max=aHistogramBits[k][i];

  //Вывод гистограммы на экран
  if(max!=0)
   for(i=0; i<256; ++i)
    for(j=i; j<i+step; ++j){
     MoveToEx(hdc, i+j, h*(k+1), NULL);
     LineTo(hdc, i+j, h*(k+1)-(h-5)*aHistogramBits[k][i]/max);
    }
 }

 DeleteObject(SelectObject(hdc, hPen));
}

/*
BOOL DIB::Order(BYTE p, int i){
 int j,n=v[i].size();
 for(j=0; j<n; ++j)
  if(p==v[i][j])
   return TRUE;
 return FALSE;
}

//Выравнивание гистограммы
void DIB::HistStretch(){
 int i,j,row,col,max[3],min[3],a[3],b[3];
 bytesPerLine=((width*24+31)/32)*4;
 
 for(i=0; i<3; ++i)
  for(j=0; j<256; ++j)
   if(aHistogramBits[i][j]<10)
	v[i].push_back(j);

 //Выбор максимального и минимального уровней яркости и нахождение коэффициентов новой яркости
 for(i=0; i<3; ++i){
  max[i]=aBitmapBits[i];
  min[i]=aBitmapBits[i];
 }
 for(i=0; i<height; ++i){
  row=i;
  for(j=0; j<width; ++j){
   col=3*j;
   if(aBitmapBits[row*bytesPerLine+col]>max[0] && !Order(aBitmapBits[row*bytesPerLine+col],2))
	max[0]=aBitmapBits[row*bytesPerLine+col];
   if(aBitmapBits[row*bytesPerLine+col]<min[0] && !Order(aBitmapBits[row*bytesPerLine+col],2))
	min[0]=aBitmapBits[row*bytesPerLine+col];
   if(aBitmapBits[row*bytesPerLine+col+1]>max[1] && !Order(aBitmapBits[row*bytesPerLine+col+1],1))
	max[1]=aBitmapBits[row*bytesPerLine+col+1];
   if(aBitmapBits[row*bytesPerLine+col+1]<min[1] && !Order(aBitmapBits[row*bytesPerLine+col+1],1))
	min[1]=aBitmapBits[row*bytesPerLine+col+1];
   if(aBitmapBits[row*bytesPerLine+col+2]>max[2] && !Order(aBitmapBits[row*bytesPerLine+col+2],0))
	max[2]=aBitmapBits[row*bytesPerLine+col+2];
   if(aBitmapBits[row*bytesPerLine+col+2]<min[2] && !Order(aBitmapBits[row*bytesPerLine+col+2],0))
	min[2]=aBitmapBits[row*bytesPerLine+col+2];
  }
 }
 for(i=0; i<3; ++i){
  b[i]=255/(max[i]-min[i]);
  a[i]=(-b[i])*min[i];
 }

 //Построение изменённой гистограммы изображения
 for(j=0; j<height; ++j){
  row=j;
  for(i=0; i<width; ++i){
   col=3*i;
   if(!Order(aBitmapBits[row*bytesPerLine+col],2)){
    aBitmapBits[row*bytesPerLine+col]=a[0]+b[0]*aBitmapBits[row*bytesPerLine+col];
    aHistogramBits[2][aBitmapBits[row*bytesPerLine+col]]++;
   }
   if(!Order(aBitmapBits[row*bytesPerLine+col+1],1))
    aBitmapBits[row*bytesPerLine+col+1]=a[1]+b[1]*aBitmapBits[row*bytesPerLine+col+1];
    aHistogramBits[1][aBitmapBits[row*bytesPerLine+col+1]]++;
   }
   if(!Order(aBitmapBits[row*bytesPerLine+col+2],0)){
    aBitmapBits[row*bytesPerLine+col+2]=a[2]+b[2]*aBitmapBits[row*bytesPerLine+col+2];
    aHistogramBits[0][aBitmapBits[row*bytesPerLine+col+2]]++;
   }
 }
}*/