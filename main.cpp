#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>

using namespace std;

// Variáveis globais
unsigned int object;
vector<vector<float>> vertices;
vector<vector<int>> faces;
vector<vector<float>> normals;
vector<vector<float>> textureBounds;
float scale;
float translateX, translateY, translateZ;
float rotationX, rotationY, rotationZ;
float rotVel = 3.5;
bool light1, light2, light3;

// Métodos
void lights();
void loadObj(string);
void keyboard(unsigned char, int, int);
void keyboard_special(int, int, int);
void reshape(int, int);
void drawObject();
void display();
void timer(int);
void mouse(int, int, int, int);
void lights();
void initialize(int, char**);


int main(int argc, char** argv) {
	// Inicializando glut, janela e propriedades
	initialize(argc, argv);

	// Instanciando funções que rodam no motor
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutTimerFunc(10, timer, 0);
	glutMouseFunc(mouse);

	// Carregando .obj
	loadObj("objs/elepham.obj");

	// Inicializando motor principal
	glutMainLoop();
	return 0;
}


// Carregando arquivo OBJ
void loadObj(string fname) {
	int read;
	float x, y, z;
	ifstream arquivo(fname);
	if (!arquivo.is_open()) {
		cout << "arquivo nao encontrado";
		exit(1);
	} else {
		string tipo;
		while (arquivo >> tipo) {

			if (tipo == "v") {
				vector<float> vertice;
				float x, y, z;
				arquivo >> x >> y >> z;
				vertice.push_back(x);
				vertice.push_back(y);
				vertice.push_back(z);
				vertices.push_back(vertice);
			}

			if (tipo == "f") {
				vector<int> face;
				string x, y, z;
				arquivo >> x >> y >> z;
				int fp = stoi(x.substr(0, x.find("/"))) - 1;
				int fs = stoi(y.substr(0, y.find("/"))) - 1;
				int ft = stoi(z.substr(0, z.find("/"))) - 1;
				face.push_back(fp);
				face.push_back(fs);
				face.push_back(ft);
				faces.push_back(face);
			}

			if (tipo == "vt") {
				vector<float> textura;
				float x, y;
				arquivo >> x >> y;
				textura.push_back(x);
				textura.push_back(y);
				textureBounds.push_back(textura);
			}

			if (tipo == "vn") {
				vector<float> normal;
				float x, y, z;
				arquivo >> x >> y >> z;
				normal.push_back(x);
				normal.push_back(y);
				normal.push_back(z);
				normals.push_back(normal);
			}
		}
	}

	object = glGenLists(1);
	glPointSize(2.0);
	glNewList(object, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_TRIANGLES);
		//glBegin(GL_LINES);

		for (int i = 0; i < faces.size(); i++) {
			vector<int> face = faces[i];

			glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);
			glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);

			//glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);
			glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);

			//glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);
			//glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);

		}
		glEnd();

	}
	glPopMatrix();
	glEndList();
	arquivo.close();
}

// Função de teclas comuns do keyboard
void keyboard(unsigned char key, int x, int y) {
	std::cout << "Key:" << key << std::endl;
	switch (key) {
	case 27:
		exit(0);
		break;
	case 55:
		// Rotacionar para baixo
		rotationX += rotVel;
		break;
	case 57:
		// Rotacionar para cima
		rotationX -= rotVel;
		break;
	case 52:
		// Rotacionar para esquerda
		rotationY += rotVel;
		break;
	case 54:
		// Rotacionar para direita
		rotationY -= rotVel;
		break;
	case 49:
		// Rotacionar eixo z
		rotationZ += rotVel;
		break;
	case 51:
		// Rotacionar eixo z
		rotationZ -= rotVel;
		break;
	case 53:
		rotationX += rand() % 30 - 15;
		rotationY += rand() % 30 - 15;
		rotationZ += rand() % 20 - 10;
		break;
	case 'q':
		// Ambiente
		light1 = !light1;
		break;
	case 'w':
		// Difusa
		light2 = !light2;
		break;
	case 'e':
		// Especular
		light3 = !light3;
		break;
	}
}

// Função de teclas especiais do keyboard
void keyboard_special(int key, int x, int y) {
	std::cout << "Special Key:" << key << std::endl;
	switch (key) {
	case GLUT_KEY_DOWN:
		translateY -= 1;
		break;

	case GLUT_KEY_UP:
		translateY += 1;
		break;

	case GLUT_KEY_RIGHT:
		translateX += 1;
		break;

	case GLUT_KEY_LEFT:
		translateX -= 1;
		break;

	case GLUT_KEY_PAGE_UP:
		scale += 0.1;
		break;

	case GLUT_KEY_PAGE_DOWN:
		if(scale > 0.2)
			scale -= 0.1;
		break;
	}
}

// Função Global de Reshape
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
}

// Função global de desenho
void drawObject() {
	glPushMatrix();
	glColor3f(1.0, 0.23, 0.27);

	// Movimentar
	glTranslatef(translateX, translateY, translateZ);

	// Escalar
	glScalef(scale, scale, scale);

	// Rotacionar
	glRotatef(rotationX, 1, 0, 0);
	glRotatef(rotationY, 0, 1, 0);
	glRotatef(rotationZ, 0, 0, 1);

	// Luzes
	lights();

	// Determina as propriedades do material
	GLfloat cor_verde[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat cor_branco[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat cor_azul[] = { 0.0, 0.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, cor_verde);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor_azul);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cor_branco);
	glMaterialf(GL_FRONT, GL_SHININESS, 100);

	glCallList(object);

	glPopMatrix();
}

// Função de display screen
void display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	drawObject();
	glutSwapBuffers();
}

// Timer de display
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

// Função para dar print nas posições do mouse por click
void mouse(int button, int state, int x, int y) {
	cout << "X: " << x << endl;
	cout << "Y: " << y << endl;
}

// Função para criação da luz
void lights() {
	//Ativa o uso de luz
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);

	double angulo_luz = 180.0;
	double tipo_luz = 0.0; // 0.0 = Direcional / 1.0 = Spotlight

	// Determina a intensidade e cor da luz
	GLfloat luz_ambiente[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat luz_difusa[] = { 0.0, 0., 0., 0.0 };
	GLfloat luz_especular[] = { 0.0, 0., 0.0, 0.0 };

	// Controle luz 1 (ambiente)
	if (light1) {
		glEnable(GL_LIGHT1);
		GLfloat posicao_luz1[] = { -400, -225.0, 1.0, tipo_luz }; // x, y, z, w
		GLfloat direcao_spotlight1[] = { 0.0, 0.0, -1.0 }; // x, y, z

		double luz_ambiente_valor = 1.0;
		luz_ambiente[0] = luz_ambiente_valor;
		luz_ambiente[1] = luz_ambiente_valor;
		luz_ambiente[2] = luz_ambiente_valor;
		luz_ambiente[3] = luz_ambiente_valor;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa);
		glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular);
		glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente);
		glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);

		// Definindo atuação
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.5);

		// Definindo spotlight
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angulo_luz);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao_spotlight1);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0);
	} else {
		double luz_ambiente_valor = 0;
		luz_ambiente[0] = luz_ambiente_valor;
		luz_ambiente[1] = luz_ambiente_valor;
		luz_ambiente[2] = luz_ambiente_valor;
		luz_ambiente[3] = luz_ambiente_valor;
		glDisable(GL_LIGHT1);
	}

	// Controle luz 2 (difusa)
	if (light2) {
		glEnable(GL_LIGHT2);
		GLfloat posicao_luz2[] = { 450, 0.0, 1.0, tipo_luz }; // x, y, z, w
		GLfloat direcao_spotlight2[] = { 0.0, -1.0, -1.0 }; // x, y, z

		double luz_difusa_valor = 100.0;
		luz_difusa[0] = luz_difusa_valor;
		luz_difusa[1] = luz_difusa_valor;
		luz_difusa[2] = luz_difusa_valor;
		luz_difusa[3] = luz_difusa_valor;

		glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa);
		glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular);
		glLightfv(GL_LIGHT2, GL_AMBIENT, luz_ambiente);
		glLightfv(GL_LIGHT2, GL_POSITION, posicao_luz2);

		// Definindo atuação
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.5);

		// Definindo spotlight
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angulo_luz);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direcao_spotlight2);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.0);
	} else {
		double luz_difusa_valor = 0.0;
		luz_difusa[0] = luz_difusa_valor;
		luz_difusa[1] = luz_difusa_valor;
		luz_difusa[2] = luz_difusa_valor;
		luz_difusa[3] = luz_difusa_valor;
		glDisable(GL_LIGHT2);
	}

	// Controle luz 3 (especular)
	if (light3) {
		glEnable(GL_LIGHT3);
		GLfloat posicao_luz3[] = { 400.0, 0.0, 1.0, tipo_luz }; // x, y, z, w
		GLfloat direcao_spotlight3[] = { 0.0, 0.0, -1.0 }; // x, y, z

		double luz_especular_valor = 1.0;
		luz_especular[0] = luz_especular_valor;
		luz_especular[1] = luz_especular_valor;
		luz_especular[2] = luz_especular_valor;
		luz_especular[3] = luz_especular_valor;

		glLightfv(GL_LIGHT3, GL_DIFFUSE, luz_difusa);
		glLightfv(GL_LIGHT3, GL_SPECULAR, luz_especular);
		glLightfv(GL_LIGHT3, GL_AMBIENT, luz_ambiente);
		glLightfv(GL_LIGHT3, GL_POSITION, posicao_luz3);

		// Definindo atuação
		glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5);
		glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.5);

		// Definindo spotlight
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, angulo_luz);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direcao_spotlight3);
		glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.0);
	} else {
		double luz_especular_valor = 0.0;
		luz_especular[0] = luz_especular_valor;
		luz_especular[1] = luz_especular_valor;
		luz_especular[2] = luz_especular_valor;
		luz_especular[3] = luz_especular_valor;
		glDisable(GL_LIGHT3);
	}
}

// Inicialização do glut e variaveis
void initialize(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 450);
	glOrtho(0.0f, 800, 450, 0.0f, 0.0f, 1.0f);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Trabalho M1 - 2");

	// Inicializando variaveis globais
	scale = 0.4;
	translateX = 0;
	translateY = 0;
	translateZ = -150;
	light1 = light2 = light3 = true;

	// Corrigindo buffer de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

// Escreve na tela (reservado para futuros projetos)
void RenderString(float x, float y, void* font, const unsigned char* string)
{
	char* c;

	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}
