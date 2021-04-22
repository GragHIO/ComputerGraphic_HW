#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef BMPTYPE_H
#define BMPTYPE_H
typedef unsigned int       DWORD;
typedef unsigned char      BYTE;
typedef unsigned short int WORD;
typedef unsigned int       LONG;

typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize; 
  LONG   biWidth; 
  LONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER { 
  unsigned short        bfType; 
  unsigned int          bfSize; 
  unsigned short        bfReserved1; 
  unsigned short        bfReserved2; 
  unsigned int          bfOffBits; 
} BITMAPFILEHEADER;

typedef struct tagRGBTRIPLE { 
  BYTE rgbtBlue; 
  BYTE rgbtGreen; 
  BYTE rgbtRed; 
} RGBTRIPLE; 

#endif