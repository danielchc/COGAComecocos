/*
	danielchc
	ComeCocos
*/
#include <windows.h>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>
#include "persoaxe.h"

//Calcula as limites da persoaxe
void calcularLimitesPersoaxe(persoaxe* p) {
	p->puntosColision[0][0] = p->x + 1;
	p->puntosColision[0][1] = p->y;
	p->puntosColision[1][0] = p->x - 1;
	p->puntosColision[1][1] = p->y;
	p->puntosColision[2][0] = p->x;
	p->puntosColision[2][1] = p->y + 1;
	p->puntosColision[3][0] = p->x;
	p->puntosColision[3][1] = p->y - 1;
}

//Debuxa o ComeCocos
void debuxarPacman(persoaxe *comecoco,GLfloat * LuzPos) {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, comecoco->textura);
		glTranslated(comecoco->x, comecoco->y, 0.0f);
		if (comecoco->direccion == esquerda)glRotated(180, 0, 0, 1);
		else if (comecoco->direccion == arriba)glRotated(90, 0, 0, 1);
		else if (comecoco->direccion == abaixo)glRotated(-90, 0, 0, 1);
		glLightfv(GL_LIGHT0, GL_POSITION, LuzPos);
		glCallList(comecoco->render);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	calcularLimitesPersoaxe(comecoco);
}

//Debuxa os Fantasmas 
void debuxarFantasmas(persoaxe fantasmas[3]) {
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glColor3f(1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, fantasmas[i].textura);
			glTranslated(fantasmas[i].x, fantasmas[i].y, 0.0f);
			if (fantasmas[i].direccion == esquerda)glScalef(-1.0f, 1.0f, 0.0f);
			glCallList(fantasmas[i].render);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		calcularLimitesPersoaxe(&fantasmas[i]);
	}
}

//Comproboba se saeu da pantalla
void comprobarTeletransporte(persoaxe *comecoco, persoaxe fantasmas[3]) {
	if (comecoco->x < -10 && comecoco->y == 0)comecoco->x = 10;
	if (comecoco->x > 10 && comecoco->y == 0)comecoco->x = -10;
	for (int i = 0; i < 3; i++) {
		if (fantasmas[i].x < -10 && fantasmas[i].y == 0)fantasmas[i].x = 10;
		if (fantasmas[i].x > 10 && fantasmas[i].y == 0)fantasmas[i].x = -10;
	}
}

//Move unha persoaxe nunha direccion
void moverPersoaxe(int dir, persoaxe* p) {
	if (!p->podeseMover[dir])return;
	p->direccion = dir;
	switch (dir) {
	case arriba:
		p->y += 2;
		break;
	case esquerda:
		p->x -= 2;
		break;
	case abaixo:
		p->y -= 2;
		break;
	case dereita:
		p->x += 2;
		break;
	}
}