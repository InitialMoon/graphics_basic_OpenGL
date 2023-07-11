#include "CEuler081.h"
#include "CVector081.h"
#include "CMatrix081.h"
#include "CQuaternion081.h"
#include <math.h>
#include<iomanip>
#include <iostream>

CVector081::CVector081(){}

CVector081::CVector081(const CVector081& vec) {// 这里要在形参里加const，不加直接报错，可以探究一下些在报告里
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

CVector081::CVector081(float X, float Y, float Z) {
	x = X;
	y = Y;
	z = Z;
}

void CVector081::Set(float X, float Y, float Z) {
	x = X;
	y = Y;
	z = Z;
};

CVector081 CVector081::operator+(const CVector081 &p) {
	return CVector081(x + p.x, y + p.y, z + p.z);
}

CVector081 CVector081::operator-(const CVector081 &p) {
	return CVector081(x - p.x, y - p.y, z - p.z);
}

CVector081 CVector081::operator*(const float p) {
	return CVector081(x * p, y * p, z * p);
}

CVector081 CVector081::crossMul(CVector081& p) {
	return CVector081(y * p.z - p.y * z, z * p.x - x * p.z, x * p.y - y * p.x);
}

CEuler081 CVector081::ToEuler()
{
	using namespace std;
	CEuler081 e = CEuler081();
	//e.b = 0;
	//CVector081 a = CVector081(0, 0, -1);
	//CVector081 zoxN = CVector081(0, 1, 0);
	//CVector081 projzox = this->project(zoxN);
	//CVector081 THIS = *this;
	//CVector081 proj = *this - projzox;
	//proj = proj.Normalize();
	//THIS = THIS.Normalize();
	//e.p = acos(proj.dotMul(THIS)) / PI * 180;
	//e.h = acos(a.dotMul(proj)) / PI * 180;
	e.b = 0;
	if (fabs(x) < 1e-6) {
		if (z <= 0) {
			e.h = 0.0;
			e.p = atan2(y, -z);
		}
		else {
			e.h = 3.1415926;
			e.p = atan2(y, z);
		}
	}
	else if (fabs(y) < 1e-6) {
		float beta = atan2(x, z);
		e.h = beta >= 0 ? 3.1415926 - beta : -3.1415926 - beta;
		e.h = -e.h;
		e.p = 0.0;
	}
	else {
		float beta = atan2(x, z);
		//std::cout << beta;
		e.h = beta >= 0 ? 3.1415926 - beta : -3.1415926 - beta;
		e.h = -e.h;
		e.p = atan2(y, sqrt(x * x + z * z));
	}
	e.b = e.b * 180 / PI;
	e.h = e.h * 180 / PI;
	e.p = e.p * 180 / PI;
	e.Normal();

	return e;
}

CVector081 CVector081::project(CVector081& n) {
	CVector081 nn = n;
	nn.Normalize();
	return nn * (this->dotMul(nn) * nn.len());
}

CVector081 CVector081::Slerp(CVector081& Vend, float t)
{
	CVector081 v = CVector081();
	v = *this + (Vend - *this) * t;
	return v;
}

void CVector081::Slerp(CVector081& Vend, int n, float* t, CVector081* Result)
{
	for (int i = 0; i < n; i++) {
		Result[i] = Slerp(Vend, t[i]);
	}
}

CVector081& CVector081::operator=(const CVector081 &p) {
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}

bool CVector081::operator==(CVector081 &p) {
	CVector081 v;
	return this->x == p.x && this->y == p.y && this->z == p.z;
}

bool CVector081::operator!=(CVector081 &p) {
	return !(*this == p);
}

float CVector081::dotMul(CVector081& n) {
	return (this->x * n.x + this->y * n.y + this->z * n.z);
}

CVector081 CVector081::Normalize() {
	float model = len();
	return CVector081(x / model, y / model, z / model);
}

float CVector081::len() {
	return pow(x * x + y * y + z * z, 0.5);
}


