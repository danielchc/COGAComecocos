/*
	Daniel Chenel Cea
	ComeCocos
*/
#include <windows.h>
#include <glut.h>
#include <GL/gl.h>	
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "decorador.h"

//Carga unha textura
GLint cargarTextura(char* nomeArquivo, int alpha) {
	GLint textura;
	int width, height, nrChannels;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data = stbi_load(nomeArquivo, &width, &height, &nrChannels, 0);
	if (data) {
		if (alpha)gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		printf("Erro a cargar %s\n", nomeArquivo);
	}
	stbi_image_free(data);
	return textura;
}

//Crea unha lista dun cadrado texturizado
GLint cuadradoTexturizado() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	int index;
	index = glGenLists(1);
	glNewList(index, GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-.5f, .5f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-.5f, -.5f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(.5f, -.5f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(.5f, .5f, 0.0f);
		glEnd();
	glEndList();
	return index;
}



void imprimirTexto(float x, float y, void* font, char* str) {
	char* c;
	glRasterPos2f(x, y);
	for (c = str; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}