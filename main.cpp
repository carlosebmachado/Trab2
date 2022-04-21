#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>

using namespace std;

// globals

unsigned int object;
vector<vector<float>> vertices;
vector<vector<int>> faces;
vector<vector<float>> normais;
vector<vector<float>> texturas;
float scaleX, scaleY, scaleZ;
float translateX, translateY, translateZ;
float rotX, rotY, rotZ;


void loadObj(string fname)
{
	int read;
	float x, y, z;
	ifstream arquivo(fname);
	if (!arquivo.is_open()) {
		cout << "arquivo nao encontrado";
		exit(1);
	}
	else {
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
				texturas.push_back(textura);
			}

			if (tipo == "vn") {
				vector<float> normal;
				float x, y, z;
				arquivo >> x >> y >> z;
				normal.push_back(x);
				normal.push_back(y);
				normal.push_back(z);
				normais.push_back(normal);
			}
		}
	}

	object = glGenLists(1);
	glPointSize(2.0);
	glNewList(object, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_QUADS);
		//glBegin(GL_LINES);

		for (int i = 0; i < faces.size(); i++) {
			vector<int> face = faces[i];

			glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);
			glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);

			glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);
			glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);

			glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);
			glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);

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
		++rotX;
		break;
	case 57:
		// Rotacionar para cima
		--rotX;
		break;
	case 52:
		// Rotacionar para esquerda
		++rotY;
		break;
	case 54:
		// Rotacionar para direita
		--rotY;
		break;
	case 49:
		// Rotacionar eixo z
		++rotZ;
		break;
	case 51:
		// Rotacionar eixo z
		--rotZ;
		break;
	case 53:
		rotX += rand() % 30 - 15;
		rotY += rand() % 30 - 15;
		rotZ += rand() % 20 - 10;
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
		scaleX += 0.1;
		scaleY += 0.1;
		scaleZ += 0.1;
		break;

	case GLUT_KEY_PAGE_DOWN:
		scaleX -= 0.1;
		scaleY -= 0.1;
		scaleZ -= 0.1;
		break;
	}
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
}

void drawObject() {

	glPushMatrix();
	glColor3f(1.0, 0.23, 0.27);

	// movimentar
	glTranslatef(translateX, translateY, translateZ);

	// escalar
	glScalef(scaleX, scaleY, scaleZ);

	// rotacionar
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	glRotatef(rotZ, 0, 0, 1);

	// determina as propriedades do material
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

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	drawObject();
	glutSwapBuffers();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void initialize() {
	scaleX = 0.2;
	scaleY = 0.2;
	scaleZ = 0.2;
	translateX = 0;
	translateY = -40;
	translateZ = -150;


	//Ativa o uso de luz
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	// determina a intensidade e cor da luz
	GLfloat luz_ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat luz_difusa[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat luz_especular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);

	//determina a posição da luz
	GLfloat posicao_luz[] = { 400.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 450);
	glOrtho(0, 800, 450, 0, -1.0, 1.0);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Trabson 2");
	initialize();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutTimerFunc(10, timer, 0);
	//loadObj("objs/mba1.obj");
	loadObj("objs/square2.obj");
	glutMainLoop();
	return 0;
}