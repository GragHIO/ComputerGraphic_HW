#define _CRT_SECURE_NO_WARNINGS
#include "HIO_include.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <io.h>

//#pragma comment(lib, "glew32.lib")
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;
#include "Polyhedron.h"

#include "DataIO.h"


GLuint LoadTextureBMPWH(const char * filename, int height, int width)
{
	GLuint texture;

	unsigned char * data;

	FILE * file;

	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}

int* parseInfoLog(const char* infoLog)
{
	int warning_count = 0, error_count = 0, other_count = 0;
	string warning_str(""), error_string(""), other_string("");
	if (infoLog) {
		istringstream f(infoLog);
		string line;
		for (int i = 1; getline(f, line); i++) {
			if (line.length() <= 0)break;
			istringstream message(line);
			string msgType, msgText, msgMsg, buffer, buffer_msg("");
			char TmpBuffer[255], *must_free_buffer;
			int line_data, char_at;
			message >> msgType; // ERROR:
			int msgType_length = msgType.length();
			must_free_buffer = (char*)malloc((size_t)msgType_length * (sizeof(char)));
			for (int j = 0; j < msgType_length - 1; must_free_buffer[j++] = msgType.at((size_t)j));
			must_free_buffer[msgType_length - 1] = '\0';
			msgType = string(must_free_buffer);
			if (!strcmp(msgType.c_str(), "WARNING")) { 
				sprintf(TmpBuffer,"%d", ++warning_count);
				warning_str += " ";
				warning_str += TmpBuffer;
				warning_str += ": " + line + '\n';
			} else if (!strcmp(msgType.c_str(), "ERROR")) { 
				sprintf(TmpBuffer, "%d", ++error_count);
				error_string += " ";
				error_string += TmpBuffer;
				error_string += ": " + line + '\n';
			} else {
				sprintf(TmpBuffer, "%d", ++other_count);
				other_string += " ";
				other_string += TmpBuffer;
				other_string += ": " + line + '\n';
			}
			message >> TmpBuffer;
			sscanf(TmpBuffer, "%d:%d:", &line_data, &char_at);
			//message >> line_data >> TmpBuffer >> char_at >> TmpBuffer; // 30:0
			int j = 0;
			bool isloading = false;
			bool txtFished = false;
			while (getline(message, buffer)) {
				int n = buffer.length();
				for (int k = 0; k < n; k++,j++) {
					char now_char = buffer.at((size_t)k);
					if (isloading) {
						if (now_char == '\'') {
							txtFished = true;
							msgText = string(buffer_msg);
							buffer_msg = string("");
							continue;
						}
						if (txtFished) {
							if (now_char == ':') {
								isloading = false;
								continue;
							}
						}
						buffer_msg += now_char;
					}else{
						if (txtFished) {
							buffer_msg += now_char;
							continue;
						}
						if (now_char == '\'') {
							isloading = true;
							continue;
						}
					}
				}
			}
			msgMsg = string(buffer_msg);
			//cout << i << " : " << msgType << ": " << "²Ä" << line_data << "¦E²Ä" << char_at << "ÄE << " : " << msgText << " ¸ET:" << msgMsg << endl;
		}
	}
	printf("%s(%d): \n","Errors: ", error_count);
	print_formatted_words(error_string.c_str(), false);

	printf("%s(%d): \n", "Warnings: ", warning_count);
	print_formatted_words(warning_str.c_str(), false);

	printf("%s(%d): \n", "Inforamtions: ", other_count);
	print_formatted_words(other_string.c_str(), false);
	return new int[2] {warning_count, error_count};
}

int printInfoLog(GLhandleARB obj)
{
	int infologLength = 0;
	int charsWritten = 0, *message_count;
	char *infoLog;
	int error_to_start = 0;
	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
		&infologLength);
	printf("*************** shader output ***************\n");
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		message_count = parseInfoLog(infoLog);
		printf("compile with %d warning and %d error\n", message_count[0], error_to_start = message_count[1]);
		delete[] message_count;
		free(infoLog);
	}
	printf("\n");
	return error_to_start;
}



char* loadTextFile(const wchar_t * filename) {
	wifstream wifs;
	wstring txtline;
	char * buffer = 0;
	int c = 0;

	wifs.open(filename);
	if (!wifs.is_open())
	{
		cerr << "Unable to open file" << endl;
		return "";
	}
	string code;
	wifs.imbue(locale(wifs.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	while (getline(wifs, txtline)) {
		for (unsigned i = 0; i<txtline.length(); ++i)
		{
			unsigned int word;
			word = (unsigned int)txtline.at(i);
			if (word < 256)code += (char)word;
			else {
				char * tmp =
					new char[(size_t)(
						sizeof("utfbegin%dutfend") +
						(unsigned int)(2 * log10((double)word)) +
						(unsigned int)1
						)];
				sprintf(tmp, "utfbegin%dutfend", word);
				code += tmp;
			}
		}
		code += '\n';
	}
	size_t size = code.length();
	buffer = new char[size + 2];
	strcpy(buffer, code.c_str());
	wifs.close();
	return buffer;
}

int glPrintError(char *file, int line) {
	int retCode = GL_NO_ERROR;
	GLenum glErr = glGetError();

	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

void print_formatted_words(const char* input, bool lines)
{
	wstring str(L"");
	int iteratorint = 0, iteratorbegin = 0, iteratorend = 0, iteratorgenered = 0;
	char buferint[50], buferbegin[50], buferend[50], bufergenered[50];
	auto checker_begin = "utfbegin", checker_end = "utfend";
	int match_parse = 0, parse_info = 0;

	iteratorint = 0;  /* -- debug -- */  buferint[0] = ' '; buferint[1] = 0;
	iteratorbegin = 0;  /* -- debug -- */  buferbegin[0] = ' '; buferbegin[1] = 0;
	iteratorend = 0;  /* -- debug -- */  buferend[0] = ' '; buferend[1] = 0;
	iteratorgenered = 0;  /* -- debug -- */  bufergenered[0] = ' '; bufergenered[1] = 0;

	for (char *ptr = (char*)((void*)input), now_char = *ptr; *ptr; ++ptr, now_char = *ptr) {

		if (!checker_begin[iteratorbegin]) {//start symbol end
			if (!checker_end[iteratorend]) {//end symbol end
				if (iteratorint) {
					parse_info = sscanf(buferint, "%d", &match_parse);
					if (parse_info > 0) {
						if (match_parse)str += (wchar_t)((unsigned int)match_parse);
						else for (int i = 0; i < iteratorgenered; i++) str += (wchar_t)((unsigned int)bufergenered[i]);
					}
					else {//match error
						for (int i = 0; i < iteratorgenered; i++)
							str += (wchar_t)((unsigned int)bufergenered[i]);
					}
				}
				else for (int i = 0; i < iteratorgenered; i++) str += (wchar_t)((unsigned int)bufergenered[i]);
				iteratorint = 0;  /* -- debug -- */  buferint[0] = ' '; buferint[1] = 0;
				iteratorbegin = 0;  /* -- debug -- */  buferbegin[0] = ' '; buferbegin[1] = 0;
				iteratorend = 0;  /* -- debug -- */  buferend[0] = ' '; buferend[1] = 0;
				iteratorgenered = 0;  /* -- debug -- */  bufergenered[0] = ' '; bufergenered[1] = 0;
				if (ptr - 1 >= input) {
					ptr--;
				}
				continue;
			}
			else if (now_char == checker_end[iteratorend]) {
				buferend[iteratorend++] = now_char; /**/  buferend[iteratorend] = 0;
			}
			else {
				int check_dig = now_char - '0';
				if (check_dig < 0 || check_dig >= 10) {
					for (int i = 0; i < iteratorgenered; i++) str += (wchar_t)((unsigned int)bufergenered[i]);
					iteratorint = 0;  /* -- debug -- */  buferint[0] = ' '; buferint[1] = 0;
					iteratorbegin = 0;  /* -- debug -- */  buferbegin[0] = ' '; buferbegin[1] = 0;
					iteratorend = 0;  /* -- debug -- */  buferend[0] = ' '; buferend[1] = 0;
					iteratorgenered = 0;  /* -- debug -- */  bufergenered[0] = ' '; bufergenered[1] = 0;
					if (ptr - 1 >= input) {
						ptr--;
					}
					continue;
				}
				else buferint[iteratorint++] = now_char; /**/  buferint[iteratorint] = 0;
			}
			bufergenered[iteratorgenered++] = now_char; /**/  bufergenered[iteratorgenered] = 0;
		}
		else if (now_char == checker_begin[iteratorbegin]) {
			buferbegin[iteratorbegin++] = now_char; /**/  buferbegin[iteratorbegin] = 0;
			bufergenered[iteratorgenered++] = now_char; /**/  bufergenered[iteratorgenered] = 0;
			continue;
		}
		else {
			if (iteratorgenered) {
				for (int i = 0; i < iteratorgenered; i++) str += (wchar_t)((unsigned int)bufergenered[i]);
				iteratorint = 0;  /* -- debug -- */  buferint[0] = ' '; buferint[1] = 0;
				iteratorbegin = 0;  /* -- debug -- */  buferbegin[0] = ' '; buferbegin[1] = 0;
				iteratorend = 0;  /* -- debug -- */  buferend[0] = ' '; buferend[1] = 0;
				iteratorgenered = 0;  /* -- debug -- */  bufergenered[0] = ' '; bufergenered[1] = 0;
			}
			str += (wchar_t)((unsigned int)now_char);
		}
	}
	_setmode(_fileno(stdout), _O_U16TEXT);
	if (lines) {
		wistringstream f(str);
		wstring line;
		for (int i = 1; std::getline(f, line); i++) {
			wcout << i << L":\t" << line << endl;
		}
	}
	else {
		wcout << str << endl;
	}
	
	_setmode(_fileno(stdout), _O_TEXT);
}