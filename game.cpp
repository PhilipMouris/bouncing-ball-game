#include <glut.h>
#include <math.h>
#define PI 3.14159265

int xRotation = 0;
int yRotation = 0;
int xRotationArrow = 0;
int yRotationArrow = 0;
int xRotationSlow = 0;
int yRotationSlow = 0;
int releaseFlag = 0;
float ballTranslationZ = 280.0;
float ballTranslationX = 0.0;
float ballTranslationY = -6.5;
float zReference = 280.0;
float xReference = 0.0;
float yReference = -6.5;
int speed = 1;

void drawVerticalWall(int direction) {
	int color = 1;
	float nearEdge = 195.0;
	for (int i = 0; i < 7; i++) {
		float bottomEdge = -30.0;
		for (int j = 0; j < 7; j++) {
			switch (color) {
			case 1: glColor3f(1.0, 0.0, 0.0); break;
			case 2: glColor3f(0.0, 1.0, 0.0); break;
			case 3: glColor3f(0.0, 0.0, 1.0); break;
			case 4: glColor3f(1.0, 0.0, 1.0); break;
			}
			glPushMatrix();
			glTranslated(direction * 25.0, bottomEdge, nearEdge);
			glScaled(0.1, 10.0, 40.0);
			glutSolidCube(1);
			glPopMatrix();
			bottomEdge += 10;
			if (color == 4)
				color = 1;
			else
				color += 1;
		}
		nearEdge -= 40;
	}
}

void drawHorizontalWall(int direction) {
	int color = 1;
	float nearEdge = 195.0;
	for (int i = 0; i < 7; i++) {
		float leftEdge = -20.0;
		for (int j = 0; j < 5; j++) {
			switch (color) {
			case 1: glColor3f(1.0, 0.0, 0.0); break;
			case 2: glColor3f(0.0, 1.0, 0.0); break;
			case 3: glColor3f(0.0, 0.0, 1.0); break;
			case 4: glColor3f(1.0, 0.0, 1.0); break;
			}
			glPushMatrix();
			glTranslated(leftEdge, direction * 35.0, nearEdge);
			glScaled(10, 0.1, 40.0);
			glutSolidCube(1);
			glPopMatrix();
			leftEdge += 10;
			if (color == 4)
				color = 1;
			else
				color += 1;
		}
		nearEdge -= 40;
	}
}

void drawEndWall() {
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslated(0.0, 0.0, -65.0);
	glScaled(50.0, 70.0, 0.1);
	glutSolidCube(1);
	glPopMatrix();
}

void drawSphere() {
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(ballTranslationX, ballTranslationY, ballTranslationZ);
	glScaled(0.8, 1.0, 1.0);
	glutSolidSphere(1.5, 100, 100);
	glPopMatrix();
}

void drawArrow() {
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(0.0, -6.5, 265.0);
	glRotated(xRotationArrow, 1.0, 0.0, 0.0);
	glRotated(yRotationArrow, 0.0, 1.0, 0.0);
	glTranslated(0.0, 6.5, -265.0);
	glBegin(GL_TRIANGLES);
		glVertex3f(1.0f, -6.5f, 265.0f);
		glVertex3f(0.0f, -6.5f, 260.0f);
		glVertex3f(-1.0f, -6.5f, 265.0f);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(-0.5f, -6.5f, 275.0f);
		glVertex3f(0.5f, -6.5f, 275.0f);
		glVertex3f(0.5f, -6.5f, 265.0f);
		glVertex3f(-0.5f, -6.5f, 265.0f);
	glEnd();
	glPopMatrix();
}

void reset(int value) {
	xRotation = 0;
	yRotation = 0;
	xRotationArrow = 0;
	yRotationArrow = 0;
	releaseFlag = 0;
	ballTranslationZ = 280.0;
	ballTranslationX = 0.0;
	ballTranslationY = -6.5;
	zReference = 280.0;
	xReference = 0.0;
	yReference = -6.5;
	speed = 1;
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawHorizontalWall(1);
	drawHorizontalWall(-1);
	drawVerticalWall(1);
	drawVerticalWall(-1);
	drawEndWall();
	drawSphere();
	drawArrow();

	glFlush();
}

void Anim() {
	if (releaseFlag == 1) {
		if(speed == 1)
			ballTranslationZ -= 0.1;
		else
			ballTranslationZ -= 0.02;
		ballTranslationX = xReference + ((zReference - ballTranslationZ) * tan(-yRotation * PI / 180));
		ballTranslationY = yReference + ((zReference - ballTranslationZ) * tan(xRotation * PI / 180));
	}
	if (ballTranslationX >= 24 || ballTranslationX <= -24) {
		zReference = ballTranslationZ;
		yReference = ballTranslationY;
		xReference = ballTranslationX;
		yRotation *= -1;
	}
	if (ballTranslationY >= 34 || ballTranslationY <= -34) {
		zReference = ballTranslationZ;
		yReference = ballTranslationY;
		xReference = ballTranslationX;
		xRotation *= -1;
	}
	if (ballTranslationZ <= -64) {
		releaseFlag = 0;
		xRotationSlow = xRotationArrow;
		yRotationSlow = yRotationArrow;
		glutTimerFunc(500, reset, 0);
	}
	glutPostRedisplay();
}

void moveArrow(int key, int x, int y) {
	if (ballTranslationZ < 280)
		return;
	if (key == GLUT_KEY_UP && xRotation < 70)
		xRotationArrow += 1;
	if (key == GLUT_KEY_DOWN && xRotation > -70)
		xRotationArrow -= 1;
	if (key == GLUT_KEY_RIGHT && yRotation > -70)
		yRotationArrow -= 1;
	if (key == GLUT_KEY_LEFT && yRotation < 70)
		yRotationArrow += 1;
}

void release(unsigned char key, int x, int y) {
	if (ballTranslationZ < 280)
		return;
	if (key == 'a') {
		xRotationArrow = xRotationSlow;
		yRotationArrow = yRotationSlow;
		xRotation = xRotationSlow;
		yRotation = yRotationSlow;
		speed = 0;
		releaseFlag = 1;
	}
	if (key == ' ') {
		releaseFlag = 1;
		xRotation = xRotationArrow;
		yRotation = yRotationArrow;
	}
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutSpecialFunc(moveArrow);
	glutKeyboardFunc(release);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 640 / 480, 0.1f, 400.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 300.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glutMainLoop();
}
