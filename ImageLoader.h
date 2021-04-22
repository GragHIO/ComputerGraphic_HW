#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef IMAGELOADER_H
#define IMAGELOADER_H
/*------------------------------------------------------
* Procedure to read a BMP file and keep the results in
* a gray-level array. The intensities are of float type.
* Input:
*   filename[]: name of the input file.
* Output:
*   width, height: width and height of the image data.
*   imgData[]: array to keep the intensity.
* Return 0, if successful, -1, if fail.
*/
int  readBMPfile(char *filename, int *width, int *height, unsigned char **imgData);

unsigned int LoadTextureBMP(const char*, bool = false);
#endif