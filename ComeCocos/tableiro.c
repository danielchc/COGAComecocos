/*
	danielchc
	ComeCocos
*/
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#include "tableiro.h"


parede paredes[] = {
	{-9.0,9.0,9.0,9.0,0.5},
	{-9.0,-9.0,9.0,-9.0,0.5},
	//Lateral esquerdo
	{-9.0,-1.0,-9.0,-9.25,0.5},
	{-9.0,9.25,-9.0,1.0,0.5},
	//Lateral dereito
	{9.0,-1.0,9.0,-9.25,0.5},
	{9.0,9.25,9.0,1.0,0.5},
	{-9.25,-1.0,-7.0,-1.0,0.5},
	{-9.25,1.0,-7.0,1.0,0.5},

	{7.0,-1.0,9.25,-1.0,0.5},
	{7.0,1.0,9.25,1.0,0.5},
	//Arriba 1
	{-7.0,-7.0,-1.0,-7.0,0.5},
	{1.0,-7.0,7.0,-7.0,0.5},
	//Abajo 1
	{-7.0,7.0,-1.0,7.0,0.5},
	{1.0,7.0,7.0,7.0,0.5},
	//Arriba 2
	{-7.0,5.0,-3.0,5.0,0.5},
	{-1.0,5.0,1.0,5.0,0.5},
	{3.0,5.0,7.0,5.0,0.5},
	//Abaixo 2
	{-7.0,-5.0,-3.0,-5.0,0.5},
	{-1.0,-5.0,1.0,-5.0,0.5},
	{3.0,-5.0,7.0,-5.0,0.5},
	//Arriba 2
	{-7.0,3.0,-3.0,3.0,0.5},
	{3.0,3.0,7.0,3.0,0.5},
	{1.0,5.25,1.0,3.0,0.5},
	//Abaixo 2
	{-7.0,-3.0,-3.0,-3.0,0.5},
	{3.0,-3.0,7.0,-3.0,0.5},
	{-1.0,-3.0,-1.0,-5.25,0.5},
	//Central
	{-3.0,-1.0,3.0,-1.0,0.5},
	{-3.0,1.0,-1.0,1.0,0.5},
	{1.0,1.0,3.0,1.0,0.5},
	//Vertical Central
	{-3.0,1.25,-3.0,-1.25,0.5},
	{3.0,1.25,3.0,-1.25,0.5},
};


coco cocos[NCOCOS];

GLuint cocoTexura, cerezaTexura, cerezaTexura;
GLuint cocoRender, paredeRender;

//Crea a lista das paredes
void inicializarParedes() {
	paredeRender = glGenLists(1);
	glNewList(paredeRender, GL_COMPILE);
	for (int i = 0; i < sizeof(paredes) / sizeof(parede); i++) {
		parede p = paredes[i];
		glPushMatrix();
			glColor3f(0.0f, 0.0, 1.0f);
			glBegin(GL_QUADS);
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(p.esquinas[0].x, p.esquinas[0].y, 0);

				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(p.esquinas[1].x, p.esquinas[1].y, 0);

				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(p.esquinas[2].x, p.esquinas[2].y, 0);

				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(p.esquinas[3].x, p.esquinas[3].y,0.0f);
			glEnd();
		glPopMatrix();
	}
	glEndList();
	
}

//Xera os limites das paredes
void generarLimitesParedes() {
	for (int i = 0; i < sizeof(paredes) / sizeof(parede); i++) {
		parede p = paredes[i];
		float lw = p.size / 2;
		if (p.y1 > p.y2) {
			p.esquinas[0].x = p.x1 - lw;
			p.esquinas[0].y = p.y1;
			p.esquinas[1].x = p.x2 - lw;
			p.esquinas[1].y = p.y2;
			p.esquinas[2].x = p.x2 + lw;
			p.esquinas[2].y = p.y2;
			p.esquinas[3].x = p.x1 + lw;
			p.esquinas[3].y = p.y1;
		}
		else {
			p.esquinas[0].x = p.x1;
			p.esquinas[0].y = p.y1 - lw;
			p.esquinas[1].x = p.x2;
			p.esquinas[1].y = p.y2 - lw;
			p.esquinas[2].x = p.x2;
			p.esquinas[2].y = p.y2 + lw;
			p.esquinas[3].x = p.x1;
			p.esquinas[3].y = p.y1 + lw;
		}
		paredes[i] = p;
	}
}

//Pinta as paredes
void pintarParedes() {
	glCallList(paredeRender);
}

//Comproba as colisións dunha persoaxe cas paredes
void comprobarColisionsParedes(persoaxe* pe) {
	parede p;
	for (int i = 0; i < 4; i++) pe->podeseMover[i] = 1;
	for (int i = 0; i < sizeof(paredes) / sizeof(parede); i++) {
		p = paredes[i];
		if (
			(
				pe->puntosColision[2][0] >= p.esquinas[0].x &&
				pe->puntosColision[2][0] <= p.esquinas[1].x &&
				pe->puntosColision[2][1] >= p.esquinas[1].y &&
				pe->puntosColision[2][1] <= p.esquinas[2].y
				)
			||
			(
				pe->puntosColision[2][0] >= p.esquinas[0].x &&
				pe->puntosColision[2][0] <= p.esquinas[2].x &&
				pe->puntosColision[2][1] >= p.esquinas[1].y &&
				pe->puntosColision[2][1] <= p.esquinas[3].y
				) ||
				(pe->x <= -9) || (pe->x >= 9)

			) pe->podeseMover[arriba] = 0;

		if ((
			pe->puntosColision[3][0] >= p.esquinas[0].x &&
			pe->puntosColision[3][0] <= p.esquinas[1].x &&
			pe->puntosColision[3][1] >= p.esquinas[1].y &&
			pe->puntosColision[3][1] <= p.esquinas[2].y
			) ||
			(
				pe->puntosColision[3][0] >= p.esquinas[0].x &&
				pe->puntosColision[3][0] <= p.esquinas[2].x &&
				pe->puntosColision[3][1] >= p.esquinas[1].y &&
				pe->puntosColision[3][1] <= p.esquinas[3].y
				) ||
				(pe->x <= -9) || (pe->x >= 9)
			)
			pe->podeseMover[abaixo] = 0;


		if (
			pe->puntosColision[1][0] >= p.esquinas[0].x &&
			pe->puntosColision[1][0] <= p.esquinas[2].x &&
			pe->puntosColision[1][1] >= p.esquinas[1].y &&
			pe->puntosColision[1][1] <= p.esquinas[3].y
			)pe->podeseMover[esquerda] = 0;
		if (
			pe->puntosColision[0][0] >= p.esquinas[0].x &&
			pe->puntosColision[0][0] <= p.esquinas[2].x &&
			pe->puntosColision[0][1] >= p.esquinas[1].y &&
			pe->puntosColision[0][1] <= p.esquinas[3].y
			)pe->podeseMover[dereita] = 0;
	}
}


//Incializa a posicións dos cocos
void inicializarCocos(GLuint cText, GLuint cTextEspecial,GLuint render) {
	cocoTexura = cText;
	cerezaTexura = cTextEspecial;
	cocoRender = render;
	int r = 0, c = 0;
	for (int i = -8; i <= 8; i += 2) {
		for (int j = -8; j <= 8; j += 2, c++) {
			cocos[c].x = i;
			cocos[c].y = j;
		}
	}
	reiniciarCocos();
}

//Pinta os cocos e cereixas
void pintarCocos() {
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < NCOCOS; i++) {
		if (cocos[i].comido)continue;
		glPushMatrix();
		if(!cocos[i].especial)glBindTexture(GL_TEXTURE_2D, cocoTexura);
		else glBindTexture(GL_TEXTURE_2D, cerezaTexura);
		glColor3f(1.0, 1.0, 1.0);
		glTranslated(cocos[i].x, cocos[i].y, 0);
		glCallList(cocoRender);
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
}

void reiniciarCocos() {
	int r;
	for (int i = 0; i < NCOCOS; i++) {
		cocos[i].comido = 0;
		cocos[i].especial = 0;
	}
	for (int i = 0; i < 5; i++) {
		r = rand() % NCOCOS;
		cocos[r].especial = 1;
	}
}