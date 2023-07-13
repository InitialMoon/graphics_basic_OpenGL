#include "miniEngine.h"
#include "CMatrix081.h"
#include "CVector081.h"
#include "CEuler081.h"
#include "CQuaternion081.h"
#include <atlimage.h>
#include "glew.h"
#include "glut.h"
#include "CModel.h"
#include "camera.h" // 这个要是写在前面的话就不行的，非得写在这个位置才能编译过
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "string.h"
#include <vector>

using namespace std;

//常量区
float unit_M[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

//2D字内容
char* status_contnet;

// 相机
// 记录当前相机是几号相机，0-4
int camera_number = 0;
int next_camera_number = (camera_number + 1) % 5; // 记录下一个相机编号
bool can_switch_camera = true; // 能否调整视点，也就是是否稳定了
vector<camera> cameras;
camera* cur_camera; // 这个是我们当前使用的那个相机，可能被多个相机替代,直接声明一个指针方便
camera inter_camera; // 插值用的相机

// 控制量
double theta = 0.0; // 星球旋转的角度

int view_mode = 0; // 视点位置，当到达上帝视角的终点时值为0，在火箭视点上时为1

bool recieve_key = true; // 当这个值为真时是平滑变化完毕的时候，当为假的时候，就代表在变化的路径上，不接受任何键盘输入，同时会保证所有的其他值都为false
bool planets_rotate = false;
bool need_fly = true; // 记录火箭是否还需要继续飞，因为飞过头会直接扎进月亮里，还会来回抖动
bool have_started = false; // 记录火箭的起飞过，防止追踪月亮的过程中高度低于起飞阈值导致重新起飞了
bool rocket_start = false; 

// 所有模型的声明

// 星球
CModel sun = CModel();
CModel mercury = CModel();
CModel venus = CModel();
CModel earth = CModel();
CModel moon = CModel();
CModel space_station = CModel();
CModel person = CModel();
CModel mars = CModel();
CModel jupiter = CModel();
CModel saturn = CModel();
CModel uranus = CModel();
CModel neptune = CModel();
// 空间站
CModel station = CModel();
// 火箭
CModel rocket = CModel(); // 火箭头
CModel rocket1 = CModel(); // 火箭底座
//天空盒
CModel sky = CModel();
//人
CModel preson = CModel();


bool rocket1_draw = true;
bool updateAll = true;
float rocket_seta = 0; // 记录火箭旋转的角度，是指变方向转的时候

// 是否接受回车键，也就是在动作发生期间设为false，以免打乱动作进行
bool recieve_enter = true;

map<char, bool> keyMap = { //map如果用const修饰，那么就没法用[]调用了
	{'w', false},
	{'a', false},
	{'s', false},
	{'d', false},
	{'q', false},
	{'e', false},
	{'j', false},
	{'l', false},
	{'i', false},
	{'k', false},
	{'u', false},
	{'o', false},
	{'[', false},
	{']', false},
	{'{', false},
	{'}', false},
	{'1', false},
	{'2', false},
	{'3', false},
	{' ', false}
};

void myDisplay(void);
void myRotate(float seta, float x, float y, float z) { // 自己实现的旋转函数
	CMatrix081 mymatrix;
	mymatrix.SetRotate(seta, CVector081(x, y, z));
	glMultMatrixf(mymatrix);
}
void myTranslatef(float x, float y, float z) { // 自己实现的平移函数
	CVector081 transVec = CVector081(x, y, z);
	CMatrix081 transMat;
	transMat.SetTrans(transVec);
	glMultMatrixf(transMat);
};
void myScalef(float x, float y, float z) {
	CVector081 ScaleVec = CVector081(x, y, z);
	CMatrix081 ScaleMat;
	ScaleMat.SetScale(ScaleVec);
	glMultMatrixf(ScaleMat);
}

void responseAllKey() {
	for (auto& it : keyMap) {
		if (it.second) {
			switch (it.first) {
			case 'w':
				cur_camera->move_w();
				break;
			case 's':
				cur_camera->move_s();
				break;
			case 'a':
				cur_camera->move_a();
				break;
			case 'd':
				cur_camera->move_d();
				break;
			case 'q':
				cur_camera->move_q();
				break;
			case 'e':
				cur_camera->move_e();
				break;
			case 'i':
				cur_camera->rotate_i();
				break;
			case 'k':
				cur_camera->rotate_k();
				break;
			case 'j':
				cur_camera->rotate_j();
				break;
			case 'l':
				cur_camera->rotate_l();
				break;
			case 'u':
				cur_camera->rotate_u();
				break;
			case 'o':
				cur_camera->rotate_o();
				break;
			case '{':
				cur_camera->rspeed += 0.2;
				break;
			case '}':
				if (cur_camera->rspeed > 0.2) { // 防止出现减过头反向加速的情况
					cur_camera->rspeed -= 0.2;
				}
				break;
			case '[':
				cur_camera->mspeed += 0.2;
				break;
			case ']':
				if (cur_camera->mspeed > 0.2) { // 防止出现减过头反向加速的情况
					cur_camera->mspeed -= 0.2;
				}
				break;
			}
		}
	}
}

void updateRocket() {
	//TODO
	//cout << rocket1.getAbsPos().x << ",";
	//cout << rocket1.getAbsPos().y << ",";
	//cout << rocket1.getAbsPos().z << endl;
}

void updatePlanets() {
	theta += 0.1;
	if ((theta / (22.5 * 8.8 * 36.5 * 67.8 * 3 * 433.3 * 37.8 * 16.5 * 3079.9)) > 360) {
		theta -= (22.5 * 8.8 * 36.5 * 67.8 * 3 * 433.3 * 37.8 * 16.5 * 3079.9) * 360;
	}
	venus.pos.x = cos(theta / 22.5) * venus.R;
	venus.pos.z = sin(theta / 22.5) * venus.R;
	mercury.pos.x = cos(theta / 8.8) * mercury.R;
	mercury.pos.z = sin(theta / 8.8) * mercury.R;
	earth.pos.x = cos(theta / 36.5) * earth.R;
	earth.pos.z = sin(theta / 36.5) * earth.R;
	mars.pos.x = cos(theta / 68.7) * mars.R;
	mars.pos.z = sin(theta / 68.7) * mars.R;
	moon.pos.x = cos(theta / 3) * moon.R;
	moon.pos.z = sin(theta / 3) * moon.R;
	station.pos.x = cos(theta / 2) * station.R;
	station.pos.z = sin(theta / 2) * station.R;
	person.pos.y = cos(theta / 20) * (moon.size.x + person.size.y);
	person.pos.z = sin(theta / 20) * (moon.size.x + person.size.y);
	jupiter.pos.x = cos(theta / 433.3) * jupiter.R;
	jupiter.pos.z = sin(theta / 433.3) * jupiter.R;
	saturn.pos.x = cos(theta / 37.8) * saturn.R;
	saturn.pos.z = sin(theta / 37.8) * saturn.R;
	neptune.pos.x = cos(theta / 16.5) * neptune.R;
	neptune.pos.z = sin(theta / 16.5) * neptune.R;
	uranus.pos.x = cos(theta / 3079.9) * uranus.R;
	uranus.pos.z = sin(theta / 3079.9) * uranus.R;
}

void updateView() { // 控制视点插值过程的的属性值变动模块，解放键位操作
	if (!recieve_key) {
		if (cur_camera->auto_move() == 0) {
			//for (int i = 0; i < 3; i++) {
			//	cout << cur_camera->pos[i] << ",";
			//}
			//cout << endl;
			updateAll = false;
			return;
		}
		else {
			recieve_key = true;
			updateAll = true;
			can_switch_camera = true;
			cur_camera = &cameras[next_camera_number];
			camera_number = next_camera_number;
			next_camera_number = (camera_number + 1) % 5;
		}
	}
}

void update() {
	if (updateAll)
		updateRocket();
	if (planets_rotate) {
		if (updateAll)
			updatePlanets();
	}
	if (!recieve_key) {
		updateView();
	}
}

void myTimerFunc(int val)
{
	responseAllKey();
	update();
	myDisplay();
	glutTimerFunc(30, myTimerFunc, 0);
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	if (recieve_key) {
		switch (key)
	{
	case 'w':
	case 'W':
		keyMap['w'] = true;
		break;
	case 's':
	case 'S':
		keyMap['s'] = true;
		break;
	case 'a':
	case 'A':
		keyMap['a'] = true;
		break;
	case 'd':
	case 'D':
		keyMap['d'] = true;
		break;
	case 'q':
	case 'Q':
		keyMap['q'] = true;
		break;
	case 'e':
	case 'E':
		keyMap['e'] = true;
		break;
	case 'j':
	case 'J':
		keyMap['j'] = true;
		break;
	case 'k':
	case 'K':
		keyMap['k'] = true;
		break;
	case 'i':
	case 'I':
		keyMap['i'] = true;
		break;
	case 'l':
	case 'L':
		keyMap['l'] = true;
		break;
	case 'u':
	case 'U':
		keyMap['u'] = true;
		break;
	case 'o':
	case 'O':
		keyMap['o'] = true;
		break;
	case '{':
		keyMap['{'] = true;
		break;
	case '}':
		keyMap['}'] = true;
		break;
	case '[':
		keyMap['['] = true;
		break;
	case ']':
		keyMap[']'] = true;
		break;
	}
	}
}

void myKeyboardUpFunc(unsigned char key, int x, int y)
{
	if (recieve_key)
	switch (key)
	{
	case 'w':
	case 'W':
		keyMap['w'] = false;
		break;
	case 's':
	case 'S':
		keyMap['s'] = false;
		break;
	case 'a':
	case 'A':
		keyMap['a'] = false;
		break;
	case 'd':
	case 'D':
		keyMap['d'] = false;
		break;
	case 'q':
	case 'Q':
		keyMap['q'] = false;
		break;
	case 'e':
	case 'E':
		keyMap['e'] = false;
		break;
	case 'j':
	case 'J':
		keyMap['j'] = false;
		break;
	case 'k':
	case 'K':
		keyMap['k'] = false;
		break;
	case 'i':
	case 'I':
		keyMap['i'] = false;
		break;
	case 'l':
	case 'L':
		keyMap['l'] = false;
		break;
	case 'u':
	case 'U':
		keyMap['u'] = false;
		break;
	case 'o':
	case 'O':
		keyMap['o'] = false;
		break;
	case '{':
		keyMap['{'] = false;
		break;
	case '}':
		keyMap['}'] = false;
		break;
	case '[':
		keyMap['['] = false;
		break;
	case ']':
		keyMap[']'] = false;
		break;
	case '1':
		cur_camera->mode = 1;
		break;
	case '2':
		cur_camera->mode = 0;
		break;
	case '3':
	if (can_switch_camera) {
		cameras[next_camera_number].mode = cur_camera->mode;
		can_switch_camera = false;
		recieve_key = false;

		// 让当前的相机指向的不再是现成的相机，而是中间专门用来插值的相机，不是无意义的，虽然值一样，但不是同一个对象了
		inter_camera = *cur_camera;
		cur_camera = &inter_camera;

		for (auto& it : keyMap) { // 取消所有键盘的效果
			it.second = false;
		}
		CQuaternion081 q_Eye = cur_camera->mlookAt.GetInverse().ToQuaternion();
		CQuaternion081 result_dir = cameras[next_camera_number].mlookAt.GetInverse().ToQuaternion();

		CVector081 start_pos =cur_camera->getAbsPos();
		CVector081 end_pos = cameras[next_camera_number].getAbsPos();

		// 根据角度差确定插值数量
		cur_camera->view_num = int (abs(cur_camera->elookAt.h - cameras[next_camera_number].elookAt.h) +
			abs(cur_camera->elookAt.p - cameras[next_camera_number].elookAt.p) +
			abs(cur_camera->elookAt.b - cameras[next_camera_number].elookAt.b)) / 10;
		// 根据距离差确定插值数量
		cur_camera->path_num = int((start_pos - end_pos).len());
		cur_camera->view = (CQuaternion081*)malloc(sizeof(CQuaternion081) * cur_camera->view_num);
		cur_camera->path = (CVector081*)malloc(sizeof(CVector081) * cur_camera->path_num);
		float* ts0 = (float*)malloc(sizeof(float) * cur_camera->path_num);
		float* ts1 = (float*)malloc(sizeof(float) * cur_camera->view_num);
		for (int i = 0; i < cur_camera->path_num; i++) {
			ts0[i] = (i + 1) * 1.0 / cur_camera->path_num;
		}
		for (int i = 0; i < cur_camera->view_num; i++) {
			ts1[i] = (i + 1) * 1.0 / cur_camera->view_num;
		}
		start_pos.Slerp(end_pos, cur_camera->path_num, ts0, cur_camera->path);
		q_Eye.Slerp(result_dir, cur_camera->view_num, ts1, cur_camera->view);
	}
	break;
	case ' ':
		planets_rotate = !planets_rotate;
		break;
	case '\r':
		if (recieve_enter) {
			recieve_enter = false;
			rocket_start = true;
		}
		break;
	}
}

void initLight() {
	// 光照设置
	glClearColor(0, 0, 0, 1);
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float amb[4] = { 0.4,0.4,0.4,1.0 };					//红色环境光
	float dif[4] = { 1.0,1.0,1.0,1 };					//白色漫射光和反射光
	float spe[4] = { 1.0,1.0,1.0,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void SetRC()
{
	initLight();
	// 非光照设置
	glShadeModel(GL_FLAT);
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	// 给需要贴图的模型加载贴图,不能只加载一次好像，我也不知道为啥，反正只能放这里才能生效
	moon.LoadGLTextures("Inkedmoon.jpg");
	sun.LoadGLTextures("Inkedsun.jpg");
	sky.LoadGLTextures("sky.jpg");
	earth.LoadGLTextures("Inkedearth.jpg");
	station.LoadGLTextures("station.jpg");
	person.LoadGLTextures("person.jpg");
}

// 统一版本
void SetView()
{
	glLoadMatrixf(cur_camera->mlookAt);
	if (!recieve_key) {
		glTranslatef(-cur_camera->pos.x, -cur_camera->pos.y, -cur_camera->pos.z);
	}
	else { // 统一使用父矩阵的方式来讲相机的初始位置和后面的累积变化量分开来，统一了形式
		CVector081 v = cur_camera->getAbsPos();
		glTranslatef(-v.x, -v.y, -v.z);
	}
}

void Font2D(char* str, double x, double y, int size)
{
	glColor3f(1, 1, 1);
	//设置投影方式：平行投影
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//输出字符串
	int len = strlen(str);
	glRasterPos2f(x, y);
	for (int i = 0; i < len; ++i)
	{
		switch (size) {
		case 1:
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
			break;
		case 2:
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
			break;
		case 3:
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
			break;
		case 4:
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			break;
		case 5:
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
			break;
		case 6:
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
			break;
		case 7:
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			break;
		}
	}
	//恢复投影方式
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void RenderWorld() {
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	sky.Draw(2);
	sun.Draw(1);
	float pos[4] = {0,0,0,1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	station.Draw(2);
	mercury.Draw(0);
	venus.Draw(0);
	earth.Draw(1);
	moon.Draw(1);
	mars.Draw(0);
	jupiter.Draw(0);
	saturn.Draw(0);
	uranus.Draw(0);
	neptune.Draw(0);
	person.Draw(2);
	rocket.Draw(3);
	rocket1.Draw(4);

	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	Font2D(status_contnet, -0.9, 0.95, 7);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	SetView();
	RenderWorld();
	//模型控制
	glPopMatrix();
	glutSwapBuffers();
}

void myReshape(int w, int h)
{
	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initCamera() {
	float INIT_CAMERAPOS_X = 155, INIT_CAMERAPOS_Y = 10, INIT_CAMERAPOS_Z = 50;
	camera god_camera = camera(); // 上帝视角相机
	camera earth_camera = camera(); // 地球视角
	camera rocket_camera = camera(); // 火箭上绑定的相机
	camera space_station_camera = camera(); // 空间站视点
	camera person_camera = camera(); // 太空人视点

	// 视点和对应模型绑定
	god_camera.parentModel = new CModel();
	earth_camera.parentModel = &earth;
	rocket_camera.parentModel = &rocket1;
	space_station_camera.parentModel = &station;
	person_camera.parentModel = &person;

	// 相机位置，视角初始化
	CVector081 cameraPos = CVector081(INIT_CAMERAPOS_X, INIT_CAMERAPOS_Y, INIT_CAMERAPOS_Z);
	CEuler081 direct = CEuler081();

	god_camera.pos = cameraPos;
	god_camera.elookAt = direct;
	god_camera.mlookAt = direct.ToMatrix().GetInverse();

	// 这里的偏移量不能放在父矩阵中，要放在相机的位移量中，与移动的累计量合并
	// 否则在父矩阵自己在变化的时候就刷新掉了，父矩阵是我们不能控制的，只能绑定
	earth_camera.pos.y = earth.size.x;
	direct.h = 90;
	direct.p = 0;
	earth_camera.elookAt = direct;
	earth_camera.mlookAt = direct.ToMatrix().GetInverse();

	rocket_camera.pos.x = - 3;
	direct.h = -90;
	direct.p = 0;
	rocket_camera.elookAt = direct;
	rocket_camera.mlookAt = direct.ToMatrix().GetInverse();

	space_station_camera.pos.y = station.size.y;
	direct.h = -90;
	direct.p = 30;
	space_station_camera.elookAt = direct;
	space_station_camera.mlookAt = direct.ToMatrix().GetInverse();

	person_camera.pos.y = person.pos.y;
	direct.h = 90;
	direct.p = 10;
	person_camera.elookAt = direct;
	person_camera.mlookAt = direct.ToMatrix().GetInverse();

	cameras.push_back(god_camera);
	cameras.push_back(earth_camera);
	cameras.push_back(rocket_camera);
	cameras.push_back(space_station_camera);
	cameras.push_back(person_camera);

	// 初始相机指定
	cur_camera = &cameras[0];
}

void initModelData() {

	// 行星绕行半径
	mercury.R = 110;
	venus.R = 130;
	earth.R = 160;
	moon.R = 25;
	station.R = 15;
	mars.R = 210;
	jupiter.R = 240;
	saturn.R = 320;
	uranus.R = 390;
	neptune.R = 420;

	//模型大小设置
	sun.setSize(100);
	mercury.setSize(3);
	venus.setSize(5);
	earth.setSize(10);
	moon.setSize(3);
	station.setSize(3);
	mars.setSize(6);
	jupiter.setSize(20);
	saturn.setSize(30);
	uranus.setSize(9);
	neptune.setSize(8);
	person.setSize(0.03, 0.1, 0.03);
	sky.setSize(500);
	rocket.setSize(2, 1.5, 2);
	rocket1.setSize(2, 1.5, 2);

	// 不贴图模型颜色的设置
	mercury.setColor(220, 220, 220);
	venus.setColor(172, 102, 17);
	mars.setColor(173, 133, 92);
	jupiter.setColor(151, 123, 99);
	saturn.setColor(181, 154, 126);
	uranus.setColor(137, 157, 168);
	neptune.setColor(99, 148, 232);

	// 父矩阵设置
	sky.parentModel = new CModel();
	sun.parentModel = new CModel();
	mercury.parentModel = &sun;
	venus.parentModel = &sun;
	earth.parentModel = &sun;
	moon.parentModel = &earth;
	station.parentModel = &earth;
	mars.parentModel = &sun;
	jupiter.parentModel = &sun;
	saturn.parentModel = &sun;
	uranus.parentModel = &sun;
	neptune.parentModel = &sun;
	person.parentModel = &moon;
	rocket.parentModel = &earth;
	rocket1.parentModel = &earth;
}

void initModelPos() {
	// 由于我们使用了父子坐标的方式，而且父坐标使用绑定的方式实现的，
	// 绝对位置会由函数getAbsPos实时根据父坐标和子坐标变化自动计算，
	// 因此我们不用再赋予位置的时候自己全都考虑一遍，这就是绑定的好处
	sun.pos = CVector081(0, 0, 0);
	earth.pos = CVector081(earth.R, 0, 0);
	mercury.pos = CVector081(mercury.R, 0, 0);
	venus.pos = CVector081(venus.R, 0, 0);
	moon.pos = CVector081(moon.R, 0, 0);
	station.pos = CVector081(station.R, 0, 0);
	person.pos = CVector081(0, moon.size.x + person.size.y / 2, 0);
	mars.pos = CVector081(mars.R, 0, 0);
	jupiter.pos = CVector081(jupiter.R, 0, 0);
	saturn.pos = CVector081(saturn.R, 0, 0);
	uranus.pos = CVector081(uranus.R, 0, 0);
	neptune.pos = CVector081(neptune.R, 0, 0);
	rocket.pos = CVector081(0, earth.size.x + 1.6, 0);
	rocket1.pos = CVector081(0, earth.size.x + 1.8, 0);
}

void initModel() {
	initModelData();
	initModelPos();
}

void init() {
	status_contnet = "1.Fly";
	initModel();
	initCamera();
}

void glMain(int argc, char* argv[]) {
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("第一个OpenGL程序");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(30, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutKeyboardUpFunc(&myKeyboardUpFunc);
	SetRC();
	glutMainLoop();
}
