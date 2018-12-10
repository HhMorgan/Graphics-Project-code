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
char title[] = "Crash Bandicoot's Wild Adventure";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 5000;

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


bool rotateCameraFlag = 0;

void mouseClicks(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		rotateCameraFlag = !rotateCameraFlag;
	}
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
Model_3DS diablos_left_wing;
Model_3DS diablos_right_wing;
Model_3DS griver;
Model_3DS griver_left_wing;
Model_3DS griver_right_wing;
Model_3DS slime;
Model_3DS bomb;
Model_3DS crash;
Model_3DS crash_right_arm;
Model_3DS crash_left_arm;
Model_3DS crash_right_leg;
Model_3DS crash_left_leg;
Model_3DS wall;
Model_3DS grass;
Model_3DS station;
Model_3DS key;
Model_3DS gate;


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


	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}


//=======================================================================
//Global variables
//=======================================================================
bool isStage1 = 1;
bool isStage2 = 0;
bool isKey1 = 1;
bool isKey2 = 1;
bool isKey3 = 1;
bool isKey4 = 1;
bool isKey5 = 1;
bool isKey6 = 1;
bool isCrash = 1;
bool isGate1 = 1;
bool isGate2 = 1;
bool freeRoam = 0;
bool isCrashMoving = 0;
bool initializeCamera = 1;
bool isThirdPersonView = 1;
bool isTopView = 0;
bool isGriver1 = 1;
bool isGriver2 = 1;
bool isDiablos = 1;
float crashAnim = 5;
float crashAnimConstant = 1.5;
float griverAnim = 5;
float griverAnimConstant = 0.25;
float rotateKeys = 0;
int rotateCamera = 0;
float crashIdleAnim = 1.005;
float crashRotate = 0;
float moveCrashX = 0;
float moveCrashY = 0;
float moveCrashZ = 0;
float crashMotion1 = 0.3;
float crashMotion2 = 0.3;
float moveBombX = 0.0;
float moveBombZ = 0.0;
int rotateBomb = 0;
float moveSlimeX = 0.0;
float moveSlimeZ = 0.0;
int rotateSlime = 0;
float moveGriver1X = 0.0;
float moveGriver1Z = 0.0;
int rotateGriver1 = 0;
float moveGriver2X = 0.0;
float moveGriver2Z = 0.0;
int rotateGriver2 = -90;
float moveDiablosX = 0.0;
float moveDiablosZ = 0.0;
int rotateDiablos = 90;
float LightRotate = 0;
int rotateThirdPerson = 0;
POINT mousePos;


void setupLight0() {

	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);

	GLfloat ambient[] = { 0.05f, 0.05f, 0.05, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 15.0f, 15.0f, 15.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glPushMatrix();
	glTranslatef(18, 15, 15);
	glRotatef(LightRotate * 360, 0, 1, 0);
	GLfloat lightpos[4] = { -18.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glPopMatrix();

}
void setupLight1() {

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	GLfloat ambient[] = { 0.05f, 0.05f, 0.05, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 15.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glPushMatrix();
	glTranslatef(50, 30, 10);
	glRotatef(LightRotate * 360, 0, 1, 0);
	GLfloat lightpos[4] = { -30.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glPopMatrix();

}


float getCrashPosX(){
	return moveCrashX + 35 + 0.1;;
}

float getCrashPosZ(){
	return moveCrashZ + 35 + 0.1;;
}

bool collideCrashRectangle(float x, float z, float sidex, float sidez){
	float crashCurrentX = getCrashPosX();
	float crashCurrentZ = getCrashPosZ();
	if ((x > crashCurrentX - sidex && x < crashCurrentX + sidex) && (z > crashCurrentZ - sidez && z < crashCurrentZ + sidez)){
		return true;
	}
	return false;
}

bool collideCrashSquare(float x, float z, float side){
	return collideCrashRectangle(x, z, side, side);
}

void crashAnimation(){
	if (crashAnim > 5)
		crashAnimConstant *= -1;
	else if (crashAnim < -5)
		crashAnimConstant *= -1;

	crashAnim += crashAnimConstant;
}

void griverAnimation(){
	if (griverAnim > 7)
		griverAnimConstant *= -1;
	else if (griverAnim < -7)
		griverAnimConstant *= -1;

	griverAnim += griverAnimConstant;
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
		result3 |= collideCrashSquare(54.9, 26.1, 0.75);
		result4 |= collideCrashSquare(35.1, 15.9, 0.75);
		result5 |= collideCrashSquare(8.10002, -7.19994, 0.75);
		result6 |= collideCrashSquare(56.1, -7.19994, 0.75);
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
float bombCollisionsX = 46.2;
float bombCollisionsZ = 61.8;
float slimeCollisionsX = 22.05;
float slimeCollisionsZ = 62.7;
float griever1CollisionsX = 50.1;
float griever1CollisionsZ = 9.60002;
float griever2CollisionsX = 19.8;
float griever2CollisionsZ = 9.90001;
float diablosCollisionsX = 30;
float diablosCollisionsZ = -6.89994;

bool isCrashDead(){
	bool result = 0;
	if (isStage1){
		//Slime
		result |= collideCrashRectangle(slimeCollisionsX, slimeCollisionsZ, 2.25, 1.05);
		//Bomb
		result |= collideCrashSquare(bombCollisionsX, bombCollisionsZ, 1.05);
	}
	if (isStage2){
		//Griever1
		result |= collideCrashSquare(griever1CollisionsX, griever1CollisionsZ, 1.3);

		//Griever2
		result |= collideCrashSquare(griever2CollisionsX, griever2CollisionsZ, 1.3);

		//Diablos
		result |= collideCrashSquare(diablosCollisionsX, diablosCollisionsZ, 1.05);
	}
	return result;
}
void isEnemyDead(){
	if (isStage2){
		if (collideCrashSquare(griever1CollisionsX, griever1CollisionsZ, 1.3)){
			isGriver1 = 0;
		}
		if (collideCrashSquare(griever2CollisionsX, griever2CollisionsZ, 1.3)){
			isGriver2 = 0;
		}
		//Diablos
		if (isDiablos && collideCrashSquare(diablosCollisionsX, diablosCollisionsZ, 1.05)){
			isDiablos = 0;
		}
	}
}

bool stageEnd(){
	bool result = 0;
	if (isStage1){
		result |= collideCrashSquare(34.74, 86.9699, 1.05);
	}
	if (isStage2){
		result |= collideCrashSquare(50.1, -42.3, 1.05);
	}
	return result;
}

bool isKeysCollected(){
	if (isStage1){
		if (!isKey1 && !isKey2){
			return 1;
		}
	}
	if (isStage2){
		if (!isKey3 && !isKey4 && !isKey5 && !isKey6){
			return 1;
		}
	}
	return 0;
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
		if (isGate1){
			//Gate
			result |= collideCrashRectangle(33.75, 75 - crashMotion1, 7.65, 1.05);
		}

	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(62.1f, -4.500099999999996f + crashMotion2, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.95f, 41.7f + crashMotion2, 31.349999999999998f, 1.05);
		result |= collideCrashRectangle(2.1f, -4.500099999999996f + crashMotion2, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.9f, -48.3001f + crashMotion2, 31.7f, 1.05);
		result |= collideCrashRectangle(32.1f, -26.549950000000003f + crashMotion2, 1.05, 9.150050000000002f);
		result |= collideCrashRectangle(47.25f, -26.0999f + crashMotion2, 16.049999999999997f, 1.05);
		result |= collideCrashRectangle(32.1f, -18.2999f + crashMotion2, 13.5f, 1.05);
		result |= collideCrashRectangle(50.1f, -7.649935000000001f + crashMotion2, 1.05, 9.149965f);
		result |= collideCrashRectangle(56.099999999999994f, -3.29996f + crashMotion2, 7.199999999999999f, 1.05);
		result |= collideCrashRectangle(31.5f, 3.30003f + crashMotion2, 13.5f, 1.05);
		result |= collideCrashRectangle(32.1f, 12.750015f + crashMotion2, 1.05, 9.449985f);
		result |= collideCrashRectangle(13.2f, -7.649985000000001f + crashMotion2, 1.05, 8.850014999999999f);
		result |= collideCrashRectangle(7.65f, -2.9996f + crashMotion2, 6.75f, 1.05);
		result |= collideCrashRectangle(38.55f, 21.9f + crashMotion2, 25.35f, 1.05);
		result |= collideCrashRectangle(18.0f, 26.7f + crashMotion2, 1.05, 5.699999999999999f);
		result |= collideCrashRectangle(18.0f, 39.45f + crashMotion2, 1.05, 3.150000000000002f);

		if (isGate2){
			//Gate
			result |= collideCrashRectangle(32.1f, -40.99995f + crashMotion2, 1.05, 6.500050000000002f);
		}

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
		if (isGate1){
			//Gate
			result |= collideCrashRectangle(33.75, 75 + crashMotion1, 7.65, 1.05);
		}
	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(62.1f, -4.500099999999996f - crashMotion2, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.95f, 41.7f - crashMotion2, 31.349999999999998f, 1.05);
		result |= collideCrashRectangle(2.1f, -4.500099999999996f - crashMotion2, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.9f, -48.3001f - crashMotion2, 31.7f, 1.05);
		result |= collideCrashRectangle(32.1f, -26.549950000000003f - crashMotion2, 1.05, 9.150050000000002f);
		result |= collideCrashRectangle(47.25f, -26.0999f - crashMotion2, 16.049999999999997f, 1.05);
		result |= collideCrashRectangle(32.1f, -18.2999f - crashMotion2, 13.5f, 1.05);
		result |= collideCrashRectangle(50.1f, -7.649935000000001f - crashMotion2, 1.05, 9.149965f);
		result |= collideCrashRectangle(56.099999999999994f, -3.29996f - crashMotion2, 7.199999999999999f, 1.05);
		result |= collideCrashRectangle(31.5f, 3.30003f - crashMotion2, 13.5f, 1.05);
		result |= collideCrashRectangle(32.1f, 12.750015f - crashMotion2, 1.05, 9.449985f);
		result |= collideCrashRectangle(13.2f, -7.649985000000001f - crashMotion2, 1.05, 8.850014999999999f);
		result |= collideCrashRectangle(7.65f, -2.9996f - crashMotion2, 6.75f, 1.05);
		result |= collideCrashRectangle(38.55f, 21.9f - crashMotion2, 25.35f, 1.05);
		result |= collideCrashRectangle(18.0f, 26.7f - crashMotion2, 1.05, 5.699999999999999f);
		result |= collideCrashRectangle(18.0f, 39.45f - crashMotion2, 1.05, 3.150000000000002f);

		if (isGate2){
			//Gate
			result |= collideCrashRectangle(32.1f, -40.99995f - crashMotion2, 1.05, 6.500050000000002f);
		}
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
		if (isGate1){
			//Gate
			result |= collideCrashRectangle(33.75 + crashMotion1, 75, 7.65, 1.05);
		}

	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(62.1f - crashMotion2, -4.500099999999996f, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.95f - crashMotion2, 41.7f, 31.349999999999998f, 1.05);
		result |= collideCrashRectangle(2.1f - crashMotion2, -4.500099999999996f, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.9f - crashMotion2, -48.3001f, 31.7f, 1.05);
		result |= collideCrashRectangle(32.1f - crashMotion2, -26.549950000000003f, 1.05, 9.150050000000002f);
		result |= collideCrashRectangle(47.25f - crashMotion2, -26.0999f, 16.049999999999997f, 1.05);
		result |= collideCrashRectangle(32.1f - crashMotion2, -18.2999f, 13.5f, 1.05);
		result |= collideCrashRectangle(50.1f - crashMotion2, -7.649935000000001f, 1.05, 9.149965f);
		result |= collideCrashRectangle(56.099999999999994f - crashMotion2, -3.29996f, 7.199999999999999f, 1.05);
		result |= collideCrashRectangle(31.5f - crashMotion2, 3.30003f, 13.5f, 1.05);
		result |= collideCrashRectangle(32.1f - crashMotion2, 12.750015f, 1.05, 9.449985f);
		result |= collideCrashRectangle(13.2f - crashMotion2, -7.649985000000001f, 1.05, 8.850014999999999f);
		result |= collideCrashRectangle(7.65f - crashMotion2, -2.9996f, 6.75f, 1.05);
		result |= collideCrashRectangle(38.55f - crashMotion2, 21.9f, 25.35f, 1.05);
		result |= collideCrashRectangle(18.0f - crashMotion2, 26.7f, 1.05, 5.699999999999999f);
		result |= collideCrashRectangle(18.0f - crashMotion2, 39.45f, 1.05, 3.150000000000002f);

		if (isGate2){
			//Gate
			result |= collideCrashRectangle(32.1f - crashMotion2, -40.99995f, 1.05, 6.500050000000002f);
		}
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
		if (isGate1){
			//Gate
			result |= collideCrashRectangle(33.75 - crashMotion1, 75, 7.65, 1.05);
		}

	}
	if (isStage2){
		//Walls
		result |= collideCrashRectangle(62.1f + crashMotion2, -4.500099999999996f, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.95f + crashMotion2, 41.7f, 31.349999999999998f, 1.05);
		result |= collideCrashRectangle(2.1f + crashMotion2, -4.500099999999996f, 1.05, 50.700100000000006f);
		result |= collideCrashRectangle(31.9f + crashMotion2, -48.3001f, 31.7f, 1.05);
		result |= collideCrashRectangle(32.1f + crashMotion2, -26.549950000000003f, 1.05, 9.150050000000002f);
		result |= collideCrashRectangle(47.25f + crashMotion2, -26.0999f, 16.049999999999997f, 1.05);
		result |= collideCrashRectangle(32.1f + crashMotion2, -18.2999f, 13.5f, 1.05);
		result |= collideCrashRectangle(50.1f + crashMotion2, -7.649935000000001f, 1.05, 9.149965f);
		result |= collideCrashRectangle(56.099999999999994f + crashMotion2, -3.29996f, 7.199999999999999f, 1.05);
		result |= collideCrashRectangle(31.5f + crashMotion2, 3.30003f, 13.5f, 1.05);
		result |= collideCrashRectangle(32.1f + crashMotion2, 12.750015f, 1.05, 9.449985f);
		result |= collideCrashRectangle(13.2f + crashMotion2, -7.649985000000001f, 1.05, 8.850014999999999f);
		result |= collideCrashRectangle(7.65f + crashMotion2, -2.9996f, 6.75f, 1.05);
		result |= collideCrashRectangle(38.55f + crashMotion2, 21.9f, 25.35f, 1.05);
		result |= collideCrashRectangle(18.0f + crashMotion2, 26.7f, 1.05, 5.699999999999999f);
		result |= collideCrashRectangle(18.0f + crashMotion2, 39.45f, 1.05, 3.150000000000002f);

		if (isGate2){
			//Gate
			result |= collideCrashRectangle(32.1f + crashMotion2, -40.99995f, 1.05, 6.500050000000002f);
		}
	}
	//cout << "X : " << getCrashPosX() << ", Z : " << getCrashPosZ() << "\n";
	return result;
}

bool bombSeq1 = 1;
bool bombSeq2 = 0;
bool bombSeq3 = 0;
bool bombSeq4 = 0;
bool bombSeq5 = 0;
bool bombSeq6 = 0;
bool bombSeq7 = 0;
bool slimeSeq1 = 1;
bool slimeSeq2 = 0;
bool griver1Seq1 = 1;
bool griver1Seq2 = 0;
bool griver1Seq3 = 0;
bool griver1Seq4 = 0;
bool griver1Seq5 = 0;
bool griver1Seq6 = 0;
bool griver2Seq1 = 1;
bool griver2Seq2 = 0;
bool griver2Seq3 = 0;
bool griver2Seq4 = 0;
bool griver2Seq5 = 0;
bool griver2Seq6 = 0;
bool griver2Seq7 = 0;
bool diablosSeq1 = 1;
bool diablosSeq2 = 0;
bool diablosSeq3 = 0;
bool diablosSeq4 = 0;
bool diablosSeq5 = 0;
bool diablosSeq6 = 0;
bool diablosSeq7 = 0;
bool diablosSeq8 = 0;
bool diablosSeq9 = 0;
bool diablosSeq10 = 0;
bool diablosSeq11 = 0;
bool diablosSeq12 = 0;
bool diablosSeq13 = 0;
bool diablosSeq14 = 0;
bool diablosSeq15 = 0;
float bombMoveConstant = 0.01;
float slimeMoveConstant = 0.01;
float griver1MoveConstant = 0.05;
float griver2MoveConstant = 0.05;
float diablosMoveConstant = 0.05;
void idle() {
	if (isStage1){
		//=====================================================================
		if (bombSeq1){
			moveBombZ += bombMoveConstant;
			bombCollisionsZ += bombMoveConstant;
			if (moveBombZ >= 9.40612){
				bombSeq1 = 0;
				bombSeq2 = 1;
				rotateBomb = -90;
			}
		}
		else{
			if (bombSeq2){
				moveBombX -= bombMoveConstant;
				bombCollisionsX -= bombMoveConstant;
				if (moveBombX <= -10.7302){
					bombSeq2 = 0;
					bombSeq3 = 1;
					rotateBomb = 180;
				}
			}
			else{
				if (bombSeq3){
					moveBombZ -= bombMoveConstant;
					bombCollisionsZ -= bombMoveConstant;
					if (moveBombZ <= 0){
						bombSeq3 = 0;
						bombSeq4 = 1;
						rotateBomb = 0;
					}
				}
				else{
					if (bombSeq4){
						moveBombZ += bombMoveConstant;
						bombCollisionsZ += bombMoveConstant;
						if (moveBombZ >= 9.40612){
							bombSeq4 = 0;
							bombSeq5 = 1;
							rotateBomb = 90;
						}
					}
					else{
						if (bombSeq5){
							moveBombX += bombMoveConstant;
							bombCollisionsX += bombMoveConstant;
							if (moveBombX >= 0){
								bombSeq5 = 0;
								bombSeq6 = 1;
								rotateBomb = 180;
							}
						}
						else{
							if (bombSeq6){
								moveBombZ -= bombMoveConstant;
								bombCollisionsZ -= bombMoveConstant;
								if (moveBombZ <= 0){
									bombSeq6 = 0;
									bombSeq1 = 1;
									rotateBomb = 0;
								}
							}
						}
					}
				}
			}
		}

		//=====================================================================
		if (slimeSeq1){
			moveSlimeZ -= slimeMoveConstant;
			slimeCollisionsZ -= slimeMoveConstant;
			if (moveSlimeZ <= -10.4601){
				slimeSeq1 = 0;
				slimeSeq2 = 1;
				rotateSlime = 180;
			}

		}
		else{
			if (slimeSeq2){
				moveSlimeZ += slimeMoveConstant;
				slimeCollisionsZ += slimeMoveConstant;
				if (moveSlimeZ >= 9.40612){
					slimeSeq2 = 0;
					slimeSeq1 = 1;
					rotateSlime = 0;
				}
			}
		}
	}
	if (isStage2){
		//=============================================================
		//cout << "Griever movement : " << griever1CollisionsX << ", " <<moveGriver1X<<"\n";
		if (griver1Seq1){
			moveGriver1X += griver1MoveConstant;
			griever1CollisionsX += griver1MoveConstant;
			if (moveGriver1X >= 7.90009){
				griver1Seq1 = 0;
				griver1Seq2 = 1;
				rotateGriver1 = 90;
			}
		}
		else{
			if (griver1Seq2){
				moveGriver1Z += griver1MoveConstant;
				griever1CollisionsZ += griver1MoveConstant;
				if (moveGriver1Z >= 9.16011){
					griver1Seq2 = 0;
					griver1Seq3 = 1;
					rotateGriver1 = 0;
				}
			}
			else{
				if (griver1Seq3){
					moveGriver1X -= griver1MoveConstant;
					griever1CollisionsX -= griver1MoveConstant;
					if (moveGriver1X <= -13.6802){
						griver1Seq3 = 0;
						griver1Seq4 = 1;
						rotateGriver1 = -90;
					}
				}
				else{
					if (griver1Seq4){
						moveGriver1Z -= griver1MoveConstant;
						griever1CollisionsZ -= griver1MoveConstant;
						if (moveGriver1Z <= 0){
							griver1Seq4 = 0;
							griver1Seq5 = 1;
							rotateGriver1 = 180;
						}
					}
					else{
						if (griver1Seq5){
							moveGriver1X += griver1MoveConstant;
							griever1CollisionsX += griver1MoveConstant;
							if (moveGriver1X >= 0){
								griver1Seq5 = 0;
								griver1Seq1 = 1;
								rotateGriver1 = 180;
							}
						}
					}
				}
			}
		}
		//=============================================================
		if (griver2Seq1){
			moveGriver2X += griver2MoveConstant;
			griever2CollisionsX += griver2MoveConstant;
			if (moveGriver2X >= 9.49012){
				griver2Seq1 = 0;
				griver2Seq2 = 1;
				rotateGriver2 = 0; // correct
			}
		}
		else{
			if (griver2Seq2){
				moveGriver2Z -= griver2MoveConstant;
				griever2CollisionsZ -= griver2MoveConstant;
				if (moveGriver2Z <= -4.34){
					griver2Seq2 = 0;
					griver2Seq3 = 1;
					rotateGriver2 = 180; // correct
				}
			}
			else{
				if (griver2Seq3){
					moveGriver2Z += griver2MoveConstant;
					griever2CollisionsZ += griver2MoveConstant;
					if (moveGriver2Z >= 9.11011){
						griver2Seq3 = 0;
						griver2Seq4 = 1;
						rotateGriver2 = 90; // correct
					}
				}
				else{
					if (griver2Seq4){
						moveGriver2X -= griver2MoveConstant;
						griever2CollisionsX -= griver2MoveConstant;
						if (moveGriver2X <= -10.7602){
							griver2Seq4 = 0;
							griver2Seq5 = 1;
							rotateGriver2 = -90; // correct
						}
					}
					else{
						if (griver2Seq5){
							moveGriver2X += griver2MoveConstant;
							griever2CollisionsX += griver2MoveConstant;
							if (moveGriver2X >= 9.11011){
								griver2Seq5 = 0;
								griver2Seq6 = 1;
								rotateGriver2 = 0; // this rotation does not make a difference
							}
						}
						if (griver2Seq6){
							moveGriver2Z -= griver2MoveConstant;
							griever2CollisionsZ -= griver2MoveConstant;
							if (moveGriver2Z >= 0){
								griver2Seq6 = 0;
								griver2Seq7 = 1;
								rotateGriver2 = 0; // this rotation does not make a difference
							}
						}
						else{
							if (griver2Seq7){
								moveGriver2X -= griver2MoveConstant;
								griever2CollisionsX -= griver2MoveConstant;
								if (moveGriver2X >= 0){
									griver2Seq7 = 0;
									griver2Seq1 = 1;
									rotateGriver2 = 0; // this rotation does not make a difference
								}
							}
						}
					}
				}
			}
		}
		//=============================================================
		if (diablosSeq1){
			moveDiablosX += diablosMoveConstant;
			diablosCollisionsX += diablosMoveConstant;
			if (moveDiablosX >= 16.2503){
				diablosSeq1 = 0;
				diablosSeq2 = 1;
				rotateDiablos = 180;
			}
		}
		else{
			if (diablosSeq2){
				moveDiablosZ -= diablosMoveConstant;
				diablosCollisionsZ -= diablosMoveConstant;
				if (moveDiablosZ <= -13.3002){
					diablosSeq2 = 0;
					diablosSeq3 = 1;
					rotateDiablos = 90;
				}
			}
			else{
				if (diablosSeq3){
					moveDiablosX += diablosMoveConstant;
					diablosCollisionsX += diablosMoveConstant;
					if (moveDiablosX >= 27.3005){
						diablosSeq3 = 0;
						diablosSeq4 = 1;
						rotateDiablos = 0;
					}
				}
				else{
					if (diablosSeq4){
						moveDiablosZ += diablosMoveConstant;
						diablosCollisionsZ += diablosMoveConstant;
						if (moveDiablosZ >= 1.5){
							diablosSeq4 = 0;
							diablosSeq5 = 1;
							rotateDiablos = 180;
						}
					}
					else{
						if (diablosSeq5){
							moveDiablosZ -= diablosMoveConstant;
							diablosCollisionsZ -= diablosMoveConstant;
							if (moveDiablosZ <= -13.3002){
								diablosSeq5 = 0;
								diablosSeq6 = 1;
								rotateDiablos = -90;
							}
						}
						else{
							if (diablosSeq6){
								moveDiablosX -= diablosMoveConstant;
								diablosCollisionsX -= diablosMoveConstant;
								if (moveDiablosX <= 16.2503){
									diablosSeq6 = 0;
									diablosSeq7 = 1;
									rotateDiablos = 0;
								}
							}
							else{
								if (diablosSeq7){
									moveDiablosZ += diablosMoveConstant;
									diablosCollisionsZ += diablosMoveConstant;
									if (moveDiablosZ >= 5.59003){
										diablosSeq7 = 0;
										diablosSeq8 = 1;
										rotateDiablos = -90;
									}
								}
								else{
									if (diablosSeq8){
										moveDiablosX -= diablosMoveConstant;
										diablosCollisionsX -= diablosMoveConstant;
										if (moveDiablosX <= -13.3002){
											diablosSeq8 = 0;
											diablosSeq9 = 1;
											rotateDiablos = 180;
										}
									}
									else{
										if (diablosSeq9){
											moveDiablosZ -= diablosMoveConstant;
											diablosCollisionsZ -= diablosMoveConstant;
											if (moveDiablosZ <= -13.3002){
												diablosSeq9 = 0;
												diablosSeq10 = 1;
												rotateDiablos = -90;
											}
										}
										else{
											if (diablosSeq10){
												moveDiablosX -= diablosMoveConstant;
												diablosCollisionsX -= diablosMoveConstant;
												if (moveDiablosX <= -21.7604){
													diablosSeq10 = 0;
													diablosSeq11 = 1;
													rotateDiablos = 0; // i stopped here
												}
											}
											else{
												if (diablosSeq11){
													moveDiablosZ += diablosMoveConstant;
													diablosCollisionsZ += diablosMoveConstant;
													if (moveDiablosZ >= 1.5){
														diablosSeq11 = 0;
														diablosSeq12 = 1;
														rotateDiablos = 180;
													}
												}
												else{
													if (diablosSeq12){
														moveDiablosZ -= diablosMoveConstant;
														diablosCollisionsZ -= diablosMoveConstant;
														if (moveDiablosZ <= -13.3002){
															diablosSeq12 = 0;
															diablosSeq13 = 1;
															rotateDiablos = 90;
														}
													}
													else{
														if (diablosSeq13){
															moveDiablosX += diablosMoveConstant;
															diablosCollisionsX += diablosMoveConstant;
															if (moveDiablosX >= -13.3002){
																diablosSeq13 = 0;
																diablosSeq14 = 1;
																rotateDiablos = 0; // here
															}
														}
														else{
															if (diablosSeq14){
																moveDiablosZ += diablosMoveConstant;
																diablosCollisionsZ += diablosMoveConstant;
																if (moveDiablosZ >= 0){
																	diablosSeq14 = 0;
																	diablosSeq15 = 1;
																	rotateDiablos = 90;
																}
															}
															else{
																if (diablosSeq15){
																	moveDiablosX += diablosMoveConstant;
																	diablosCollisionsX += diablosMoveConstant;
																	if (moveDiablosX >= 0){
																		diablosSeq15 = 0;
																		diablosSeq1 = 1;
																		rotateDiablos = 90;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (!isThirdPersonView){
		GetCursorPos(&mousePos);
		//std::cout << "mouse pos . x" << mousePos.x;
		if (rotateCameraFlag)
			rotateCamera = (mousePos.x / 125);
		else
			rotateCamera = (mousePos.x / 125) * -1;
	}
}
void Keyboard(unsigned char key, int x, int y) {
	float d = 0.5;
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
		break;

	case 'i':
		camera.moveY(d);
		break;
	case 'k':
		camera.moveY(-d);
		break;
	case 'j':
		camera.moveX(d);
		break;
	case 'l':
		camera.moveX(-d);
		break;
	case 'u':
		camera.moveZ(d);
		break;
	case 'o':
		camera.moveZ(-d);
		break;

		/////////////////////////////////////////////////////////////
	case 'w':
		if (!colideCrashWithAllObjectsForward()){
			if (isStage1){
				moveCrashZ += crashMotion1;
				crashRotate = 0;
				crashAnimation();
				isCrashMoving = true;
			}
			else{
				if (isStage2){
					moveCrashZ -= crashMotion2;
					crashRotate = 180;
					crashAnimation();
					isCrashMoving = true;
				}
			}
		}
		CollectKey();
		break;
	case 's':
		//cout << collideCrash(35.21, 54.6 - crashMotion, 0.06) << "\n";
		if (!colideCrashWithAllObjectsBackward()){
			if (isStage1){
				moveCrashZ -= crashMotion1;
				crashRotate = 180;
				crashAnimation();
				isCrashMoving = true;
			}
			else{
				if (isStage2){
					moveCrashZ += crashMotion2;
					crashRotate = 0;
					crashAnimation();
					isCrashMoving = true;
				}
			}
		}
		CollectKey();
		break;
	case 'd':
		//cout << collideCrash(35.21 - crashMotion, 54.6, 0.06) << "\n";
		if (!colideCrashWithAllObjectsRight()){
			if (isStage1){
				moveCrashX -= crashMotion1;
				crashRotate = -90;
				crashAnimation();
				isCrashMoving = true;
			}
			else{
				if (isStage2){
					moveCrashX += crashMotion2;
					crashRotate = 90;
					crashAnimation();
					isCrashMoving = true;
				}
			}
		}
		CollectKey();
		break;
	case 'a':
		//cout << collideCrash(35.21 + crashMotion, 54.6, 0.06) << "\n";
		if (!colideCrashWithAllObjectsLeft()){
			if (isStage1){
				moveCrashX += crashMotion1;
				crashRotate = 90;
				crashAnimation();
				isCrashMoving = true;
			}
			else{
				if (isStage2){
					moveCrashX -= crashMotion2;
					crashRotate = 270;
					crashAnimation();
					isCrashMoving = true;
				}
			}
		}
		CollectKey();
		break;

	case 'v':
		isThirdPersonView = !isThirdPersonView;
		rotateCamera = 0;
		break;

	case 'f':
		freeRoam = 1;
		isTopView = 0;
		isThirdPersonView = 0;
		break;
	case 'g':
		isThirdPersonView = 1;
		isTopView = 0;
		freeRoam = 0;
		break;
	case 't':
		isTopView = 1;
		isThirdPersonView = 0;
		freeRoam = 0;
		break;

		/////////////////////////////////////////////////////////////

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
bool startHero = 1;
void Display(void) {
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (isStage1)
		setupLight0();
	else if (isStage2)
		setupLight1();

	if (LightRotate >= 1)
		LightRotate = 0;
	else
		LightRotate += 0.001;

	if (isStage1 || (isStage2 && (!isKeysCollected()))){
		if (isCrashDead()){
			moveCrashX = 0;
			moveCrashZ = 0;
			if (isStage1){
				isKey1 = 1;
				isKey2 = 1;
				isGate1 = 1;
			}
			else{
				if (isStage2){
					isKey3 = 1;
					isKey4 = 1;
					isKey5 = 1;
					isKey6 = 1;
					isGate2 = 1;
				}
			}
		}
	}
	else{
		isEnemyDead();
		if (startHero){
			PlaySound(TEXT("Struggle.wav"), NULL, SND_ASYNC | SND_LOOP);
			startHero = 0;
		}
	}

	if (stageEnd()){
		if (isStage1){
			isStage1 = 0;
			isStage2 = 1;
			initializeCamera = 1;
		}
		else{
			if (isStage2){
				isStage2 = 0;
				isStage1 = 1;
				initializeCamera = 1;
			}
		}
		moveCrashX = 0;
		moveCrashZ = 0;
	}
	if (isStage1){
		if (isKeysCollected()){
			isGate1 = 0;
		}
	}
	if (isStage2){
		if (!isDiablos && !isGriver1 && !isGriver2){
			isGate2 = 0;
		}
	}
	//Draw Level 1
	if (isStage1){
		if (isCrash){
			glPushMatrix();
			glTranslatef(moveCrashX, moveCrashY, moveCrashZ);
			glTranslatef(19, -0.15, -10);
			glScaled(18, 18, 18);

			//glRotatef(90.0, 1, 0, 0);
			glRotatef(crashRotate, 0, 1, 0);
			//glRotatef(205.f, 0, 0, 1);
			glScaled(0.001, 0.001, 0.001);
			glScaled(crashIdleAnim, crashIdleAnim, crashIdleAnim);
			glPushMatrix();
			//glRotated(crashAnim, 0, 1, 0); // Animate the body? Yes or No? y = 1 looks nice and x = 1 looks nice too
			crash.Draw();
			glPopMatrix();
			glPushMatrix();
			glRotated(crashAnim, 0, 1, 0); // Animate the right arm
			crash_right_arm.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(crashAnim, 0, 1, 0);
			crash_left_arm.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(crashAnim, 1, 0, 0);
			glTranslated(0, 0.1, 0);
			crash_right_leg.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(crashAnim, -1, 0, 0);
			glTranslated(0, 0.1, 0);
			crash_left_leg.Draw();
			glPopMatrix();

			glPopMatrix();
		}

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

		if (isGate1){
			//gate
			glPushMatrix();
			glTranslatef(18, 0, 30);
			glScaled(0.03, 0.03, 0.03);
			glRotatef(90.0, 1, 0, 0);
			gate.Draw();
			glPopMatrix();
		}
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
		glTranslatef(moveBombX, 0, moveBombZ);
		glTranslatef(30, 0, 18);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(90.0, 0, 1, 0);
		glRotatef(260.f, 0, 0, 1);
		glRotated(rotateBomb, 0, 0, 1);
		glScaled(0.0003, 0.0003, 0.0003);
		bomb.Draw();
		glPopMatrix();

		//slime
		glPushMatrix();
		glTranslatef(moveSlimeX, 0, moveSlimeZ);
		glTranslatef(6, 0, 18);
		glRotatef(90.0, 0, 0, 1);
		glRotatef(90.0, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		glScaled(0.0003, 0.0003, 0.0003);
		glRotated(rotateSlime, 0, 0, 1);
		slime.Draw();
		glPopMatrix();

		if (isKey1){
			//collectable key1
			glPushMatrix();
			glTranslatef(30, -1, 10);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			glRotated(rotateKeys, 0, 0, 1);
			key.Draw();
			glPopMatrix();
		}

		if (isKey2){
			//collectable key2
			glPushMatrix();
			glTranslatef(6, -1, 25);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			glRotated(rotateKeys, 0, 0, 1);
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

		if (isCrash){
			glPushMatrix();
			glTranslatef(moveCrashX, moveCrashY, moveCrashZ);
			glTranslatef(55, -0.15, 45);
			glScaled(18, 18, 18);


			//glRotatef(90.0, 1, 0, 0);
			glRotatef(crashRotate, 0, 1, 0);
			//glRotatef(205.f, 0, 0, 1);
			glScaled(0.001, 0.001, 0.001);
			glScaled(crashIdleAnim, crashIdleAnim, crashIdleAnim);
			glPushMatrix();
			//glRotated(crashAnim, 0, 1, 0); // Animate the body? Yes or No? y = 1 looks nice and x = 1 looks nice too
			crash.Draw();
			glPopMatrix();
			glPushMatrix();
			glRotated(crashAnim, 0, 1, 0); // Animate the right arm
			crash_right_arm.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(crashAnim, 0, 1, 0);
			crash_left_arm.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(crashAnim, 1, 0, 0);
			glTranslated(0, 0.1, 0);
			crash_right_leg.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(crashAnim, -1, 0, 0);
			glTranslated(0, 0.1, 0);
			crash_left_leg.Draw();
			glPopMatrix();

			glPopMatrix();
		}

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

		if (isGate2){
			//gate
			glPushMatrix();
			glTranslatef(52.2, 0, -31.5);
			glScaled(0.03, 0.03, 0.03);
			glRotatef(90.0, 1, 0, 0);
			glRotatef(-90.0, 0, 0, 1);
			gate.Draw();
			glPopMatrix();
		}
		if (isGriver1){
			//griver
			glPushMatrix();
			glTranslatef(moveGriver1X, 0, moveGriver1Z);
			glTranslatef(70, 0, 20);
			glRotatef(90.0, 0, 0, 1);
			glRotatef(90.0, 0, 1, 0);
			glRotatef(180.f, 0, 0, 1);
			glRotated(rotateGriver1, 0, 0, 1);
			glScaled(3, 3, 3);
			griver.Draw();
			glPushMatrix();
			glRotated(griverAnim, 1, 1, 1);
			griver_left_wing.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(griverAnim, 0, 0, 1);
			griver_right_wing.Draw();
			glPopMatrix();
			glPopMatrix();
		}
		if (isGriver2){
			//griver
			glPushMatrix();
			glTranslatef(moveGriver2X, 0, moveGriver2Z);
			glTranslatef(40, 0, 20);
			glRotatef(90.0, 0, 0, 1);
			glRotatef(90.0, 0, 1, 0);
			glRotatef(90.0f, 0, 0, 1);
			glRotated(rotateGriver2, 0, 0, 1);
			glScaled(3, 3, 3);
			griver.Draw();
			glPushMatrix();
			glRotated(griverAnim, 1, 1, 1);
			griver_left_wing.Draw();
			glPopMatrix();
			glPushMatrix();
			glRotated(griverAnim, 0, 0, 1);
			griver_right_wing.Draw();
			glPopMatrix();
			glPopMatrix();
		}
		if (isDiablos){
			//diablos
			glPushMatrix();
			glTranslatef(moveDiablosX, 0, moveDiablosZ);
			glTranslatef(50, 0, 3);
			glRotatef(90.0, 0, 0, 1);
			glRotatef(90.0, 0, 1, 0);
			glRotatef(260.f, 0, 0, 1);
			glRotated(rotateDiablos, 0, 0, 1);
			glScaled(3, 3, 3);
			diablos.Draw();

			glPushMatrix();
			glRotated(griverAnim, 1, 1, 1);
			diablos_left_wing.Draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(griverAnim, 1, 1, 1);
			diablos_right_wing.Draw();
			glPopMatrix();

			glPopMatrix();
		}

		if (isKey3){
			//collectable Key3
			glPushMatrix();
			glTranslatef(75, -1, 36);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			glRotated(rotateKeys, 0, 0, 1);
			key.Draw();
			glPopMatrix();
		}

		if (isKey4){
			//collectable Key4
			glPushMatrix();
			glTranslatef(55, -1, 26);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			glRotated(rotateKeys, 0, 0, 1);
			key.Draw();
			glPopMatrix();
		}

		if (isKey5){
			//collectable key5
			glPushMatrix();
			glTranslatef(28, -1, 3);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			glRotated(rotateKeys, 0, 0, 1);
			key.Draw();
			glPopMatrix();
		}

		if (isKey6){
			//collectable key6
			glPushMatrix();
			glTranslatef(76, -1, 3);
			glScaled(0.025, 0.025, 0.025);
			glRotatef(90.0, 1, 0, 0);
			glRotated(rotateKeys, 0, 0, 1);
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
//=======================================================================

void setupCamera() {

	if (initializeCamera){
		if (isStage2){

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
		else{
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
		}

		initializeCamera = 0;
	}
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
	griver.Load("Models/FFGr/griever_body.3ds");
	griver_left_wing.Load("Models/FFGr/griever_left_wing.3ds");
	griver_right_wing.Load("Models/FFGr/griever_right_wing.3ds");
	diablos.Load("Models/FFDi/diabolos_body.3ds");
	diablos_left_wing.Load("Models/FFDi/diabolos_left_wing.3ds");
	diablos_right_wing.Load("Models/FFDi/diabolos_right_wing.3ds");
	wall.Load("Models/wallStone/stoneWall.3DS");
	grass.Load("Models/Grass/Grass/Grass.3ds");
	station.Load("Models/station/Computer Panel/main_panel_ma01.3ds");
	gate.Load("Models/gate/Gate 2/Gate_01.3DS");
	key.Load("Models/collect/KingdomKey/KingdomKey.3ds");
	crash.Load("Models/crash/body.3ds");
	crash_right_arm.Load("Models/crash/rightarm.3ds");
	crash_left_arm.Load("Models/crash/leftarm.3ds");
	crash_right_leg.Load("Models/crash/rightleg.3ds");
	crash_left_leg.Load("Models/crash/leftleg.3ds");

	loadBMP(&tex, "Textures/sky4-jpg.bmp", true);
}


void cameraAnim(int value) {

	if (cameraRotate) {
		centerX = 1.0;
		camera.rotateY(centerX);
	}

	glutPostRedisplay();
	glutTimerFunc(1, cameraAnim, 0);

}
void thirdPersonView(){

	// Third Person View

	if (!colideCrashWithAllObjectsRight())
		camera.center.x = -16.9406 + getCrashPosX(); // No Collisions on the right side of crash

	else if (colideCrashWithAllObjectsRight())
		camera.center.x = -16 + getCrashPosX(); // To avoid the camera going through the object, I decremented the value

	camera.center.y = 2.57471;
	camera.center.z = -51.1186 + getCrashPosZ();

	if (!colideCrashWithAllObjectsRight())
		camera.eye.x = -17.0132 + getCrashPosX(); // No Collisions on the right side of crash

	else if (colideCrashWithAllObjectsRight())
		camera.eye.x = -16.15 + getCrashPosX(); // To avoid the camera going through the object, I decremented the value

	camera.eye.y = 2.56383;
	camera.eye.z = -52.1159 + getCrashPosZ();
	camera.up.x = -0.00104444;
	camera.up.y = 0.999941;
	camera.up.z = -0.0108271;
}

void firstPersonView()
{
	if (isStage1){
		// First Person View
		camera.center.x = -16.0318 + getCrashPosX() + rotateCamera;
		//camera.center.y = 2.45376;
		camera.center.z = -42.77647 + getCrashPosZ();
		camera.eye.x = -16.0696 + getCrashPosX();
		//camera.eye.y = 2.44289;
		camera.eye.z = -43.7757 + getCrashPosZ();
		camera.up.x = -0.00104502;
		//camera.up.y = 0.999941;
		camera.up.z = -0.0108352;
	}

	else{
		// First Person View
		camera.center.x = 19.4544 + getCrashPosX() + rotateCamera;
		//camera.center.y = 2.45376;
		camera.center.z = 7.5696 + getCrashPosZ();
		camera.eye.x = 19.5271 + getCrashPosX();
		//camera.eye.y = 2.44289;
		camera.eye.z = 8.5669 + getCrashPosZ();
		camera.up.x = -0.00103698;
		//camera.up.y = 0.999941;
		camera.up.z = -0.0108241;
	}

}

void topView(){
	camera.center.x = 18.6111 + getCrashPosX();
	camera.center.y = 34.9876;
	camera.center.z = -8.00771 + getCrashPosZ();
	camera.eye.x = 18.6097 + getCrashPosX();
	camera.eye.y = 35.9874;
	camera.eye.z = -7.98469 + getCrashPosZ();
	camera.up.x = 0.0510274;
	camera.up.y = -0.0229137;
	camera.up.z = 0.998434;
}

void timer(int value){
	if (!freeRoam){
		if (isTopView){
			topView();
		}
		else{
			if (isThirdPersonView){
				thirdPersonView();
			}
			else{
				firstPersonView();
			}
		}
	}

	if (!isCrashMoving){
		crashAnim = 0;
		if (crashIdleAnim > 1.005)
			crashAnimConstant *= -1;
		else if (crashIdleAnim < 1)
			crashAnimConstant *= -1;

		crashIdleAnim += (crashAnimConstant * 0.00005);
	}

	if (isStage1){
		if (!freeRoam){
			if (isThirdPersonView){
				if (!colideCrashWithAllObjectsRight())
					camera.center.x = -16.9406 + getCrashPosX(); // No Collisions on the right side of crash

				else if (colideCrashWithAllObjectsRight())
					camera.center.x = -16 + getCrashPosX(); // To avoid the camera going through the object, I decremented the value

				camera.center.y = 2.57471;

				if (!colideCrashWithAllObjectsBackward())
					camera.center.z = -51.1186 + getCrashPosZ();

				else
					camera.center.z = -47.1186 + getCrashPosZ() + 6;

				if (!colideCrashWithAllObjectsRight())
					camera.eye.x = -17.0132 + getCrashPosX(); // No Collisions on the right side of crash

				else if (colideCrashWithAllObjectsRight())
					camera.eye.x = -16.15 + getCrashPosX(); // To avoid the camera going through the object, I decremented the value

				camera.eye.y = 2.56383;

				if (!colideCrashWithAllObjectsBackward())
					camera.eye.z = -52.1159 + getCrashPosZ();
				else
					camera.eye.z = -52.1159 + getCrashPosZ() + 6;

				camera.up.x = -0.00104444;
				camera.up.y = 0.999941;
				camera.up.z = -0.0108271;
			}
			if (isTopView){
				camera.center.x = 19.6343 + getCrashPosX() - 35 - 0.1;
				camera.center.y = 24.8104;
				camera.center.z = -5.45305 + getCrashPosZ() - 35 - 0.1;
				camera.eye.x = 19.6329 + getCrashPosX() - 35 - 0.1;
				camera.eye.y = 25.8101;
				camera.eye.z = -5.43003 + getCrashPosZ() - 35 - 0.1;
				camera.up.x = 0.0510274;
				camera.up.y = -0.0229137;
				camera.up.z = 0.998434;
			}


		}
	}

	if (isStage2){
		if (!freeRoam){
			if (isThirdPersonView){
				camera.center.x = 55.1269 + getCrashPosX() - 35 - 0.1;
				camera.center.y = 3.17513;
				camera.center.z = 50.2287 + getCrashPosZ() - 35 - 0.1;
				camera.eye.x = 55.1983 + getCrashPosX() - 35 - 0.1;
				camera.eye.y = 3.35948;
				camera.eye.z = 51.209 + getCrashPosZ() - 35 - 0.1;
				camera.up.x = -0.013642;
				camera.up.y = 0.982862;
				camera.up.z = -0.183838;
			}
			if (isTopView){
				camera.center.x = 55.0848 + getCrashPosX() - 35 - 0.1;
				camera.center.y = 23.2224;
				camera.center.z = 48.7982 + getCrashPosZ() - 35 - 0.1;
				camera.eye.x = 55.0734 + getCrashPosX() - 35 - 0.1;
				camera.eye.y = 24.1919;
				camera.eye.z = 49.0428 + getCrashPosZ() - 35 - 0.1;
				camera.up.x = 0.00368193;
				camera.up.y = 0.244656;
				camera.up.z = -0.969603;
			}
		}
	}
	rotateKeys++;
	griverAnimation();

	glutTimerFunc(5, timer, value);
	glutPostRedisplay();
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	PlaySound(TEXT("Destati.wav"), NULL, SND_ASYNC | SND_LOOP);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 100);

	glutCreateWindow(title);

	glutDisplayFunc(Display);

	glutKeyboardFunc(Keyboard);
	glutMouseFunc(mouseClicks);
	glutSpecialFunc(Special);

	glutIdleFunc(idle);
	glutTimerFunc(0, timer, 0);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutTimerFunc(0, cameraAnim, 0);

	glutMainLoop();
}