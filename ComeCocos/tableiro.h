/*
	danielchc
	ComeCocos
*/
#ifndef PAREDES_H
#define PAREDES_H
#include "persoaxe.h"
#define NCOCOS 81

typedef struct {
	float x, y;
}esquina;

typedef struct {
	float x1, y1, x2, y2;
	float size;
	esquina esquinas[4];
} parede;

typedef struct {
	float x, y;
	int comido;
	int especial;
}coco;

coco cocos[NCOCOS];
void generarLimitesParedes();
void inicializarParedes();
void comprobarColisionsParedes(persoaxe* pe);
void inicializarCocos(GLuint cText, GLuint cTextEspecial, GLuint render);

void reiniciarCocos();
void pintarCocos();
void pintarParedes();
#endif