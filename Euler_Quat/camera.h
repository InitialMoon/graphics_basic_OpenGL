#pragma once
#include <cstddef>
class CVector081;
class CMatrix081;
class CEuler081;
class CQuaternion081;

class camera {
public:
	CVector081 pos;// 这个位置是相对于父矩阵的
	float rspeed;
	float mspeed;
	int mode; // 视点模式，受模式1控制，受模式0控制，不受控制
	CQuaternion081* view = NULL;
	CVector081* path = NULL;
	int path_pos = 0; // 位置数组下标
	int path_num = 0; // 总的位置点位数,从0开始数，也就是等于这个数-1的时候就无数据了
	int view_num = 0; // 总的视角点位数
	int view_pos = 0; // 位置数组下标
	CEuler081 elookAt;
	CMatrix081 mlookAt;
	CMatrix081 parentMatrix; // 父矩阵,这玩意是在外面用的，这里面的相机所有的变化都在外面套上一层父矩阵即可
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
