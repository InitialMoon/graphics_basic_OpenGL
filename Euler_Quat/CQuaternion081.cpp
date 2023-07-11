#include "CEuler081.h"
#include "CVector081.h"
#include "CMatrix081.h"
#include "CQuaternion081.h"
#include <cmath>

void CQuaternion081::Set(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

CEuler081 CQuaternion081::ToEuler()
{
	CMatrix081 m = this->ToMatrix();
	CEuler081 e = m.ToEuler();
	return e;

	//this->Normalize();
	//e.p = (asin(2 * w * x - 2 * y * z) / PI) * 180;
	//if (abs(cos(e.p)) < 10e-5) {
	//	e.h = (atan2(2 * w * y - 2 * z * x, 1 - 2 * y * y - 2 * z * z) / PI) * 180;
	//	e.b = 0;
	//}
	//else {
	//	e.h = (atan2(2 * z * x + 2 * w * y, 1 - 2 * y * y - 2 * x * x) / PI) * 180;
	//	e.b = (atan2(2 * x * y + 2 * w * z, 1 - 2 * z * z - 2 * x * x) / PI) * 180;
	//}
	//return e;
}

CMatrix081 CQuaternion081::ToMatrix()
{
	CMatrix081 m = CMatrix081();
	m.m00 = 1 - 2 * y * y - 2 * z * z;
	m.m01 = 2 * x * y - 2 * w * z;
	m.m02 = 2 * x * z + 2 * w * y;
	m.m03 = 0;
	m.m10 = 2 * x * y + 2 * w * z;
	m.m11 = 1 - 2 * x * x - 2 * z * z;
	m.m12 = 2 * y * z - 2 * w * x;
	m.m13 = 0;
	m.m20 = 2 * x * z - 2 * w * y;
	m.m21 = 2 * y * z + 2 * w * x;
	m.m22 = 1 - 2 * x * x - 2 * y * y;
	m.m23 = 0;
	m.m30 = 0;
	m.m31 = 0;
	m.m32 = 0;
	m.m33 = 1;
	return m;
}

void CQuaternion081::SetAngle(float angle, CVector081 axis)
{
	w = cos(angle / 2);
	axis = axis * sin(angle / 2);
	x = axis.x;
	y = axis.y;
	z = axis.z;
}

CQuaternion081& CQuaternion081::operator=(const CQuaternion081& p)
{
	w = p.w;
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}

CQuaternion081 CQuaternion081::operator+(const CQuaternion081& p)
{
	CQuaternion081 q = CQuaternion081();
	q.w = w + p.w;
	q.x = x + p.x;
	q.y = y + p.y;
	q.z = z + p.z;
	return q;
}

CQuaternion081 CQuaternion081::operator*(float data)
{
	CQuaternion081 q = CQuaternion081();
	q.w = w * data;
	q.x = x * data;
	q.y = y * data;
	q.z = z * data;
	return q;
}

CQuaternion081 CQuaternion081::operator*(const CQuaternion081& p)
{
	CQuaternion081 q = CQuaternion081();
	q.w = w * p.w - x * p.x - y * p.y - z * p.z;
	q.x = w * p.x + x * p.w + y * p.z - z * p.y;
	q.y = w * p.y + y * p.w + z * p.x - x * p.z;
	q.z = w * p.z + z * p.w + x * p.y - y * p.x;
	return q;
}

float CQuaternion081::dotMul(const CQuaternion081& p)
{
	return x * p.x + y * p.y + z * p.z + w * p.w;
}

float CQuaternion081::len()
{
	return sqrt(w * w + x * x + y * y + z * z);
}

bool CQuaternion081::Normalize()
{
	float mod = this->len();
	if (mod > 10e-5) {
		w /= mod;
		x /= mod;
		y /= mod;
		z /= mod;
		return true;
	}
	return false;
}

CQuaternion081& CQuaternion081::Inverse()
{
	CQuaternion081 q = this->GetInverse();
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return q;
}

CQuaternion081 CQuaternion081::GetInverse()
{
	CQuaternion081 q = CQuaternion081();
	q.w = w;
	q.x = -x;
	q.y = -y;
	q.z = -z;
	float len = pow(q.len(), 2);
	q = q * (1.0 / len);
	return q;
}

CQuaternion081 CQuaternion081::Div(const CQuaternion081& b)
{
	CQuaternion081 d = CQuaternion081();
	d.w = b.w;
	d.x = b.x;
	d.y = b.y;
	d.z = b.z;
	d = *this * d.GetInverse();
	return d;
}

void CQuaternion081::GetAngle(float& angle, CVector081& axis)
{
	CQuaternion081 unitq = CQuaternion081();
	unitq = *this;
	unitq.Normalize();
	angle = 2 * acos(unitq.w);
	CVector081 v = CVector081(x, y, z);
	axis = v * (1 / sin(angle / 2));
	angle = angle / PI * 180;
}

CQuaternion081 CQuaternion081::Slerp(const CQuaternion081& Vend, float t)
{
	CQuaternion081 qt = CQuaternion081();
	CQuaternion081 thi = *this;
	CQuaternion081 vend = Vend;
	vend.Normalize();
	thi.Normalize();
	CQuaternion081 d = vend.Div(thi);
	float angle = 0;
	CVector081 v = CVector081();
	d.GetAngle(angle, v);
	CQuaternion081 q = CQuaternion081();
	q.SetAngle(angle * t / 180 * PI, v);
	qt = q * thi;
	qt.Normalize();
	return qt;
}

void CQuaternion081::Slerp(const CQuaternion081& Vend, int n, float* t, CQuaternion081* Result)
{
	for (int i = 0; i < n; i++) {
		Result[i] = Slerp(Vend, t[i]);
	}
}
