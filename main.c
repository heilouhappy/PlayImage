#define _CRT_SECURE_NO_WARNINGS
#include <gl/glut.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define FileName ""

static GLint ImageWidth;//图像宽度

static GLint ImageHeight;//图像高度

static GLubyte* PixelData;//图像数据

static GLint windowWidth = 500;
static GLint windowHeight = 500;
char name[50];//图像名字
int nrChannels;
long hnd; // 查找文件名与正则表达式chRE的匹配第一个文件
int  nRet;
char Path[50];//文件路径
struct _finddata_t data;

void openF();
void display(void);
void upWindow(int w,int h);


void openF() {
		//glutPostRedisplay();
	if (nRet >= 0)
	{
		if (data.attrib == _A_SUBDIR)  // 如果是目录
			printf("   [%s]*\n", data.name);
		else {
			strcpy(name, Path);
			strcat(name, data.name);
		}
		nRet = _findnext(hnd, &data);
	}
	PixelData = stbi_load(name, &ImageWidth, &ImageHeight, &nrChannels, 0);
	display();
	free(PixelData);
	glutTimerFunc(30, &openF, 1);
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos3f(-1.0f, -1.0f, 0);
	glPixelZoom((float)windowWidth / (float)ImageWidth, (float)windowHeight / (float)ImageHeight);
	glDrawPixels(ImageWidth, ImageHeight, GL_RGB, GL_UNSIGNED_BYTE, PixelData);
	glFlush();
	glutSwapBuffers();
}

void upWindow(int w,int h) {
	if (w<=0||h<=0) {
		w = 500;
		h = 500;
	}
	else
	{
		windowWidth = w;
		windowHeight = h;
	}
	printf("%d,%d\n", w, h);
}

void main(int argc, char* argv[]) {
	
	if (argc > 1) {
		char *path = (char *)malloc(sizeof(char) * 50);
		scanf("%s", path);
		strcpy(Path, path);
		strcat(path, "*.*");
		hnd = _findfirst(path, &data);
		nRet = (hnd < 0) ? -1 : 1;
	}
	else
	{
		printf("Input file Path:\n");
		char *path = (char *)malloc(sizeof(char) * 50);
		scanf("%s", path);
		strcpy(Path, path);
		strcat(path, "*.*");
		hnd = _findfirst(path, &data);
		nRet = (hnd < 0) ? -1 : 1;
	}
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);

	stbi_set_flip_vertically_on_load(TRUE);
	glutCreateWindow(FileName);
	glutDisplayFunc(&display);
	glutMouseFunc(&openF);
	glutReshapeFunc(&upWindow);

	glutMainLoop();
	_findclose(hnd);
}