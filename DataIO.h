#pragma once

#ifndef DATAIO_H
#define DATAIO_H
#define _CRT_SECURE_NO_WARNINGS

char* loadTextFile(const wchar_t *);
int glPrintError(char *, int);
void print_formatted_words(const char*, bool);
int printInfoLog(GLhandleARB);

GLuint LoadTextureBMPWH(const char * filename, int height, int width);

#endif