#pragma once
#include <cstddef>
class CVector081;
class CMatrix081;
class CEuler081;
class CQuaternion081;

class camera {
public:
	CVector081 pos;// ���λ��������ڸ������
	float rspeed;
	float mspeed;
	int mode; // �ӵ�ģʽ����ģʽ1���ƣ���ģʽ0���ƣ����ܿ���
	CQuaternion081* view = NULL;
	CVector081* path = NULL;
	int path_pos = 0; // λ�������±�
	int path_num = 0; // �ܵ�λ�õ�λ��,��0��ʼ����Ҳ���ǵ��������-1��ʱ�����������
	int view_num = 0; // �ܵ��ӽǵ�λ��
	int view_pos = 0; // λ�������±�
	CEuler081 elookAt;
	CMatrix081 mlookAt;
	CMatrix081 parentMatrix; // ������,���������������õģ��������������еı仯������������һ�㸸���󼴿�
	camera();
	camera(CVector081 p, CEuler081 e, int m);
	camera(CVector081 p, int m, CMatrix081 M);
	camera(CVector081 p, int m, CEuler081 e, CMatrix081 pm);
	camera(CVector081 p, int m, CMatrix081 M, CMatrix081 pm);
	camera(CVector081 p, int m);
	void Set_inter_path(CVector081* interpolation_pos, int num);
	void Set_inter_view(CQuaternion081* interpolation_views, int num);
	int auto_move();
	void switch_mode(int m);
	void move_w();
	void move_s();
	void move_a();
	void move_d();
	void move_q();
	void move_e();
	void rotate_i();
	void rotate_k();
	void rotate_j();
	void rotate_l();
	void rotate_u();
	void rotate_o();
	void eulerRotate();
};
