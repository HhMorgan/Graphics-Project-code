#define _USE_MATH_DEFINES
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <iostream>
#include "math.h"
#include <glut.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
void setupLight0();
void setupLight1();
void setupLight2();
void setupLight3();
void setupCamera();

int WIDTH = 1280;
int HEIGHT = 720;

GLfloat normalX;
GLfloat normalY;
GLfloat normalZ;

GLfloat constA = 1.0f;
GLfloat linearA = 0.0f;
GLfloat quadicA = 0.0f;

GLfloat posX = 0.0f;
GLfloat posY = 0.0f;
GLfloat posZ = 0.0f;

GLfloat posXS = 0.0f;
GLfloat posYS = 0.0f;
GLfloat posZS = 0.0f;
GLfloat cutOff = 45.0;
GLfloat lightexp = 0.0f;

GLfloat posXR = 0.0f;
GLfloat posYR = 0.0f;
GLfloat posZR = 0.0f;
GLfloat rotangle = 90.0;

float centerX = 0.0;
bool cameraRotate = false;
bool cameraTop = false;

GLfloat green[] = { 0.0f, 1.0f, 0.0f };
GLfloat blue[] = { 0.0f, 0.0f, 1.0f };
GLfloat lemonYellow[] = { 1.0f, 1.0f, 0.0f };
GLfloat pink[] = { 1.0f, 0.0f, 1.0f };
GLfloat skyBlue[] = { 0.0f, 1.0f, 1.0f };
GLfloat purple[] = { 0.6f, 0.0f, 1.0f };
GLfloat white[] = { 1.0f, 1.0f, 1.0f };
GLfloat drakGrey[] = { 0.4f, 0.4f, 0.4f };
GLfloat lightGrey[] = { 0.8f, 0.8f, 0.8f };
GLfloat drakBlue[] = { 0.05f, 0.05f, 0.28f };
GLfloat red[] = { 1.0f, 0.0f, 0.0f };
GLfloat black[] = { 0.0f, 0.0f, 0.0f };
GLfloat colorArray[8][3] = {
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f },
	{ 1.00000f, 0.61569f, 0.00000f },
	{ 0.0f, 1.0f, 1.0f },
	{ 0.6f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f }
};

GLfloat rgblightColor[] = { 255.0, 0.0, 0.0 };
GLfloat glLightColor[] = { 1.0, 0.0, 0.0 };
int stepNo = 0;

int lightColorCounter = 0;
using namespace std;
GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 5000;

// Textures
GLTexture tex_bar2;

//--------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f &v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f &v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 51.0f, float eyeY = 35.0f, float eyeZ = 4.0f, float centerX = 51.0f, float centerY = 34.0f, float centerZ = 3.0f, float upX = 1.0f, float upY = -0.3f, float upZ = -0.5f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
			);

	}
};
Camera camera;


void Special(int key, int x, int y) {
	float a = 2.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void changeColor(int movementSpeed) {

	GLfloat red = rgblightColor[0];
	GLfloat green = rgblightColor[1];
	GLfloat blue = rgblightColor[2];

	if (stepNo == 0) {
		if (green >= 255.0) {
			glLightColor[1] = 1.0;
			rgblightColor[1] = 255.0;
			stepNo = 1;
		}
		else {
			rgblightColor[1] += movementSpeed;
			glLightColor[1] = (rgblightColor[1] / 255.0);
		}
	}
	else if (stepNo == 1) {
		if (red <= 0.0) {
			glLightColor[0] = 0.0;
			rgblightColor[0] = 0.0;
			stepNo = 2;
		}
		else {
			rgblightColor[0] -= movementSpeed;
			glLightColor[0] = (rgblightColor[0] / 255.0);
		}
	}
	else if (stepNo == 2) {
		if (blue >= 255.0) {
			glLightColor[2] = 1.0;
			rgblightColor[2] = 255.0;
			stepNo = 3;
		}
		else {
			rgblightColor[2] += movementSpeed;
			glLightColor[2] = (rgblightColor[2] / 255.0);
		}
	}
	else if (stepNo == 3) {
		if (green <= 0.0) {
			glLightColor[1] = 0.0;
			rgblightColor[1] = 0.0;
			stepNo = 4;
		}
		else {
			rgblightColor[1] -= movementSpeed;
			glLightColor[1] = (rgblightColor[1] / 255.0);
		}
	}
	else if (stepNo == 4) {
		if (red >= 255.0) {
			glLightColor[0] = 1.0;
			rgblightColor[0] = 255.0;
			stepNo = 5;
		}
		else {
			rgblightColor[0] += movementSpeed;
			glLightColor[0] = (rgblightColor[0] / 255.0);
		}
	}
	else if (stepNo == 5) {
		if (blue <= 0.0) {
			glLightColor[2] = 0.0;
			rgblightColor[2] = 0.0;
			stepNo = 0;
		}
		else {
			rgblightColor[2] -= movementSpeed;
			glLightColor[2] = (rgblightColor[0] / 255.0);
		}
	}

}
GLvoid CalculateVectorNormal(GLfloat fVert1[], GLfloat fVert2[], GLfloat fVert3[], GLfloat *fNormalX, GLfloat *fNormalY, GLfloat *fNormalZ)
{
	GLfloat Qx, Qy, Qz, Px, Py, Pz;

	Qx = fVert2[0] - fVert1[0];
	Qy = fVert2[1] - fVert1[1];
	Qz = fVert2[2] - fVert1[2];
	Px = fVert3[0] - fVert1[0];
	Py = fVert3[1] - fVert1[1];
	Pz = fVert3[2] - fVert1[2];

	*fNormalX = Py * Qz - Pz * Qy;
	*fNormalY = Pz * Qx - Px * Qz;
	*fNormalZ = Px * Qy - Py * Qx;

}


void bindTexture(GLTexture texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.texture[0]);
}


class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS stage1;
Model_3DS stage2;
Model_3DS diablos;
Model_3DS griver;
Model_3DS slime;
Model_3DS bomb;
Model_3DS crash;
Model_3DS wall;
Model_3DS grass;
Model_3DS station;
Model_3DS key;
Model_3DS gate;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	/*/// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/


	//setupLight0();
	//setupLight1();
	//setupLight3();

}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	camera.look();
	//gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void setupLight0() {

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//  glMatrixMode(GL_MODELVIEW);
	//  glLoadIdentity();
	//  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);


	/*GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glMaterialfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_LIGHT1, GL_SPECULAR, specular);
	glMaterialfv(GL_LIGHT1, GL_SHININESS, shininess);*/

	GLfloat ambient[] = { 0, 0, 0, 1.0f };
	GLfloat diffuse[] = { 1, 1, 1, 1.0f };
	GLfloat specular[] = { 1, 1, 1, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glMaterialfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_LIGHT0, GL_SPECULAR, specular);
	glMaterialfv(GL_LIGHT0, GL_SHININESS, shininess);

	GLfloat lightIntensityA[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat lightIntensityS[] = { 1.f, 1.f, 1.0, 1.0f };
	GLfloat lightIntensityD[] = { 0.5f, 0.5f, 0.5, 1.0f };

	//lightColorGLfloat LightColor[] = { colorArray[lightColorCounter][0], colorArray[lightColorCounter][1], colorArray[lightColorCounter][2] };

	GLfloat lightPosition[] = { 0.0, 85.0, 0.0, 0 };

	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

	GLfloat blackColor[] = { 0.0f, 0.0f, 0.0f };

	//glLightfv(GL_LIGHT1, GL_AMBIENT, redLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, blackColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, blackColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);


	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constA);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, -3.0);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadicA);

	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10);

}

void setupLight1() {

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	GLfloat lightPosition[] = { 3, 7, 3, 1 };

	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

	GLfloat redLight[] = { 1.0, 0.0, 0.0 };

	GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, redLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, redLight);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);


	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 6);

}

void setupLight2() {

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);

	GLfloat lightPosition[] = { 0.1, 9, -9.2, 1 };

	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

	GLfloat redLight[] = { 1.0, 0.0, 0.0 };

	GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT2, GL_SPECULAR, redLight);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, redLight);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition);


	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, constA);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, linearA);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, quadicA);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 11);

}

void setupLight3() {

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT3);

	GLfloat lightPosition[] = { -9, 9, -9, 1 };

	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

	GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT3, GL_SPECULAR, glLightColor);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, glLightColor);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPosition);


	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.0);

	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 4);

}

void setupLight4() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT4);

	GLfloat lightPosition[] = { 3, 7, 3, 1 };

	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

	GLfloat redLight[] = { 1.0, 0.0, 0.0 };

	GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT4, GL_SPECULAR, glLightColor);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, glLightColor);
	glLightfv(GL_LIGHT4, GL_POSITION, lightPosition);


	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.0);

	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 7);
}

void setupLight5() {

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT5);

	GLfloat lightPosition[] = { -8.65, 9, 0.0, 1 };

	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

	GLfloat blueLight[] = { 0.0, 0.0, 1.0 };

	GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT5, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT5, GL_SPECULAR, blueLight);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, blueLight);
	glLightfv(GL_LIGHT5, GL_POSITION, lightPosition);


	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, constA);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, linearA);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, quadicA);

	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 11);

}


//=======================================================================
//idle Function
//=======================================================================
bool isStage1 = 1;
bool isStage2 = 0;
bool isKey1 = 1;
bool isKey2 = 1;
bool isKey3 = 1;
bool isKey4 = 1;
bool isKey5 = 1;
bool isKey6 = 1;
float moveCrashX = 0;
float moveCrashY = 0;
float moveCrashZ = 0;
float crashMotion1 = 0.3;
float crashMotion2 = 0.03;
float moveBombX = 0.0;
float moveBombY = 0.0;
float moveSlimeX = 0.0;
float moveSlimeY = 0.0;
float moveGriverX = 0.0;
float moveGriverY = 0.0;
float moveDiablosX = 0.0;
float moveDiablosY = 0.0;
float getCrashPosX(){
	return moveCrashX + 35 + 0.1;;
}

float getCrashPosZ(){
	return moveCrashZ + 35 + 0.1;;
}

bool collideCrashRectangle(float x, float z, float sidex, float sidez){
	float crashCurrentX = getCrashPosX();
	float crashCurrentZ = getCrashPosZ();
	//cout << "X : " <<crashCurrentX << "    " << x << ", Z : " <<crashCurrentZ << "     " << z<< "\n";
	if ((x > crashCurrentX - sidex && x < crashCurrentX + sidex) && (z > crashCurrentZ - sidez && z < crashCurrentZ + sidez)){
		cout << "=======================" << "\n";
		return true;
	}
	return false;
}

bool collideCrashSquare(float x, float z, float side){
	return collideCrashRectangle(x, z, side, side);
}

bool keysFunction(){
	bool result1 = 0;
	bool result2 = 0;
	bool result3 = 0;
	bool result4 = 0;
	bool result5 = 0;
	bool result6 = 0;
	if (isStage1){
		result1 |= collideCrashSquare(46.2, 54.6, 0.75);
		result2 |= collideCrashSquare(22.2, 69.6, 0.75);
		if (result1){
			isKey1 = 0;
		}
		if (result2){
			isKey2 = 0;
		}
	}
	if (isStage2){
		result3 |= collideCrashSquare(36.20, 34.62, 0.03);
		result4 |= collideCrashSquare(35.1, 34.08, 0.03);
		result5 |= collideCrashSquare(33.6, 32.79, 0.03);
		result6 |= collideCrashSquare(36.27, 32.79, 0.03);
		if (result3){
			isKey3 = 0;
		}
		if (result4){
			isKey4 = 0;
		}
		if (result5){
			isKey5 = 0;
		}
		if (result6){
			isKey6 = 0;
		}
	}
	return result1 | result2 | result3 | result4 | result5 | result6;
}

bool CollectKey(){
	bool result = 0;
	result = keysFunction();
	cout << "X : " << getCrashPosX() << ", Z : " << getCrashPosZ() << "\n";
	return result;
}


bool colideCrashWithAllObjectsForward(){
	bool result = 0;
	if (isStage1){
		//left border stage 1
		result |= collideCrashRectangle(53.1, 58.65 - crashMotion1, 1.05, 42.15);
		//Right border stage 1
		result |= collideCrashRectangle(17.7, 58.65 - crashMotion1, 1.05, 42.15);
		//back border stage 1
		result |= collideCrashRectangle(33.3, 27 - crashMotion1, 42.15, 1.05);
		//front border stage 1
		result |= collideCrashRectangle(33.3, 92 - crashMotion1, 42.15, 1.05);
		//middle left
		result |= collideCrashRectangle(47.4, 53.1 - crashMotion1, 4.8, 1.05);
		//middle left perpendicular
		result |= collideCrashRectangle(41.1, 54.6 - crashMotion1, 1.05, 6.3);
		//middle right perpendicular
		result |= collideCrashRectangle(31.2, 61.04 - crashMotion1, 1.05, 6.15);
		//middle right
		result |= collideCrashRectangle(29.55, 66 - crashMotion1, 2.85, 1.05);
		//mddle right right
		result |= collideCrashRectangle(27.3, 72.15 - crashMotion1, 1.05, 3.75);
		//right perpendicular gate
		result |= collideCrashRectangle(22.35, 75 - crashMotion1, 4.35, 1.05);
		//left perpendicular gate
		result |= collideCrashRectangle(45.9, 75 - crashMotion1, 6.3, 1.05);
		//Gate
		result |= collideCrashRectangle(33.75, 75 - crashMotion1, 7.65, 1.05);

	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(34.905, 35.49 + crashMotion2, 1.725, 0.05);
		result |= collideCrashRectangle(33.28, 32.94 + crashMotion2, 0.05, 2.76);
		result |= collideCrashRectangle(36.6, 32.955 + crashMotion2, 0.05, 2.745);
		result |= collideCrashRectangle(34.92, 30.52 + crashMotion2, 1.68, 0.05);
		result |= collideCrashRectangle(34.925, 31.71 + crashMotion2, 0.05, 0.51);
		result |= collideCrashRectangle(34.905, 32.16 + crashMotion2, 0.735, 0.05);
		result |= collideCrashRectangle(35.73, 31.74 + crashMotion2, 0.84, 0.05);
		result |= collideCrashRectangle(35.93, 32.76 + crashMotion2, 0.05, 0.52);
		result |= collideCrashRectangle(36.255, 33.01 + crashMotion2, 0.405, 0.05);
		result |= collideCrashRectangle(34.89, 33.37 + crashMotion2, 0.75, 0.05);
		result |= collideCrashRectangle(34.92, 33.855 + crashMotion2, 0.05, 0.495);
		result |= collideCrashRectangle(35.295, 34.38 + crashMotion2, 1.395, 0.05);
		result |= collideCrashRectangle(34.14, 34.695 + crashMotion2, 0.05, 0.315);
		result |= collideCrashRectangle(34.155, 35.37 + crashMotion2, 0.05, 0.18);
		result |= collideCrashRectangle(33.54, 33 + crashMotion2, 0.39, 0.05);
		result |= collideCrashRectangle(33.87, 32.76 + crashMotion2, 0.05, 0.51);
		//Gate
		result |= collideCrashRectangle(34.93, 30.865 + crashMotion2, 0.05, 0.365);
	}
	//cout << "X : " << getCrashPosX() << ", Z : " << getCrashPosZ() << "\n";
	return result;
}


bool colideCrashWithAllObjectsBackward(){
	bool result = 0;
	if (isStage1){
		//left border stage 1
		result |= collideCrashRectangle(53.1, 58.65 + crashMotion1, 1.05, 42.15);
		//Right border stage 1
		result |= collideCrashRectangle(17.7, 58.65 + crashMotion1, 1.05, 42.15);
		//back border stage 1
		result |= collideCrashRectangle(33.3, 27 + crashMotion1, 42.15, 1.05);
		//front border stage 1
		result |= collideCrashRectangle(33.3, 92 + crashMotion1, 42.15, 1.05);
		//middle left
		result |= collideCrashRectangle(47.4, 53.1 + crashMotion1, 4.8, 1.05);
		//middle left perpendicular
		result |= collideCrashRectangle(41.1, 54.6 + crashMotion1, 1.05, 6.3);
		//middle right perpendicular
		result |= collideCrashRectangle(31.2, 61.04 + crashMotion1, 1.05, 6.15);
		//middle right
		result |= collideCrashRectangle(29.55, 66 + crashMotion1, 2.85, 1.05);
		//mddle right right
		result |= collideCrashRectangle(27.3, 72.15 + crashMotion1, 1.05, 3.75);
		//right perpendicular gate
		result |= collideCrashRectangle(22.35, 75 + crashMotion1, 4.35, 1.05);
		//left perpendicular gate
		result |= collideCrashRectangle(45.9, 75 + crashMotion1, 6.3, 1.05);
		//Gate
		result |= collideCrashRectangle(33.75, 75 + crashMotion1, 7.65, 1.05);

	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(34.905, 35.49 - crashMotion2, 1.725, 0.05);
		result |= collideCrashRectangle(33.28, 32.94 - crashMotion2, 0.05, 2.76);
		result |= collideCrashRectangle(36.6, 32.955 - crashMotion2, 0.05, 2.745);
		result |= collideCrashRectangle(34.92, 30.52 - crashMotion2, 1.68, 0.05);
		result |= collideCrashRectangle(34.925, 31.71 - crashMotion2, 0.05, 0.51);
		result |= collideCrashRectangle(34.905, 32.16 - crashMotion2, 0.735, 0.05);
		result |= collideCrashRectangle(35.73, 31.74 - crashMotion2, 0.84, 0.05);
		result |= collideCrashRectangle(35.93, 32.76 - crashMotion2, 0.05, 0.52);
		result |= collideCrashRectangle(36.255, 33.01 - crashMotion2, 0.405, 0.05);
		result |= collideCrashRectangle(34.89, 33.37 - crashMotion2, 0.75, 0.05);
		result |= collideCrashRectangle(34.92, 33.855 - crashMotion2, 0.05, 0.495);
		result |= collideCrashRectangle(35.295, 34.38 - crashMotion2, 1.395, 0.05);
		result |= collideCrashRectangle(34.14, 34.695 - crashMotion2, 0.05, 0.315);
		result |= collideCrashRectangle(34.155, 35.37 - crashMotion2, 0.05, 0.18);
		result |= collideCrashRectangle(33.54, 33 - crashMotion2, 0.39, 0.05);
		result |= collideCrashRectangle(33.87, 32.76 - crashMotion2, 0.05, 0.51);
		//Gate
		result |= collideCrashRectangle(34.93, 30.865 - crashMotion2, 0.05, 0.365);
	}
	//cout << "X : " << getCrashPosX() << ", Z : " << getCrashPosZ() << "\n";
	return result;
}

bool colideCrashWithAllObjectsRight(){
	bool result = 0;
	if (isStage1){
		//left border stage 1
		result |= collideCrashRectangle(53.1 + crashMotion1, 58.65, 1.05, 42.15);
		//Right border stage 1
		result |= collideCrashRectangle(17.7 + crashMotion1, 58.65, 1.05, 42.15);
		//back border stage 1
		result |= collideCrashRectangle(33.3 + crashMotion1, 27, 42.15, 1.05);
		//front border stage 1
		result |= collideCrashRectangle(33.3 + crashMotion1, 92, 42.15, 1.05);
		//middle left
		result |= collideCrashRectangle(47.4 + crashMotion1, 53.1, 4.8, 1.05);
		//middle left perpendicular
		result |= collideCrashRectangle(41.1 + crashMotion1, 54.6, 1.05, 6.3);
		//middle right perpendicular
		result |= collideCrashRectangle(31.2 + crashMotion1, 61.04, 1.05, 6.15);
		//middle right
		result |= collideCrashRectangle(29.55 + crashMotion1, 66, 2.85, 1.05);
		//mddle right right
		result |= collideCrashRectangle(27.3 + crashMotion1, 72.15, 1.05, 3.75);
		//right perpendicular gate
		result |= collideCrashRectangle(22.35 + crashMotion1, 75, 4.35, 1.05);
		//left perpendicular gate
		result |= collideCrashRectangle(45.9 + crashMotion1, 75, 6.3, 1.05);
		//Gate
		result |= collideCrashRectangle(33.75 + crashMotion1, 75, 7.65, 1.05);

	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(34.905 - crashMotion2, 35.49, 1.725, 0.05);
		result |= collideCrashRectangle(33.28 - crashMotion2, 32.94, 0.05, 2.76);
		result |= collideCrashRectangle(36.6 - crashMotion2, 32.955, 0.05, 2.745);
		result |= collideCrashRectangle(34.92 - crashMotion2, 30.52, 1.68, 0.05);
		result |= collideCrashRectangle(34.925 - crashMotion2, 31.71, 0.05, 0.51);
		result |= collideCrashRectangle(34.905 - crashMotion2, 32.16, 0.735, 0.05);
		result |= collideCrashRectangle(35.73 - crashMotion2, 31.74, 0.84, 0.05);
		result |= collideCrashRectangle(35.93 - crashMotion2, 32.76, 0.05, 0.52);
		result |= collideCrashRectangle(36.255 - crashMotion2, 33.01, 0.405, 0.05);
		result |= collideCrashRectangle(34.89 - crashMotion2, 33.37, 0.75, 0.05);
		result |= collideCrashRectangle(34.92 - crashMotion2, 33.855, 0.05, 0.495);
		result |= collideCrashRectangle(35.295 - crashMotion2, 34.38, 1.395, 0.05);
		result |= collideCrashRectangle(34.14 - crashMotion2, 34.695, 0.05, 0.315);
		result |= collideCrashRectangle(34.155 - crashMotion2, 35.37, 0.05, 0.18);
		result |= collideCrashRectangle(33.54 - crashMotion2, 33, 0.39, 0.05);
		result |= collideCrashRectangle(33.87 - crashMotion2, 32.76, 0.05, 0.51);
		//Gate
		result |= collideCrashRectangle(34.93 - crashMotion2, 30.865, 0.05, 0.365);
	}
	//cout << "X : " << getCrashPosX() << ", Z : " << getCrashPosZ() << "\n";
	return result;
}

bool colideCrashWithAllObjectsLeft(){
	bool result = 0;
	if (isStage1){
		//left border stage 1
		result |= collideCrashRectangle(53.1 - crashMotion1, 58.65, 1.05, 42.15);
		//Right border stage 1
		result |= collideCrashRectangle(17.7 - crashMotion1, 58.65, 1.05, 42.15);
		//back border stage 1
		result |= collideCrashRectangle(33.3 - crashMotion1, 27, 42.15, 1.05);
		//front border stage 1
		result |= collideCrashRectangle(33.3 - crashMotion1, 92, 42.15, 1.05);
		//middle left
		result |= collideCrashRectangle(47.4 - crashMotion1, 53.1, 4.8, 1.05);
		//middle left perpendicular
		result |= collideCrashRectangle(41.1 - crashMotion1, 54.6, 1.05, 6.3);
		//middle right perpendicular
		result |= collideCrashRectangle(31.2 - crashMotion1, 61.04, 1.05, 6.15);
		//middle right
		result |= collideCrashRectangle(29.55 - crashMotion1, 66, 2.85, 1.05);
		//mddle right right
		result |= collideCrashRectangle(27.3 - crashMotion1, 72.15, 1.05, 3.75);
		//right perpendicular gate
		result |= collideCrashRectangle(22.35 - crashMotion1, 75, 4.35, 1.05);
		//left perpendicular gate
		result |= collideCrashRectangle(45.9 - crashMotion1, 75, 6.3, 1.05);
		//Gate
		result |= collideCrashRectangle(33.75 - crashMotion1, 75, 7.65, 1.05);

	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(34.905 + crashMotion2, 35.49, 1.725, 0.05);
		result |= collideCrashRectangle(33.28 + crashMotion2, 32.94, 0.05, 2.76);
		result |= collideCrashRectangle(36.6 + crashMotion2, 32.955, 0.05, 2.745);
		result |= collideCrashRectangle(34.92 + crashMotion2, 30.52, 1.68, 0.05);
		result |= collideCrashRectangle(34.925 + crashMotion2, 31.71, 0.05, 0.51);
		result |= collideCrashRectangle(34.905 + crashMotion2, 32.16, 0.735, 0.05);
		result |= collideCrashRectangle(35.73 + crashMotion2, 31.74, 0.84, 0.05);
		result |= collideCrashRectangle(35.93 + crashMotion2, 32.76, 0.05, 0.52);
		result |= collideCrashRectangle(36.255 + crashMotion2, 33.01, 0.405, 0.05);
		result |= collideCrashRectangle(34.89 + crashMotion2, 33.37, 0.75, 0.05);
		result |= collideCrashRectangle(34.92 + crashMotion2, 33.855, 0.05, 0.495);
		result |= collideCrashRectangle(35.295 + crashMotion2, 34.38, 1.395, 0.05);
		result |= collideCrashRectangle(34.14 + crashMotion2, 34.695, 0.05, 0.315);
		result |= collideCrashRectangle(34.155 + crashMotion2, 35.37, 0.05, 0.18);
		result |= collideCrashRectangle(33.54 + crashMotion2, 33, 0.39, 0.05);
		result |= collideCrashRectangle(33.87 + crashMotion2, 32.76, 0.05, 0.51);
		//Gate
		result |= collideCrashRectangle(34.93 + crashMotion2, 30.865, 0.05, 0.365);
	}
	//cout << "X : " << getCrashPosX() << ", Z : " << getCrashPosZ() << "\n";
	return result;
}

void idle() {

}
void Keyboard(unsigned char key, int x, int y) {
	float d = 2;
	float cameraMotion = 0.53;
	switch (key) {
	case 'p':
		std::cout << " camera.center.x = " << camera.center.x;
		std::cout << "; \n";
		std::cout << " camera.center.y = " << camera.center.y;
		std::cout << "; \n";
		std::cout << " camera.center.z = " << camera.center.z;
		std::cout << "; \n";
		std::cout << " camera.eye.x = " << camera.eye.x;
		std::cout << "; \n";
		std::cout << " camera.eye.y = " << camera.eye.y;
		std::cout << "; \n";
		std::cout << " camera.eye.z = " << camera.eye.z;
		std::cout << "; \n";
		std::cout << " camera.up.x = " << camera.up.x;
		std::cout << "; \n";
		std::cout << " camera.up.y = " << camera.up.y;
		std::cout << "; \n";
		std::cout << " camera.up.z = " << camera.up.z;
		std::cout << "; \n";

		std::cout << " POSX = " << posX;
		std::cout << "; \n";
		std::cout << " POSY = " << posY;
		std::cout << "; \n";
		std::cout << " POSZ = " << posZ;
		std::cout << "; \n";
		break;

	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 'i':
		//cout << collideCrash(35.21, 54.6 + crashMotion,0.06) << "\n";
		if (!colideCrashWithAllObjectsForward()){
			if (isStage1){
				moveCrashZ += crashMotion1;
			}
			else{
				if (isStage2){
					moveCrashZ -= crashMotion2;
				}
			}
		}
		//camera.moveZ(cameraMotion);
		cout << "Collection of Key" << CollectKey() << "\n";
		break;
	case 'k':
		//cout << collideCrash(35.21, 54.6 - crashMotion, 0.06) << "\n";
		if (!colideCrashWithAllObjectsBackward()){
			if (isStage1){
				moveCrashZ -= crashMotion1;
			}
			else{
				if (isStage2){
					moveCrashZ += crashMotion2;
				}
			}
		}
		cout << "Collection of Key" << CollectKey() << "\n";
		//camera.moveZ(-cameraMotion);
		break;
	case 'l':
		//cout << collideCrash(35.21 - crashMotion, 54.6, 0.06) << "\n";
		if (!colideCrashWithAllObjectsRight()){
			if (isStage1){
				moveCrashX -= crashMotion1;
			}
			else{
				if (isStage2){
					moveCrashX += crashMotion2;
				}
			}
		}
		cout << "Collection of Key" << CollectKey() << "\n";
		//camera.moveX(-cameraMotion);
		break;
	case 'j':
		//cout << collideCrash(35.21 + crashMotion, 54.6, 0.06) << "\n";
		if (!colideCrashWithAllObjectsLeft()){
			if (isStage1){
				moveCrashX += crashMotion1;
			}
			else{
				if (isStage2){
					moveCrashX -= crashMotion2;
				}
			}
		}
		//camera.moveX(cameraMotion);
		cout << "Collection of Key" << CollectKey() << "\n";
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glFlush();

	glutPostRedisplay();
}

//=======================================================================
// Display Function
//=======================================================================
void buildWall(float x, float progress, int count){
	for (int i = 0; i < count; i++){
		glPushMatrix();
		glTranslatef(x + (progress*i), 0, 52);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(-90.0, 0, 0, 1);
		glScaled(0.03, 0.03, 0.03);
		wall.Draw();
		glPopMatrix();
	}
}

void buildgrass(float x, float progress, int count){
	for (int i = 0; i < count; i++){
		glPushMatrix();
		glTranslatef(x + (progress*i), 0, 0);
		glRotatef(90.0, 1, 0, 0);
		grass.Draw();
		glPopMatrix();
	}
}

void multipleGrassPatch(float x1, float x2, float x3){
	//grass
	glPushMatrix();
	glTranslatef(x1, 0, 10);
	glRotatef(90.0, 0, 1, 0);
	buildgrass(-3, 0.7, 20);
	glPopMatrix();

	//grass
	glPushMatrix();
	glTranslatef(x2, 0, 26);
	glRotatef(90.0, 0, 1, 0);
	buildgrass(13, 0.7, 20);
	glPopMatrix();

	//grass
	glPushMatrix();
	glTranslatef(x3, 0, 26);
	glRotatef(90.0, 0, 1, 0);
	buildgrass(13, 0.7, 20);
	glPopMatrix();
}
void myDisplay(void) {
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//setupLight0();
	//setupLight1(); 
	//setupLight2();
	//setupLight3();
	//setupLight4();
	//setupLight5();

	//GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	/*GLfloat lightIntensity[] = { 1, 1, 1, 1.0f };
	GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);*/

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	//Draw Level 1
	if (isStage1){
		// Draw crash
		glPushMatrix();
		glTranslatef(moveCrashX, moveCrashY, moveCrashZ);
		glTranslatef(19, -0.15, -10);
		glScaled(18, 18, 18);

		glRotatef(90.0, 1, 0, 0);
		//glRotatef(205.f, 0, 0, 1);
		glScaled(0.001, 0.001, 0.001);
		crash.Draw();
		glPopMatrix();

		// Draw stage1
		glPushMatrix();
		glRotatef(-270.f, 1, 0, 0);
		stage1.Draw();
		glPopMatrix();

		//wall left side
		glPushMatrix();
		glTranslated(-50, 0, 90);
		glRotatef(90.0, 0, 1, 0);
		buildWall(35, 1.67, 50);
		glPopMatrix();

		//wall right side
		glPushMatrix();
		glTranslated(-15, 0, 90);
		glRotatef(90.0, 0, 1, 0);
		buildWall(35, 1.67, 50);
		glPopMatrix();

		//grass
		glPushMatrix();
		multipleGrassPatch(18, 19.5, 16.5);
		glPopMatrix();

		//grass
		glPushMatrix();
		glTranslatef(15, 0, 20);
		multipleGrassPatch(18, 19.5, 16.5);
		glPopMatrix();

		//grass
		glPushMatrix();
		glTranslatef(-12, 0, 15);
		multipleGrassPatch(18, 19.5, 16.5);
		glPopMatrix();

		//grass
		glPushMatrix();
		glTranslatef(3, 0, 33);
		multipleGrassPatch(18, 19.5, 16.5);
		glPopMatrix();

		//grass
		glPushMatrix();
		glTranslatef(-2, 0, 33);
		multipleGrassPatch(18, 19.5, 16.5);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(71, 0, 60);
		glRotatef(180.0, 0, 1, 0);
		buildWall(35, 1.67, 7);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(-27, 0, 50);
		glRotatef(90.0, 0, 1, 0);
		buildWall(35, 1.67, 7);
		glPopMatrix();


		//wall
		glPushMatrix();
		glTranslated(-37, 0, 55);
		glRotatef(90.0, 0, 1, 0);
		buildWall(35, 1.67, 6);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(50.1, 0, 73);
		glRotatef(180.0, 0, 1, 0);
		buildWall(35, 1.67, 3);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(-41, 0, 65);
		glRotatef(90.0, 0, 1, 0);
		buildWall(35, 1.67, 6);
		glPopMatrix();

		//gate
		glPushMatrix();
		glTranslatef(18, 0, 30);
		glScaled(0.03, 0.03, 0.03);
		glRotatef(90.0, 1, 0, 0);
		gate.Draw();
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(70.5, 0, 81.7);
		glRotatef(180.0, 0, 1, 0);
		buildWall(35, 1.67, 7);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(45, 0, 81.7);
		glRotatef(180.0, 0, 1, 0);
		buildWall(35, 1.67, 5);
		glPopMatrix();

		//teleporter
		glPushMatrix();
		glTranslatef(18.5, 0, 42);
		glRotatef(90.0, 1, 0, 0);
		glRotatef(180.0, 0, 0, 1);
		//glRotatef(-90.0, 0, 0, 1);
		//glRotatef(260.f, 0, 0, 1);
		station.Draw();
		glPopMatrix();

		//bomb
		glPushMatrix();
		glTranslatef(30, 0, 18);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(90.0, 0, 1, 0);
		glRotatef(260.f, 0, 0, 1);
		glScaled(0.0003, 0.0003, 0.0003);
		bomb.Draw();
		glPopMatrix();

		//slime
		glPushMatrix();
		glTranslatef(6, 0, 18);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(90.0, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		glScaled(0.0003, 0.0003, 0.0003);
		slime.Draw();
		glPopMatrix();

		if (isKey1){
			//collectable key1
			glPushMatrix();
			glTranslatef(30, -1, 10);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			key.Draw();
			glPopMatrix();
		}

		if (isKey2){
			//collectable key2
			glPushMatrix();
			glTranslatef(6, -1, 25);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			key.Draw();
			glPopMatrix();
		}

	}


	//Draw Level 2
	if (isStage2){

		//Draw Stage 2
		glPushMatrix();
		glTranslatef(50, 0, 10);
		glRotatef(90.0f, 1, 0, 0);
		glScaled(0.025, 0.025, 0.025);
		stage2.Draw();
		glPopMatrix();

		//Draw Crash
		glPushMatrix();
		glTranslatef(55, -0.15, 45);
		glScaled(18, 18, 18);
		glTranslatef(moveCrashX, moveCrashY, moveCrashZ);
		glRotatef(90.0, 1, 0, 0);
		glRotatef(205.f, 0, 0, 1);
		glScaled(0.001, 0.001, 0.001);
		crash.Draw();
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(-30, 0, 90);
		glRotatef(90, 0, 1, 0);
		buildWall(35, 1.67, 60);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(-30, 0, 90);
		glRotatef(90, 0, 1, 0);
		buildWall(35, 1.67, 60);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(30, 0, 90);
		glRotatef(90, 0, 1, 0);
		buildWall(35, 1.67, 60);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(30, 0, 90);
		glRotatef(90, 0, 1, 0);
		buildWall(35, 1.67, 60);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(-12, 0, 0);
		buildWall(35, 1.67, 36);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(-12, 0, -90);
		buildWall(35, 1.67, 36);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(0, 0, -20);
		buildWall(35, 1.67, 29);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(5, 0, -38.5);
		buildWall(35, 1.67, 15);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(5, 0, -60);
		buildWall(35, 1.67, 15);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(-12.9, 0, -45);
		buildWall(35, 1.67, 7);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(85, 0, -40);
		glRotatef(270, 0, 1, 0);
		buildWall(35, 1.67, 10);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(36, 0, -45);
		buildWall(35, 1.67, 7);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(17.9, 0, -68);
		buildWall(35, 1.67, 18);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(122, 0, -40);
		glRotatef(270, 0, 1, 0);
		buildWall(35, 1.67, 10);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(90, 0, 13);
		glRotatef(270, 0, 1, 0);
		buildWall(35, 1.67, 3);
		glPopMatrix();

		//wall
		glPushMatrix();
		glTranslated(90, 0, -2);
		glRotatef(270, 0, 1, 0);
		buildWall(35, 1.67, 6);
		glPopMatrix();


		//wall
		glPushMatrix();
		glTranslated(0, 0, 66);
		glRotatef(90, 0, 1, 0);
		buildWall(35, 1.67, 11);
		glPopMatrix();


		//wall
		glPushMatrix();
		glTranslated(0, 0, 26);
		glRotatef(90, 0, 1, 0);
		buildWall(35, 1.67, 10);
		glPopMatrix();

		//gate
		glPushMatrix();
		glTranslatef(52.2, 0, -31.5);
		glScaled(0.03, 0.03, 0.03);
		glRotatef(90.0, 1, 0, 0);
		glRotatef(-90.0, 0, 0, 1);
		gate.Draw();
		glPopMatrix();

		//griver
		glPushMatrix();
		glTranslatef(70, 0, 20);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(90.0, 0, 1, 0);
		glRotatef(150.f, 0, 0, 1);
		glScaled(0.0003, 0.0003, 0.0003);
		griver.Draw();
		glPopMatrix();

		//griver
		glPushMatrix();
		glTranslatef(40, 0, 20);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(90.0, 0, 1, 0);
		glRotatef(320.0f, 0, 0, 1);
		glScaled(0.0003, 0.0003, 0.0003);
		griver.Draw();
		glPopMatrix();

		//diablos
		glPushMatrix();
		glTranslatef(50, 0, 3);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(90.0, 0, 1, 0);
		glRotatef(260.f, 0, 0, 1);
		glScaled(0.0003, 0.0003, 0.0003);
		diablos.Draw();
		glPopMatrix();

		if (isKey3){
			//collectable Key3
			glPushMatrix();
			glTranslatef(75, -1, 36);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			key.Draw();
			glPopMatrix();
		}

		if (isKey4){
			//collectable Key4
			glPushMatrix();
			glTranslatef(55, -1, 26);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			key.Draw();
			glPopMatrix();
		}

		if (isKey5){
			//collectable key5
			glPushMatrix();
			glTranslatef(28, -1, 3);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			key.Draw();
			glPopMatrix();
		}

		if (isKey6){
			//collectable key6
			glPushMatrix();
			glTranslatef(76, -1, 3);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			key.Draw();
			glPopMatrix();
		}

		//teleporter
		glPushMatrix();
		glTranslatef(70, 0, -32);
		glRotatef(90.0, 1, 0, 0);
		glRotatef(90, 0, 0, 1);
		//glRotatef(-90.0, 0, 0, 1);
		//glRotatef(260.f, 0, 0, 1);
		station.Draw();
		glPopMatrix();
	}

	// Draw crash
	glPushMatrix();
	glTranslatef(35, -0.15, 55);
	glScaled(18, 18, 18);
	glTranslatef(moveCrashX, moveCrashY, moveCrashZ);
	glRotatef(90.0, 1, 0, 0);
	glRotatef(205.f, 0, 0, 1);
	glScaled(0.001, 0.001, 0.001);
	//crash.Draw();
	glPopMatrix();

	//bomb
	glPushMatrix();
	glTranslatef(35, 0, 52);
	glRotatef(90.0, 0, 0, 1);
	glRotatef(90.0, 0, 1, 0);
	glRotatef(260.f, 0, 0, 1);
	glScaled(0.0003, 0.0003, 0.0003);
	//bomb.Draw();
	glPopMatrix();


	//slime
	glPushMatrix();
	glTranslatef(35, 0, 52);
	glRotatef(90.0, 0, 0, 1);
	glRotatef(90.0, 0, 1, 0);
	glRotatef(260.f, 0, 0, 1);
	glScaled(0.0003, 0.0003, 0.0003);
	//slime.Draw();
	glPopMatrix();


	//griver
	glPushMatrix();
	glTranslatef(35, 0, 52);
	glRotatef(90.0, 0, 0, 1);
	glRotatef(90.0, 0, 1, 0);
	glRotatef(260.f, 0, 0, 1);
	glScaled(0.0003, 0.0003, 0.0003);
	//griver.Draw();
	glPopMatrix();


	//diablos
	glPushMatrix();
	glTranslatef(35, 0, 52);
	glRotatef(90.0, 0, 0, 1);
	glRotatef(90.0, 0, 1, 0);
	glRotatef(260.f, 0, 0, 1);
	glScaled(0.0003, 0.0003, 0.0003);
	//diablos.Draw();
	glPopMatrix();


	//wall
	glPushMatrix();
	//buildWall(5);
	glPopMatrix();


	//grass
	glPushMatrix();
	glTranslatef(35, 0, 52);
	glRotatef(90.0, 1, 0, 0);
	//glRotatef(-90.0, 0, 0, 1);
	//glRotatef(260.f, 0, 0, 1);
	//grass.Draw();
	glPopMatrix();

	//teleporter
	glPushMatrix();
	glTranslatef(35, 0, 52);
	glRotatef(90.0, 1, 0, 0);
	//glRotatef(-90.0, 0, 0, 1);
	//glRotatef(260.f, 0, 0, 1);
	//station.Draw();
	glPopMatrix();

	//gate
	glPushMatrix();
	glTranslatef(35, 0, 52);
	glScaled(0.03, 0.03, 0.03);
	glRotatef(90.0, 1, 0, 0);
	//glRotatef(-90.0, 0, 0, 1);
	//glRotatef(260.f, 0, 0, 1);
	//gate.Draw();
	glPopMatrix();

	//collectable
	glPushMatrix();
	glTranslatef(35, -1, 53);
	glScaled(0.025, 0.025, 0.025);
	glRotatef(90.0, 1, 0, 0);
	//glRotatef(-90.0, 0, 0, 1);
	//glRotatef(260.f, 0, 0, 1);
	//key.Draw();
	glPopMatrix();


	glutSwapBuffers();
}


//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	//gluLookAt(Eye.x, Eye.y , Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters
	camera.look();

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}
//=======================================================================
// Reshape Function
/*
camera.center.x = 50.77;
camera.center.y = 56.9414;
camera.center.z = 35.0442;
camera.eye.x = 50.865;
camera.eye.y = 57.8846;
camera.eye.z = 34.7259;
camera.up.x = 0.994553;
camera.up.y = -0.0761497;
camera.up.z = 0.071165;

camera.center.x = 50.894;
camera.center.y = 56.7377;
camera.center.z = 35.235;
camera.eye.x = 50.9432;
camera.eye.y = 57.6842;
camera.eye.z = 34.9161;
camera.up.x = 0.998504;
camera.up.y = -0.0542527;
camera.up.z = -0.00684152;


camera.center.x = 52.1259;
camera.center.y = 54.6749;
camera.center.z = 36.5773;
camera.eye.x = 52.5412;
camera.eye.y = 55.5844;
camera.eye.z = 36.5561;
camera.up.x = 0.907619;
camera.up.y = -0.412626;
camera.up.z = 0.0772416;

camera.center.x = 34.3787;
camera.center.y = 50.7865;
camera.center.z = 56.4445;
camera.eye.x = 33.924;
camera.eye.y = 50.8932;
camera.eye.z = 57.3288;
camera.up.x = 0.00511577;
camera.up.y = 0.993105;
camera.up.z = -0.117116;

camera.center.x = 35.0334;
camera.center.y = 51.0412;
camera.center.z = 56.8944;
camera.eye.x = 34.6759;
camera.eye.y = 51.0482;
camera.eye.z = 57.8283;
camera.up.x = -0.0357871;
camera.up.y = 0.999137;
camera.up.z = -0.0210868;

camera.center.x = 35.0406;
camera.center.y = 50.8414;
camera.center.z = 56.8986;
camera.eye.x = 34.6831;
camera.eye.y = 50.8484;
camera.eye.z = 57.8325;
camera.up.x = -0.0357871;
camera.up.y = 0.999137;
camera.up.z = -0.0210868;


camera.center.x = 33.3375;
camera.center.y = 2.09296;
camera.center.z = 60.2626;
camera.eye.x = 33.1121;
camera.eye.y = 2.10409;
camera.eye.z = 61.2368;
camera.up.x = -0.0184325;
camera.up.y = 0.999707;
camera.up.z = -0.0156873;

camera.center.x = 23.0649;
camera.center.y = 72.5263;
camera.center.z = 70.1695;
camera.eye.x = 23.1202;
camera.eye.y = 73.3746;
camera.eye.z = 70.6962;
camera.up.x = 0.000971282;
camera.up.y = 0.527399;
camera.up.z = -0.849617;

camera.center.x = 18.1594;
camera.center.y = 2.57471;
camera.center.z = -15.7186;
camera.eye.x = 18.0868;
camera.eye.y = 2.56383;
camera.eye.z = -16.7159;
camera.up.x = -0.00104444;
camera.up.y = 0.999941;
camera.up.z = -0.0108271;

camera.center.x = 33.3375;
camera.center.y = 2.09296;
camera.center.z = 60.2626;
camera.eye.x = 33.1121;
camera.eye.y = 2.10409;
camera.eye.z = 61.2368;
camera.up.x = -0.0184325;
camera.up.y = 0.999707;
camera.up.z = -0.0156873;

camera.center.x = 48.4781;
camera.center.y = 39.7621;
camera.center.z = 39.8605;
camera.eye.x = 48.4214;
camera.eye.y = 40.76;
camera.eye.z = 39.8914;
camera.up.x = 0.0298803;
camera.up.y = 0.0326301;
camera.up.z = -0.999021;

*/
//=======================================================================
boolean initializeCamera = 1;
void setupCamera() {

	if (initializeCamera){
		if (isStage2){
			camera.center.x = 53.0975;
			camera.center.y = 17.0528;
			camera.center.z = 57.6918;
			camera.eye.x = 53.1114;
			camera.eye.y = 17.5563;
			camera.eye.z = 58.5557;
			camera.up.x = -0.0262019;
			camera.up.y = 0.863837;
			camera.up.z = -0.50309;

			camera.center.x = 52.4833;
			camera.center.y = 47.3171;
			camera.center.z = 52.1107;
			camera.eye.x = 52.4751;
			camera.eye.y = 48.2794;
			camera.eye.z = 52.3827;
			camera.up.x = -0.028508;
			camera.up.y = 0.271653;
			camera.up.z = -0.961973;
		}
		if (isStage1){
			camera.center.x = 18.1594;
			camera.center.y = 2.57471;
			camera.center.z = -15.7186;
			camera.eye.x = 18.0868;
			camera.eye.y = 2.56383;
			camera.eye.z = -16.7159;
			camera.up.x = -0.00104444;
			camera.up.y = 0.999941;
			camera.up.z = -0.0108271;
		}

		initializeCamera = 0;
	}
	/*if (moveCameraThirdPerson){
	camera.center.x += 0.1;
	camera.center.y = 56.7377;
	camera.center.z = 35.235;
	camera.eye.x = 50.9432;
	camera.eye.y = 57.6842;
	camera.eye.z = 34.9161;
	camera.up.x = 0.998504;
	camera.up.y = -0.0542527;
	camera.up.z = -0.00684152;
	}*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}


//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	stage1.Load("Models/FFCCABS/02085_raw.3ds");
	stage2.Load("Models/KHBCB/main/main.3ds");
	bomb.Load("Models/FFBo/Bomb.3ds");
	slime.Load("Models/FFBl/blobra.3ds");
	griver.Load("Models/FFGr/griever.3ds");
	diablos.Load("Models/FFDi/diabolos.3ds");
	wall.Load("Models/wallStone/stoneWall.3DS");
	grass.Load("Models/Grass/Grass/Grass.3ds");
	station.Load("Models/station/Computer Panel/main_panel_ma01.3ds");
	gate.Load("Models/gate/Gate 2/Gate_01.3DS");
	key.Load("Models/collect/KingdomKey/KingdomKey.3ds");
	crash.Load("Models/crash/crashbandicoot.3ds");

	loadBMP(&tex, "Textures/sky4-jpg.bmp", true);
}

void lightColorAnim(int value)
{
	//std::cout << "LIGHT COLOR" << std::endl;
	//std::cout << lightColorCounter << std::endl;
	//std::cout << "LIGHT COLOR" << std::endl;
	//if (lightColorCounter < 8)
	//	lightColorCounter++;
	//else
	//	lightColorCounter = 0;
	changeColor(20);

	/*std::cout << "" << std::endl;
	std::cout << "RGB" << std::endl;
	std::cout << rgblightColor[0] << std::endl;
	std::cout << rgblightColor[1] << std::endl;
	std::cout << rgblightColor[2] << std::endl;
	std::cout << "RGB" << std::endl;*/

	glutPostRedisplay();
	glutTimerFunc(100, lightColorAnim, 0);

}


void cameraAnim(int value) {

	if (cameraRotate) {
		centerX = 1.0;
		camera.rotateY(centerX);
	}

	glutPostRedisplay();
	glutTimerFunc(1, cameraAnim, 0);

}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	//setupCamera();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(Keyboard);

	glutSpecialFunc(Special);

	glutIdleFunc(idle);
	//glutMotionFunc(myMotion);

	//glutMouseFunc(myMouse);

	//glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutTimerFunc(0, lightColorAnim, 0);
	glutTimerFunc(0, cameraAnim, 0);

	glutMainLoop();
}