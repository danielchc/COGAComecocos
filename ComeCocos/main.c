/*
	danielchc
	ComeCocos
*/

#include <windows.h>
#include <glut.h>
#include <GL/gl.h> 	
#include <GL/glu.h>	
#include <stdio.h>

#include "persoaxe.h"
#include "tableiro.h"
#include "decorador.h"
#define NVIDAS 2

persoaxe comecoco = { 0,0,0 };
persoaxe fantasmas[3];
GLuint pantallaRender;
GLuint texturasPantalla[6];
int vidas = NVIDAS;
int modoSinLuz = 0;

enum EstadoXogo {
	Inicio,
	Iniciando,
	Xogando,
	GameOver,
	Ganou
};

GLfloat Ambient_1[4] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat Diffuse_1[4] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat Specular_1[4] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat SpotDir_1[3] = { 0.0f,0.0f,-1.0f };
GLfloat LuzPos_1[4] = { 0.0f,0.0f,1.0,1.0f };

int cocosComidos;
int puntos = 0;
int estado = Inicio;
int width = 800;
int height = 800;
int tempo=0;
char cadea[40];

char cadeaReinicio[40] = "PULSA ESPACIO PARA EMPEZAR";

//Desactiva o modo sen luz
void modoVida() {
	modoSinLuz = 0;
}

//Temporizador do texto
void temporizador() {
	tempo++;
	if (estado == Xogando) glutTimerFunc(1000, temporizador, NULL);
}

//Comproba as colisións con un fantasma
void comprobarColisionFantasma() {
	for (int i = 0; i < 3; i++) {
		if (comecoco.x == fantasmas[i].x && comecoco.y == fantasmas[i].y) {
			vidas--;
			if (vidas == 0) {
				modoSinLuz = 0;
				estado = GameOver;
				PlaySound(NULL, NULL, NULL);
				PlaySound(TEXT("morto.wav"), NULL, SND_ASYNC | SND_FILENAME);
			}else {
				comecoco.x = 0;
				comecoco.y = 0;
				modoSinLuz = 1;
				PlaySound(TEXT("intermision.wav"), NULL, SND_ASYNC | SND_FILENAME);
				glutTimerFunc(5000, modoVida, NULL);
			}
			
		}
	}
}

//Comproba as colisións con un coco ou cereixa
void comprobarColisionCocos() {
	if (estado != Xogando)return;
	for (int i = 0; i < sizeof(cocos) / sizeof(coco); i++) {
		if (comecoco.x == cocos[i].x && comecoco.y == cocos[i].y && (!cocos[i].comido)) {
			cocos[i].comido = 1;
			cocosComidos++;
			puntos += (cocos[i].especial) ? 5 : 1;
			if(modoSinLuz==0)PlaySound((cocos[i].especial)? TEXT("fruta.wav") :TEXT("coco.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
	}
	if (cocosComidos == sizeof(cocos) / sizeof(coco))estado = Ganou;
}
//Fantasmas Intelixencia artificial
void fantasmaAI(int fantasma) {
	if (estado != Xogando)return;
	float probSen = rand() % 100;
	int dir = fantasmas[fantasma].direccion;
	comprobarColisionsParedes(&fantasmas[fantasma]);
	if (fantasmas[fantasma].podeseMover[fantasmas[fantasma].direccion] != 1 || probSen>=70) {
		fantasmas[fantasma].direccion = rand() % 4;
		for (; fantasmas[fantasma].podeseMover[fantasmas[fantasma].direccion] == 0; fantasmas[fantasma].direccion--) {
			fantasmas[fantasma].direccion %= 4;
		}
	}

	moverPersoaxe(fantasmas[fantasma].direccion, &fantasmas[fantasma]);
	if (estado == Xogando)glutTimerFunc(400 + (fantasma * 200), fantasmaAI, fantasma);
}

//Pantalla do comecocos
void comeCocos() {
	glEnable(GL_LIGHT0);
	comprobarTeletransporte(&comecoco, fantasmas);
	pintarCocos();
	debuxarFantasmas(fantasmas);
	debuxarPacman(&comecoco, LuzPos_1);

	if (estado == Xogando) {
		comprobarColisionsParedes(&comecoco);
		comprobarColisionFantasma();
		comprobarColisionCocos(&estado);
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(1.5f, 9.5f, 0.0f);
		sprintf_s(cadea, 40, "VIDAS: %d PUNTOS %d TEMPO %02d:%02d", vidas, puntos, (tempo >= 60) ? (int)(tempo / 60.0f) : 0, tempo % 60);
		imprimirTexto(0, 0, GLUT_BITMAP_HELVETICA_18, cadea);
	glPopMatrix();
	if(modoSinLuz)glEnable(GL_LIGHTING);
}

//Cadrado onde se mostra as texturas de información
void pantalla(int cod) {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, texturasPantalla[cod]);
		glScaled(6.5f, 3.5f, 0.0f);
		glCallList(pantallaRender);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


//Inicio retrasado
void inicioRetrasado() {
	estado = Xogando;
	for (int i = 0; i < 3; i++) { 
		fantasmas[i].direccion = esquerda;
		glutTimerFunc(400 + (i * 200), fantasmaAI, i);
	}
	glutTimerFunc(1000, temporizador, NULL);
}

void iniciarXogo() {
	reiniciarCocos();
	vidas = NVIDAS;
	tempo = 0;
	cocosComidos = 0;
	puntos = 0;
	modoSinLuz = 0;
	fantasmas[0].x = -8;
	fantasmas[1].x = 0;
	fantasmas[2].x = 8;
	fantasmas[0].y = -2;
	fantasmas[1].y = 8;
	fantasmas[2].y = -2;
	comecoco.x=0;
	comecoco.y=-8;
	estado = Iniciando;
	PlaySound(TEXT("intro.wav"), NULL, SND_ASYNC | SND_FILENAME);
	glutTimerFunc(4500, inicioRetrasado, NULL);
}

void display (void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	pintarParedes();

	switch (estado) {
		case Inicio:
			pantalla(Inicio);
			break;
		case Iniciando:
		case Xogando:
			comeCocos();
			break;
		case Ganou:
		case GameOver:
			comeCocos();
			glPushMatrix();
				glColor3f(1.0, 1.0, 1.0);
				glTranslatef(-(glutBitmapLength(GLUT_BITMAP_HELVETICA_18, cadeaReinicio) / 80), -9.5f, 0.0f);
				imprimirTexto(0, 0, GLUT_BITMAP_HELVETICA_18, cadeaReinicio);
			glPopMatrix();
			pantalla(estado);
			break;
	}
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}	
 
//Accións das teclas
void teclas(unsigned char tras, int x, int y){
	switch (tolower(tras)) {
		case 'w':
			if (estado == Xogando)moverPersoaxe(arriba, &comecoco);
			break;
		case 's':
			if (estado == Xogando)moverPersoaxe(abaixo, &comecoco);
			break;
		case 'a':
			if (estado == Xogando)moverPersoaxe(esquerda, &comecoco);
			break;
		case 'd':
			if(estado==Xogando)moverPersoaxe(dereita, &comecoco);
			break;
		case ' ':
			if (estado != Xogando && estado!=Iniciando)iniciarXogo();
			break;		
		default:
			break;
	}
	glutPostRedisplay();
}

//Función que adapta o xogo o viewport o tamaño da ventá
void cambiarVentana(int w, int h) {
	glViewport((w/2)-(h/2), 0, h, h);
}


int main(int argc, char **argv) {
	srand(time(NULL));
	glutInit(&argc, argv);
	
	glutInitWindowPosition(0,0);  
	glutInitWindowSize (width, height);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE |GLUT_DEPTH); //Activar 
	glutCreateWindow("Pac-Man");
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	

	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient_1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse_1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular_1);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, SpotDir_1);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 70);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10.0, 10.0f, -10.0, 10.0f);
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

	printf("Pac-Man\nProxecto COGA\ndanielchc\nControles:\n\tW->Arriba\n\tS->Abaixo\n\tA->Esquerda\n\tD->Dereita\n\t<ESPACIO> Empezar\n");

	fantasmas[0].render = 
	fantasmas[1].render = 
	fantasmas[2].render =
	comecoco.render = 
	pantallaRender = 
	cuadradoTexturizado();

	comecoco.textura = cargarTextura("comecocos.png", 1);
	fantasmas[0].textura = cargarTextura("fantasma.png", 1);
	fantasmas[1].textura = cargarTextura("fantasma3.png", 1);
	fantasmas[2].textura = cargarTextura("fantasma2.png", 1);
	texturasPantalla[Inicio] = cargarTextura("inicio.png",1);
	texturasPantalla[GameOver] = cargarTextura("gameover.png",1);
	texturasPantalla[Ganou] = cargarTextura("ganou.png",1);

	generarLimitesParedes();
	inicializarParedes();
	inicializarCocos(cargarTextura("coco.png", 1), cargarTextura("cereza.png", 1), comecoco.render);

	glutKeyboardFunc(teclas);
	glutDisplayFunc(display);
	glutReshapeFunc(cambiarVentana);
	
	glutMainLoop();
	return 0;
}