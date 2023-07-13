#include "miniEngine.h"
#include "CMatrix081.h"
#include "CVector081.h"
#include "CEuler081.h"
#include "CQuaternion081.h"
#include <atlimage.h>
#include "glew.h"
#include "glut.h"
#include "CModel.h"
#include "camera.h" // ���Ҫ��д��ǰ��Ļ��Ͳ��еģ��ǵ�д�����λ�ò��ܱ����
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "string.h"
#include <vector>

using namespace std;

//������
float unit_M[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

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

// ����ģ�͵�����

// ����
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
// �ռ�վ
CModel station = CModel();
// ���
CModel rocket = CModel(); // ���ͷ
CModel rocket1 = CModel(); // �������
//��պ�
CModel sky = CModel();
//��
CModel preson = CModel();


bool rocket1_draw = true;
bool updateAll = true;
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

void updateView() { // �����ӵ��ֵ���̵ĵ�����ֵ�䶯ģ�飬��ż�λ����
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

		// �õ�ǰ�����ָ��Ĳ������ֳɵ�����������м�ר��������ֵ�����������������ģ���Ȼֵһ����������ͬһ��������
		inter_camera = *cur_camera;
		cur_camera = &inter_camera;

		for (auto& it : keyMap) { // ȡ�����м��̵�Ч��
			it.second = false;
		}
		CQuaternion081 q_Eye = cur_camera->mlookAt.GetInverse().ToQuaternion();
		CQuaternion081 result_dir = cameras[next_camera_number].mlookAt.GetInverse().ToQuaternion();

		CVector081 start_pos =cur_camera->getAbsPos();
		CVector081 end_pos = cameras[next_camera_number].getAbsPos();

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

void initLight() {
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
}

void SetRC()
{
	initLight();
	// �ǹ�������
	glShadeModel(GL_FLAT);
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	// ����Ҫ��ͼ��ģ�ͼ�����ͼ,����ֻ����һ�κ�����Ҳ��֪��Ϊɶ������ֻ�ܷ����������Ч
	moon.LoadGLTextures("Inkedmoon.jpg");
	sun.LoadGLTextures("Inkedsun.jpg");
	sky.LoadGLTextures("sky.jpg");
	earth.LoadGLTextures("Inkedearth.jpg");
	station.LoadGLTextures("station.jpg");
	person.LoadGLTextures("person.jpg");
}

// ͳһ�汾
void SetView()
{
	glLoadMatrixf(cur_camera->mlookAt);
	if (!recieve_key) {
		glTranslatef(-cur_camera->pos.x, -cur_camera->pos.y, -cur_camera->pos.z);
	}
	else { // ͳһʹ�ø�����ķ�ʽ��������ĳ�ʼλ�úͺ�����ۻ��仯���ֿ�����ͳһ����ʽ
		CVector081 v = cur_camera->getAbsPos();
		glTranslatef(-v.x, -v.y, -v.z);
	}
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
	float INIT_CAMERAPOS_X = 155, INIT_CAMERAPOS_Y = 10, INIT_CAMERAPOS_Z = 50;
	camera god_camera = camera(); // �ϵ��ӽ����
	camera earth_camera = camera(); // �����ӽ�
	camera rocket_camera = camera(); // ����ϰ󶨵����
	camera space_station_camera = camera(); // �ռ�վ�ӵ�
	camera person_camera = camera(); // ̫�����ӵ�

	// �ӵ�Ͷ�Ӧģ�Ͱ�
	god_camera.parentModel = new CModel();
	earth_camera.parentModel = &earth;
	rocket_camera.parentModel = &rocket1;
	space_station_camera.parentModel = &station;
	person_camera.parentModel = &person;

	// ���λ�ã��ӽǳ�ʼ��
	CVector081 cameraPos = CVector081(INIT_CAMERAPOS_X, INIT_CAMERAPOS_Y, INIT_CAMERAPOS_Z);
	CEuler081 direct = CEuler081();

	god_camera.pos = cameraPos;
	god_camera.elookAt = direct;
	god_camera.mlookAt = direct.ToMatrix().GetInverse();

	// �����ƫ�������ܷ��ڸ������У�Ҫ���������λ�����У����ƶ����ۼ����ϲ�
	// �����ڸ������Լ��ڱ仯��ʱ���ˢ�µ��ˣ������������ǲ��ܿ��Ƶģ�ֻ�ܰ�
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

	// ��ʼ���ָ��
	cur_camera = &cameras[0];
}

void initModelData() {

	// �������а뾶
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

	//ģ�ʹ�С����
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

	// ����ͼģ����ɫ������
	mercury.setColor(220, 220, 220);
	venus.setColor(172, 102, 17);
	mars.setColor(173, 133, 92);
	jupiter.setColor(151, 123, 99);
	saturn.setColor(181, 154, 126);
	uranus.setColor(137, 157, 168);
	neptune.setColor(99, 148, 232);

	// ����������
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
	// ��������ʹ���˸�������ķ�ʽ�����Ҹ�����ʹ�ð󶨵ķ�ʽʵ�ֵģ�
	// ����λ�û��ɺ���getAbsPosʵʱ���ݸ������������仯�Զ����㣬
	// ������ǲ����ٸ���λ�õ�ʱ���Լ�ȫ������һ�飬����ǰ󶨵ĺô�
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
	glutCreateWindow("��һ��OpenGL����");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(30, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutKeyboardUpFunc(&myKeyboardUpFunc);
	SetRC();
	glutMainLoop();
}
