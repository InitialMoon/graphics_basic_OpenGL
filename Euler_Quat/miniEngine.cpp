#include "miniEngine.h"
#include "CMatrix081.h"
#include "CVector081.h"
#include "CEuler081.h"
#include "CQuaternion081.h"
#include <atlimage.h>
#include "glew.h"
#include "glut.h"
#include "camera.h" // ���Ҫ��д��ǰ��Ļ��Ͳ��еģ��ǵ�д�����λ�ò��ܱ����
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "string.h"
#include <vector>
// ����

using namespace std;

//��������
GLuint sun_texture[1] = { 0 };
GLuint moon_texture[1] = { 0 };
GLuint space_station_texture[1] = { 0 };
GLuint earth_texture[1] = { 0 };
GLuint sky_texture[1] = { 0 };
GLuint person_texture[1] = { 0 };

//������
float unit_M[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
float INIT_CAMERAPOS_X = 155, INIT_CAMERAPOS_Y = 10, INIT_CAMERAPOS_Z = 50;

//2D������
char* status_contnet;

// ���
// ��¼��ǰ����Ǽ��������0-4
int camera_number = 0;
int next_camera_number = (camera_number + 1) % 5; // ��¼��һ��������
bool can_switch_camera = true; // �ܷ�����ӵ㣬Ҳ�����Ƿ��ȶ���
vector<camera> cameras;
camera* cur_camera; // ��������ǵ�ǰʹ�õ��Ǹ���������ܱ����������,ֱ������һ��ָ�뷽��
camera inter_camera; // ��ֵ�õ����

// ������
double theta = 0.0; // ������ת�ĽǶ�

int view_mode = 0; // �ӵ�λ�ã��������ϵ��ӽǵ��յ�ʱֵΪ0���ڻ���ӵ���ʱΪ1

bool recieve_key = true; // �����ֵΪ��ʱ��ƽ���仯��ϵ�ʱ�򣬵�Ϊ�ٵ�ʱ�򣬾ʹ����ڱ仯��·���ϣ��������κμ������룬ͬʱ�ᱣ֤���е�����ֵ��Ϊfalse
bool planets_rotate = false;
bool need_fly = true; // ��¼����Ƿ���Ҫ�����ɣ���Ϊ�ɹ�ͷ��ֱ������������������ض���
bool have_started = false; // ��¼�������ɹ�����ֹ׷�������Ĺ����и߶ȵ��������ֵ�������������
bool rocket_start = false; 

// ����λ��
CVector081 sun_pos = CVector081();
CVector081 mercury_pos = CVector081();
CVector081 venus_pos = CVector081();
CVector081 earth_pos = CVector081();
CVector081 moon_pos = CVector081();
CVector081 space_station_pos = CVector081();
CVector081 person_pos = CVector081();
CVector081 mars_pos = CVector081();
CVector081 jupiter_pos = CVector081();
CVector081 saturn_pos = CVector081();
CVector081 uranus_pos = CVector081();
CVector081 neptune_pos = CVector081();

//����İ뾶
float rMoon = 3;
float rEarth = 10;
//�ռ�վ�ı߳�
float station_size = 3;
//�˵�����
float person_y = 0.1;
float person_x = 0.03;
float person_z = 0.03;

// �������а뾶
float RMercury = 110;
float RVenus = 130;
float REarth = 160;
float RMoon = 25;
float RStation = 15;
float RMars = 210;
float RJupiter = 240;
float RSaturn = 320;
float RUranus = 390;
float RNeptune = 420;

// ���λ�ú��ٶ���Ϣ
CVector081 rocket1_pos = CVector081();
CVector081 rocket1_v = CVector081();
CVector081 rocket1_rotate_axis = CVector081();
float rocket_to_moon_seta = 0;
CVector081 dir_rocket_to_moon = CVector081();// ����ͻ�����߷���

CVector081 rocket2_pos = CVector081();
CVector081 rocket2_speed = CVector081();

bool pressed2 = true;
bool rocket2_draw = true;
bool updateAll = true;
bool pos_complete = false;// λ�ò�ֵ��ϵı�־
float rocket_seta = 0; // ��¼�����ת�ĽǶȣ���ָ�䷽��ת��ʱ��

// �Ƿ���ܻس�����Ҳ�����ڶ��������ڼ���Ϊfalse��������Ҷ�������
bool recieve_enter = true;

map<char, bool> keyMap = { //map�����const���Σ���ô��û����[]������
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
void myRotate(float seta, float x, float y, float z) { // �Լ�ʵ�ֵ���ת����
	CMatrix081 mymatrix;
	mymatrix.SetRotate(seta, CVector081(x, y, z));
	glMultMatrixf(mymatrix);
}
void myTranslatef(float x, float y, float z) { // �Լ�ʵ�ֵ�ƽ�ƺ���
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
				if (cur_camera->rspeed > 0.2) { // ��ֹ���ּ���ͷ������ٵ����
					cur_camera->rspeed -= 0.2;
				}
				break;
			case '[':
				cur_camera->mspeed += 0.2;
				break;
			case ']':
				if (cur_camera->mspeed > 0.2) { // ��ֹ���ּ���ͷ������ٵ����
					cur_camera->mspeed -= 0.2;
				}
				break;
			}
		}
	}
}

void updateRocket() {
	if (rocket_start) {
		if (!have_started) {
			if (rocket1_pos.y < 15) {
				rocket1_pos = rocket1_pos + rocket1_v;
				rocket2_pos = rocket2_pos + rocket2_speed;
			}
			else if (rocket1_pos.y >= 15 && rocket1_pos.y < 20) {
				rocket1_pos = rocket1_pos + rocket1_v;
				rocket2_pos = rocket2_pos - rocket2_speed;
			}
			else if (rocket1_pos.y >= 20) {
				if (rocket2_pos.y > 10) {
					rocket2_pos = rocket2_pos - rocket2_speed;
				}
				else if (rocket2_pos.y <= 10) {
					rocket2_draw = false;
				}
				if (rocket_seta < 90) {
					rocket_seta += 1;
				}
			}
		}
		if ((moon_pos - rocket1_pos).len() < 1.5) {
			need_fly = false;
		}
		if (need_fly && rocket_seta >= 90) {
			have_started = true;
			// ��������
			CVector081 dir_rocket_to_moon = (moon_pos - rocket1_pos).Normalize();
			rocket1_pos = rocket1_pos + dir_rocket_to_moon * 0.5;
			rocket1_rotate_axis = dir_rocket_to_moon.crossMul(rocket1_v).Normalize();
			float cos_val = rocket1_v.Normalize().dotMul(dir_rocket_to_moon);
			rocket_to_moon_seta = -acos(cos_val) / PI * 180;
		}
		if ((moon_pos - rocket1_pos).len() > 1.5) {
			need_fly = true;
		}
	}
	cameras[2].parentMatrix.SetTrans(rocket1_pos);
	//rocket_camera.parentMatrix.SetTrans(rocket1_pos);
}

void updatePlanets() {
	theta += 0.1;
	if ((theta / (22.5 * 8.8 * 36.5 * 67.8 * 3 * 433.3 * 37.8 * 16.5 * 3079.9)) > 360) {
		theta -= (22.5 * 8.8 * 36.5 * 67.8 * 3 * 433.3 * 37.8 * 16.5 * 3079.9) * 360;
	}
	venus_pos.x = cos(theta / 22.5) * RVenus;
	venus_pos.z = sin(theta / 22.5) * RVenus;
	mercury_pos.x = cos(theta / 8.8) * RMercury;
	mercury_pos.z = sin(theta / 8.8) * RMercury;
	earth_pos.x = cos(theta / 36.5) * REarth;
	earth_pos.z = sin(theta / 36.5) * REarth;
	mars_pos.x = cos(theta / 68.7) * RMars;
	mars_pos.z = sin(theta / 68.7) * RMars;

	moon_pos.x = cos(theta / 3) * RMoon;
	moon_pos.z = sin(theta / 3) * RMoon;
	CMatrix081 transM = CMatrix081();
	transM.SetTrans(earth_pos);
	moon_pos = transM.posMul(moon_pos);

	space_station_pos.x = cos(theta / 2) * RStation;
	space_station_pos.z = sin(theta / 2) * RStation;
	space_station_pos = transM.posMul(space_station_pos);

	person_pos.x = 0;
	person_pos.y = cos(theta / 20) * (rMoon + person_y);
	person_pos.z = sin(theta / 20) * (rMoon + person_y);
	CMatrix081 transM1 = CMatrix081();
	transM1.SetTrans(moon_pos);
	person_pos = transM1.posMul(person_pos);
	//cout << person_pos[0] << endl << person_pos[1] << endl << person_pos[2] << endl;

	// ���°����ӵ��ϵ����λ��
	cameras[1].parentMatrix.SetTrans(earth_pos);
	cameras[3].parentMatrix.SetTrans(space_station_pos);
	cameras[4].parentMatrix.SetTrans(person_pos);

	jupiter_pos.x = cos(theta / 433.3) * RJupiter;
	jupiter_pos.z = sin(theta / 433.3) * RJupiter;
	saturn_pos.x = cos(theta / 37.8) * RSaturn;
	saturn_pos.z = sin(theta / 37.8) * RSaturn;
	neptune_pos.x = cos(theta / 16.5) * RNeptune;
	neptune_pos.z = sin(theta / 16.5) * RNeptune;
	uranus_pos.x = cos(theta / 3079.9) * RUranus;
	uranus_pos.z = sin(theta / 3079.9) * RUranus;
}

void updateView() {
	if (!recieve_key) {
		if (cur_camera->auto_move() == 0) {
			for (int i = 0; i < 3; i++) {
				cout << cur_camera->pos[i] << ",";
			}
			cout << endl;
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

		// �õ�ǰ�����ָ��Ĳ������ֳɵ�����������м�ר��������ֵ�����������������ģ���Ȼֵһ����������ͬһ��������
		inter_camera = *cur_camera;
		cur_camera = &inter_camera;

		for (auto& it : keyMap) { // ȡ�����м��̵�Ч��
			it.second = false;
		}
		CQuaternion081 q_Eye = cur_camera->mlookAt.GetInverse().ToQuaternion();
		CQuaternion081 result_dir = cameras[next_camera_number].mlookAt.GetInverse().ToQuaternion();

		//for (int i = 0; i < 3; i++) {
		//	cout << cameras[next_camera_number].elookAt[i] << ", ";
		//}
		//cout << endl;
		//for (int i = 0; i < 3; i++) {
		//	cout << cur_camera->elookAt[i] << ", ";
		//}
		//cout << endl;

		//for (int i = 0; i < 4; i++) {
		//	cout << q_Eye[i] << ", ";
		//}
		//cout << endl;
		//for (int i = 0; i < 4; i++) {
		//	cout << result_dir[i] << ", ";
		//}
		//cout << endl;

		CVector081 start_pos =cur_camera->parentMatrix.posMul(cur_camera->pos);
		CVector081 end_pos =cameras[next_camera_number].parentMatrix.posMul(cameras[next_camera_number].pos);

		// ���ݽǶȲ�ȷ����ֵ����
		cur_camera->view_num = int (abs(cur_camera->elookAt.h - cameras[next_camera_number].elookAt.h) +
			abs(cur_camera->elookAt.p - cameras[next_camera_number].elookAt.p) +
			abs(cur_camera->elookAt.b - cameras[next_camera_number].elookAt.b)) / 10;
		// ���ݾ����ȷ����ֵ����
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

void SetRC()
{
	// ��������
	glClearColor(0, 0, 0, 1);
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float amb[4] = { 0.4,0.4,0.4,1.0 };					//��ɫ������
	float dif[4] = { 1.0,1.0,1.0,1 };					//��ɫ�����ͷ����
	float spe[4] = { 1.0,1.0,1.0,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// �ǹ�������
	glShadeModel(GL_FLAT);
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	LoadGLTextures("Inkedsun.jpg", *sun_texture);
	LoadGLTextures("Inkedmoon.jpg", *moon_texture);
	LoadGLTextures("sky.jpg", *sky_texture);
	LoadGLTextures("Inkedearth.jpg", *earth_texture);
	LoadGLTextures("station.jpg", *space_station_texture);
	LoadGLTextures("person.jpg", *person_texture);
}

// ͳһ�汾
void SetView()
{
	glLoadMatrixf(cur_camera->mlookAt);
	if (!recieve_key) {
		glTranslatef(-cur_camera->pos.x, -cur_camera->pos.y, -cur_camera->pos.z);
	}
	else { // ͳһʹ�ø�����ķ�ʽ��������ĳ�ʼλ�úͺ�����ۻ��仯���ֿ�����ͳһ����ʽ
		CVector081 v = cur_camera->parentMatrix.posMul(cur_camera->pos);
		glTranslatef(-v.x, -v.y, -v.z);
	}

	//if (view_mode == 1 && !recieve_key) {
	//	glTranslatef(-cur_camera->pos.x, -cur_camera->pos.y, -cur_camera->pos.z);
	//}
	//if (view_mode == 1 && recieve_key) {
	//	CVector081 v = cur_camera->parentMatrix.posMul(cur_camera->pos);
	//	glTranslatef(-v.x, -v.y, -v.z);
	//}
	//if (view_mode == 0) {
	//	glTranslatef(-cur_camera->pos.x, -cur_camera->pos.y, -cur_camera->pos.z);
	//}
}

void DrawRocket1()
{
	glPushMatrix();
	myTranslatef(rocket1_pos.x, rocket1_pos.y, rocket1_pos.z);
	//myRotate(90, 1, 0, 0); // �����Լ�����ת����ʵ����ת
	// ת��ƽ�ɹ����õ���ת
	if (!have_started) {
		myRotate(-rocket_seta, 0, 0, 1);
	}
	else {
		myRotate(rocket_to_moon_seta, rocket1_rotate_axis.x, rocket1_rotate_axis.y, rocket1_rotate_axis.z);
	}

	//���ͷ
	glColor3f(0.5, 0, 0);
	glPushMatrix();
	myRotate(-90, 1, 0, 0); // �����Լ�����ת����ʵ����ת
	glutSolidCone(0.1, 0.3, 10, 10);
	glPopMatrix();

	//�����
	glPushMatrix();
	glColor3f(119 / 255.0, 137 / 255.0, 173 / 255.0);
	myTranslatef(0, -0.7, 0);
	myScalef(0.1, 1.5, 0.1);
	myRotate(-90, 1, 0, 0);
	glutSolidTorus(0.5, 0.7, 10, 10);
	glPopMatrix();

	myTranslatef(-rocket1_pos.x, -rocket1_pos.y, -rocket1_pos.z);
	glPopMatrix();
}

void DrawRocket2()
{
	if (rocket2_draw) {
		myTranslatef(rocket2_pos.x, rocket2_pos.y, rocket2_pos.z);
		glPushMatrix();
		//�����
		glColor3f(53 / 255.0, 42 / 255.0, 85 / 255.0);
		glPushMatrix();

		glScalef(0.15, 1.5, 0.15);
		myRotate(-90, 1, 0, 0);
		glutSolidTorus(0.5, 0.7, 10, 10);

		glPopMatrix();

		glPopMatrix();
		myTranslatef(-rocket2_pos.x, -rocket2_pos.y, -rocket2_pos.z);
	}
}

void DrawRocket() {
	DrawRocket1();
	DrawRocket2();
}


void DrawPlanet(float r, float g, float b, float rr) {
	glColor3f(r / 255.0, g / 255.0, b / 255.0);
	gluSphere(gluNewQuadric(), rr, min(rr * 10.0, 100.0), min(rr * 10.0, 100.0));
}

void DrawSkyBox() {
	DrawBox(500, 500, 500, *sky_texture);
}

// �ռ�վ���ǵڶ�������
void DrawSpaceStation() {
	glPushMatrix();
	myTranslatef(space_station_pos.x, space_station_pos.y, space_station_pos.z);
	DrawBox(1.5, 1.5, 1.5, *space_station_texture);
	myTranslatef(-space_station_pos.x, -space_station_pos.y, -space_station_pos.z);
	glPopMatrix();
}

void DrawPerson() {
	glPushMatrix();
	myTranslatef(person_pos.x, person_pos.y, person_pos.z);
	myRotate(theta / 20 * 180 / 3.14, 1, 0, 0);
	//myRotate(-theta / 20 * 180 / 3.14, 0, 1, 0);
	DrawBox(person_x, person_y, person_z, *person_texture);
	myTranslatef(-person_pos.x, -person_pos.y, -person_pos.z);
	glPopMatrix();
}

void DrawSun() {
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glBindTexture(GL_TEXTURE_2D, sun_texture[0]);
	glEnable(GL_TEXTURE_GEN_S);
	glPolygonMode(GL_BACK, GL_FILL);
	glEnable(GL_TEXTURE_GEN_T);
	float pos[4] = {0,0,0,1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	DrawPlanet(255, 255, 255, 100);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}
void DrawMercury() {
	// ˮ�ǣ���ɫ
	glPushMatrix();
	myTranslatef(mercury_pos.x, mercury_pos.y, mercury_pos.z);
	DrawPlanet(220, 220, 220, 3);
	myTranslatef(-mercury_pos.x, -mercury_pos.y, -mercury_pos.z);
	glPopMatrix();
}
void DrawVenus() {
	glPushMatrix();
	myTranslatef(venus_pos.x, venus_pos.y, venus_pos.z);
	DrawPlanet(172, 102, 17, 5);
	myTranslatef(-venus_pos.x, -venus_pos.y, -venus_pos.z);
	glPopMatrix();
}
void DrawMoon() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glBindTexture(GL_TEXTURE_2D, moon_texture[0]);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	myTranslatef(moon_pos.x, moon_pos.y, moon_pos.z);
	DrawPlanet(255, 255, 255, rMoon);
	myTranslatef(-moon_pos.x, -moon_pos.y, -moon_pos.z);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void DrawEarth() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glBindTexture(GL_TEXTURE_2D, earth_texture[0]);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	myTranslatef(earth_pos.x, earth_pos.y, earth_pos.z);
	DrawPlanet(255, 255, 255, rEarth);
	myTranslatef(-earth_pos.x, -earth_pos.y, -earth_pos.z);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void DrawMars() {
	glPushMatrix();
	myTranslatef(mars_pos.x, mars_pos.y, mars_pos.z);
	DrawPlanet(173, 133, 92, 6);
	myTranslatef(-mars_pos.x, -mars_pos.y, -mars_pos.z);
	glPopMatrix();
}
void DrawJupiter() {
	glPushMatrix();
	myTranslatef(jupiter_pos.x, jupiter_pos.y, jupiter_pos.z);
	DrawPlanet(151, 123, 99, 20);
	myTranslatef(-jupiter_pos.x, -jupiter_pos.y, -jupiter_pos.z);
	glPopMatrix();
}
void DrawSaturn() {
	glPushMatrix();
	myTranslatef(saturn_pos.x, saturn_pos.y, saturn_pos.z);
	DrawPlanet(181, 154, 126, 30);
	myTranslatef(-saturn_pos.x, -saturn_pos.y, -saturn_pos.z);
	glPopMatrix();
}
void DrawUranus() {
	glPushMatrix();
	myTranslatef(uranus_pos.x, uranus_pos.y, uranus_pos.z);
	DrawPlanet(137, 157, 168, 9);
	myTranslatef(-uranus_pos.x, -uranus_pos.y, -uranus_pos.z);
	glPopMatrix();
}
void DrawNeptune() {
	glPushMatrix();
	myTranslatef(neptune_pos.x, neptune_pos.y, neptune_pos.z);
	DrawPlanet(99, 148, 232, 8);
	myTranslatef(-neptune_pos.x, -neptune_pos.y, -neptune_pos.z);
	glPopMatrix();
}

void DrawGalaxy() {
	DrawSun();
	DrawMercury();
	DrawVenus();
	DrawEarth();
	DrawSpaceStation();
	DrawMoon();
	DrawMars();
	DrawJupiter();
	DrawSaturn();
	DrawUranus();
	DrawNeptune();
}

void Font2D(char* str, double x, double y, int size)
{
	glColor3f(1, 1, 1);
	//����ͶӰ��ʽ��ƽ��ͶӰ
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//����ַ���
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
	//�ָ�ͶӰ��ʽ
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void RenderWorld() {
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	DrawGalaxy();
	DrawRocket();
	DrawPerson();
	DrawXYZAxis(500);
	DrawSkyBox();

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
	//ģ�Ϳ���
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
	camera god_camera; // �ϵ��ӽ����
	camera earth_camera; // �����ӽ�
	camera rocket_camera; // ����ϰ󶨵����
	camera space_station_camera; // �ռ�վ�ӵ�
	camera person_camera; // ̫�����ӵ�

	// ���λ�ã��ӽǳ�ʼ��
	CVector081 cameraPos = CVector081(INIT_CAMERAPOS_X, INIT_CAMERAPOS_Y, INIT_CAMERAPOS_Z);
	CEuler081 direct = CEuler081();

	god_camera = camera();
	//god_camera.parentMatrix.SetTrans(cameraPos);
	god_camera.pos = cameraPos;
	god_camera.elookAt = direct;
	god_camera.mlookAt = direct.ToMatrix().GetInverse();

	cameraPos = earth_pos;
	cameraPos.y = earth_pos.y;
	earth_camera = camera();
	earth_camera.parentMatrix.SetTrans(cameraPos);
	// �����ƫ�������ܷ��ڸ������У�Ҫ���������λ�����У����ƶ����ۼ����ϲ�
	// �����ڸ������Լ��ڱ仯��ʱ���ˢ�µ��ˣ������������ǲ��ܿ��Ƶģ�ֻ�ܰ�
	earth_camera.pos.y = rEarth;
	direct.h = 90;
	direct.p = 0;
	earth_camera.elookAt = direct;
	earth_camera.mlookAt = direct.ToMatrix().GetInverse();

	cameraPos = rocket1_pos;
	rocket_camera = camera();
	rocket_camera.parentMatrix.SetTrans(cameraPos);
	rocket_camera.pos.x = - 3;
	direct.h = -90;
	direct.p = 0;
	rocket_camera.elookAt = direct;
	rocket_camera.mlookAt = direct.ToMatrix().GetInverse();

	cameraPos = space_station_pos;
	space_station_camera = camera();
	space_station_camera.parentMatrix.SetTrans(cameraPos);
	space_station_camera.pos.y = 1.5;
	direct.h = -90;
	direct.p = 30;
	space_station_camera.elookAt = direct;
	space_station_camera.mlookAt = direct.ToMatrix().GetInverse();

	cameraPos = person_pos;
	person_camera = camera();
	person_camera.parentMatrix.SetTrans(cameraPos);
	person_camera.pos.y = person_y;
	direct.h = 90;
	direct.p = 10;
	person_camera.elookAt = direct;
	person_camera.mlookAt = direct.ToMatrix().GetInverse();

	cameras.push_back(god_camera);
	cameras.push_back(earth_camera);
	cameras.push_back(rocket_camera);
	cameras.push_back(space_station_camera);
	cameras.push_back(person_camera);

	// ��ʼ���ָ��
	cur_camera = &cameras[0];

	//dir_rocket_to_moon = (moon_pos - rocket1_pos).Normalize();// ����ͻ�����߷���
	//rocket_camera.mlookAt = dir_rocket_to_moon.Normalize().ToEuler().ToMatrix().GetInverse();
	//rocket_camera.elookAt = rocket_camera.mlookAt.GetInverse().ToEuler();
}

void initModelPos() {
	earth_pos = CVector081(REarth, 0, 0);
	CMatrix081 transM = CMatrix081();
	transM.SetTrans(earth_pos);

	rocket1_pos = CVector081(0, 12, 0);
	rocket2_pos = CVector081(0, 10.55, 0);
	rocket1_v = CVector081(0, 0.1, 0.0001);
	rocket2_speed = CVector081(0, 0.1, 0.0001);
	rocket1_rotate_axis = CVector081(0, 0, 1);

	rocket1_pos = transM.posMul(rocket1_pos);
	rocket2_pos = transM.posMul(rocket2_pos);

	sun_pos = CVector081(0, 0, 0);
	mercury_pos = CVector081(RMercury, 0, 0);
	venus_pos = CVector081(RVenus, 0, 0);

	// ������λ��ʹ�õ��������̫���ģ�Ҳ���Ǿ�������ϵ����������������������ز���
	moon_pos = CVector081(RMoon, 0, 0);
	moon_pos = transM.posMul(moon_pos);
	space_station_pos = CVector081(RStation, 0, 0);
	space_station_pos = transM.posMul(space_station_pos);

	// ���˷�����ȷ�����λ����
	person_pos = CVector081(0, rMoon + person_y / 2, 0);
	CMatrix081 transM1 = CMatrix081();
	transM1.SetTrans(moon_pos);
	person_pos = transM1.posMul(person_pos);
	//cout << person_pos[0] << endl << person_pos[1] << endl << person_pos[2] << endl;

	mars_pos = CVector081(RMars, 0, 0);
	jupiter_pos = CVector081(RJupiter, 0, 0);
	saturn_pos = CVector081(RSaturn, 0, 0);
	uranus_pos = CVector081(RUranus, 0, 0);
	neptune_pos = CVector081(RNeptune, 0, 0);
}

void initLight() {
	float pos[4] = {0,0,0,1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void init() {
	status_contnet = "1.Fly";
	initLight();
	initModelPos();
	initCamera();
}

void glMain(int argc, char* argv[]) {
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("��һ��OpenGL����");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(30, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutKeyboardUpFunc(&myKeyboardUpFunc);
	SetRC();
	glutMainLoop();
}
