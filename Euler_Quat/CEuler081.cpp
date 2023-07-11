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

CVector081 CEuler081::ToVector3() {// ŷ����ת��Ϊ����CVector3
	CVector081 a = CVector081(0, 0, -1); // ���ó�ʼ����
	// ������ת��
	float x, y, z;
	z = - cos(h / 180 * PI) * cos(p / 180 * PI);
	x = - sin(h / 180 * PI) * cos(p / 180 * PI);
	y = sin(p / 180 * PI);
	a.Set(x, y, z);
	return a;
}


CMatrix081 CEuler081::ToMatrix() {	 //ŷ����ת��Ϊ����CMatrix  
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

void CEuler081::Normal() {//h,b�Ƕ���-180~180��p�Ƕ���-90~90��������ʱ,h��������b�Ķ�����ŷ���ǹ淶��void 
	CQuaternion081 m = this->ToQuaternion();
	*this = m.ToEuler();
}

CQuaternion081 CEuler081::ToQuaternion() {// ŷ����ת��Ϊ��Ԫ��CQuaternion
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
