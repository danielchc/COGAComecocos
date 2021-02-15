/*
	danielchc
	ComeCocos
*/
#ifndef PERSONAXE_H
#define PERSONAXE_H

#include <glut.h>


enum Direccion {
	arriba,
	abaixo,
	esquerda,
	dereita
};
typedef struct {
	float x, y;
	GLuint render;
	GLuint textura;
	int direccion;
	float puntosColision[4][2];
	int podeseMover[4];
} persoaxe;




void comprobarTeletransporte(persoaxe* comecoco, persoaxe fantasmas[3]);
void debuxarFantasmas(persoaxe fantasmas[3]);
void debuxarPacman(persoaxe* comecoco, GLfloat* LuzPos);
void moverPersoaxe(int dir, persoaxe* p);

#endif