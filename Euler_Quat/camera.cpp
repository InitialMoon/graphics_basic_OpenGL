#include "CEuler081.h"
#include "CMatrix081.h"
#include "CVector081.h"
#include "CQuaternion081.h"
#include "glut.h"
#include "camera.h"
#include <iostream>

camera::camera() {
	float unit_M[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	rspeed = 1.0;
	mspeed = 1.0;
	pos.Set(0, 0, 0);
	mode = 1;
	elookAt.h = 0;
	elookAt.p = 0;
	elookAt.b = 0;
	mlookAt = elookAt.ToMatrix().GetInverse();
	parentMatrix.Set(unit_M);
}
camera::camera(CVector081 p, CEuler081 e, int m) {
	float unit_M[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	parentMatrix.Set(unit_M);
	rspeed = 1.0;
	mspeed = 1.0;
	pos = p;
	mode = m;
	elookAt = e;
	mlookAt = e.ToMatrix().GetInverse();
}
camera::camera(CVector081 p, int m, CMatrix081 M) {
	float unit_M[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	parentMatrix.Set(unit_M);
	rspeed = 1.0;
	mspeed = 1.0;
	pos = p;
	mode = m;
	mlookAt = M;
	elookAt = M.GetInverse().ToEuler();
}
camera::camera(CVector081 p, int m) {
	float unit_M[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	parentMatrix.Set(unit_M);
	rspeed = 1.0;
	mspeed = 1.0;
	pos = p;
	mode = m;
	elookAt.h = 0;
	elookAt.p = 0;
	elookAt.b = 0;
	mlookAt = elookAt.ToMatrix();
}
camera::camera(CVector081 p, int m, CEuler081 e, CMatrix081 pm) {
	rspeed = 1.0;
	mspeed = 1.0;
	pos = p;
	mode = m;
	elookAt = e;
	mlookAt = e.ToMatrix().GetInverse();
	parentMatrix = pm;
}
camera::camera(CVector081 p, int m, CMatrix081 M, CMatrix081 pm) {
	rspeed = 1.0;
	mspeed = 1.0;
	pos = p;
	mode = m;
	mlookAt = M;
	elookAt = M.GetInverse().ToEuler();
	parentMatrix = pm;
}
void camera::Set_inter_path(CVector081* interpolation_pos, int num) {
	path = interpolation_pos;
	path_num = num;
}
void camera::Set_inter_view(CQuaternion081* interpolation_views, int num) {
	view = interpolation_views;
	view_num = num;
}
int camera::auto_move() {
	// 暂时只提供一种先转视角后移动位置的方式,当移动完毕后返回1，否则返回0
	if (view != NULL && path != NULL) {
		if (view_pos < view_num) {
			pos = path[path_pos];
			mlookAt = view[view_pos].ToMatrix().GetInverse();
			//elookAt = mlookAt.ToEuler();
			elookAt = mlookAt.GetInverse().ToEuler();
			view_pos++;
			return 0;
		}
		else if (path_pos < path_num) {
			pos = path[path_pos];
			path_pos++;
			return 0;
		}
		else {
			path_pos = 0;
			view_pos = 0;
			return 1;
		}
	}
	else {
		std::cout << "请先设置插值路径和视角变化路径，不能为空" << std::endl;
	}
}
void camera::switch_mode(int m) {
	mode = m;
}
void camera::move_w() {
	pos.x += mlookAt[1] * mspeed;
	pos.y += mlookAt[5] * mspeed;
	pos.z += mlookAt[9] * mspeed;
}
void camera::move_s() {
	pos.x -= mlookAt[1] * mspeed;
	pos.y -= mlookAt[5] * mspeed;
	pos.z -= mlookAt[9] * mspeed;
}
void camera::move_a() {
	pos.x -= mlookAt[0] * mspeed;
	pos.y -= mlookAt[4] * mspeed;
	pos.z -= mlookAt[8] * mspeed;
}
void camera::move_d() {
	pos.x += mlookAt[0] * mspeed;
	pos.y += mlookAt[4] * mspeed;
	pos.z += mlookAt[8] * mspeed;
}
void camera::move_q() {
	pos.x -= mlookAt[2] * mspeed;
	pos.y -= mlookAt[6] * mspeed;
	pos.z -= mlookAt[10] * mspeed;
}
void camera::move_e() {
	pos.x += mlookAt[2] * mspeed;
	pos.y += mlookAt[6] * mspeed;
	pos.z += mlookAt[10] * mspeed;
}
void camera::rotate_i() {
	if (mode == 0)
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(-rspeed, 1, 0, 0);
		glMultMatrixf(mlookAt);
		glGetFloatv(GL_MODELVIEW_MATRIX, mlookAt);
		elookAt = mlookAt.GetInverse().ToEuler();
		glPopMatrix();
	}
	else
	{
		elookAt.Normal();
		elookAt.p += rspeed;
		//std::cout << "elookAt.p = " << elookAt.p << std::endl;
		eulerRotate();
	}
}
void camera::rotate_k() {
	if (mode == 0)
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(rspeed, 1, 0, 0);
		glMultMatrixf(mlookAt);
		glGetFloatv(GL_MODELVIEW_MATRIX, mlookAt);
		elookAt = mlookAt.GetInverse().ToEuler();
		glPopMatrix();
	}
	else
	{
		elookAt.Normal();
		elookAt.p -= rspeed;
		//std::cout << "elookAt.p = " << elookAt.p << std::endl;
		eulerRotate();
	}
}
void camera::rotate_j() {
	if (mode == 0)
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(-rspeed, 0, 1, 0);
		glMultMatrixf(mlookAt);
		glGetFloatv(GL_MODELVIEW_MATRIX, mlookAt);
		elookAt = mlookAt.GetInverse().ToEuler();
		glPopMatrix();
	}
	else
	{
		elookAt.Normal();
		elookAt.h += rspeed;
		//std::cout << "elookAt.h = " << elookAt.h << std::endl;
		eulerRotate();
	}
}
void camera::rotate_l() {
	if (mode == 0)
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(rspeed, 0, 1, 0);
		glMultMatrixf(mlookAt);
		glGetFloatv(GL_MODELVIEW_MATRIX, mlookAt);
		elookAt = mlookAt.GetInverse().ToEuler();
		glPopMatrix();
	}
	else
	{
		elookAt.Normal();
		elookAt.h -= rspeed;
		//std::cout << "elookAt.h = " << elookAt.h << std::endl;
		eulerRotate();
	}
}
void camera::rotate_u() {
	if (mode == 0)
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(-rspeed, 0, 0, 1);
		glMultMatrixf(mlookAt);
		glGetFloatv(GL_MODELVIEW_MATRIX, mlookAt);
		elookAt = mlookAt.GetInverse().ToEuler();
		glPopMatrix();
	}
	else
	{
		elookAt.Normal();
		elookAt.b += rspeed;
		//std::cout << "elookAt.b = " << elookAt.b << std::endl;
		eulerRotate();
	}
}
void camera::rotate_o() {
	if (mode == 0)
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(rspeed, 0, 0, 1);
		glMultMatrixf(mlookAt);
		glGetFloatv(GL_MODELVIEW_MATRIX, mlookAt);
		elookAt = mlookAt.GetInverse().ToEuler();
		glPopMatrix();
	}
	else
	{
		elookAt.Normal();
		elookAt.b -= rspeed;
		//std::cout << "elookAt.b = " << elookAt.b << std::endl;
		eulerRotate();
	}
}

void camera::eulerRotate() {
	mlookAt = elookAt.ToMatrix().GetInverse();
}