#include "CEuler081.h"
#include "CVector081.h"
#include "CMatrix081.h"
#include "CQuaternion081.h"
#include <math.h>

void CEuler081::Set(float fh, float fp, float fb)
{
	h = fh;
	p = fp;
	b = fb;
}

CVector081 CEuler081::ToVector3() {// 欧拉角转换为向量CVector3
	CVector081 a = CVector081(0, 0, -1); // 设置初始方向
	// 设置旋转轴
	float x, y, z;
	z = - cos(h / 180 * PI) * cos(p / 180 * PI);
	x = - sin(h / 180 * PI) * cos(p / 180 * PI);
	y = sin(p / 180 * PI);
	a.Set(x, y, z);
	return a;
}


CMatrix081 CEuler081::ToMatrix() {	 //欧拉角转换为矩阵CMatrix  
	CVector081 x = CVector081(1, 0, 0);
	CVector081 y = CVector081(0, 1, 0);
	CVector081 z = CVector081(0, 0, 1);
	CMatrix081 rx = CMatrix081();
	CMatrix081 ry = CMatrix081();
	CMatrix081 rz = CMatrix081();
	rx.SetRotate(p, x);
	ry.SetRotate(h, y);
	rz.SetRotate(b, z);
	return ry * rx * rz;
}

void CEuler081::Normal() {//h,b角度在-180~180，p角度在-90~90。万向锁时,h代替所有b的度数。欧拉角规范化void 
	CQuaternion081 m = this->ToQuaternion();
	*this = m.ToEuler();
}

CQuaternion081 CEuler081::ToQuaternion() {// 欧拉角转换为四元数CQuaternion
	CQuaternion081 q = CQuaternion081();
	float k = 0.5 / 180 * PI;
	float h = this->h * k;
	float b = this->b * k;
	float p = this->p * k;
	q.w = cos(h) * cos(p) * cos(b) + sin(h) * sin(p) * sin(b);
	q.x = cos(h) * sin(p) * cos(b) + sin(h) * cos(p) * sin(b);
	q.y = sin(h) * cos(p) * cos(b) - cos(h) * sin(p) * sin(b);
	q.z = cos(h) * cos(p) * sin(b) - sin(h) * sin(p) * cos(b);
	return q;
}
