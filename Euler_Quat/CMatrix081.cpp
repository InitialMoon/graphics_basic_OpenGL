#include "CMatrix081.h"
#include "CVector081.h"
#include "CEuler081.h"
#include "CQuaternion081.h"
#include <cmath>
#include <string>

void CMatrix081::Set(float* p)
{
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
}

CMatrix081::CMatrix081(float* p)
{
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
}

CMatrix081& CMatrix081::operator=(const CMatrix081& p)
{
	CMatrix081 pm = p;
	for (int i = 0; i < 16; i++) {
		(*this)[i] = pm[i];
	}
	return pm;
}

CMatrix081 CMatrix081::operator*(float d)
{
	CMatrix081 m;
	for (int i = 0; i < 16; i++) {
		m[i] = (*this)[i] * d;
	}
	return m;
}

CMatrix081 CMatrix081::operator*(CMatrix081& p)
{
	CMatrix081 m;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int pos = j * 4 + i;
			float val = 0;
			for (int k = 0; k < 4; k++) {
				val = val + ((* this)[k * 4 + i] * p[j * 4 + k]);
			}
			m[pos] = val;
		}
	}
	return m;
}

CVector081 CMatrix081::vecMul(CVector081& p)
{
	CVector081 v;
	for (int i = 0; i < 3; i++) {
		float val = 0;
		for (int j = 0; j < 3; j++) {
			val += (*this)[j * 4 + i] * p[j];
		}
		v[i] = val;
	}
	return v;
}

CVector081 CMatrix081::posMul(CVector081& p)
{
	CVector081 v = CVector081();
	for (int i = 0; i < 3; i++) {
		float val = 0;
		for (int j = 0; j < 3; j++) {
			val += (*this)[j * 4 + i] * p[j];
		}
		v[i] = val + (*this)[12 + i];
	}
	return v;
}

CEuler081 CMatrix081::ToEuler()
{
	CEuler081 e = CEuler081();
	e.p = -asin(m12) * 180 / PI;
	e.h = atan2(m02, m22) * 180 / PI;
	e.b = atan2(m10, m11) * 180 / PI;
	return e;
}

CQuaternion081 CMatrix081::ToQuaternion()
{
	CQuaternion081 q = CQuaternion081();
	q.w = sqrt(m00 + m11 + m22 + 1) / 2;
	char max_name = 'w';
	float max_one = q.w;
	q.x = sqrt(m00 - m11 - m22 + 1) / 2;
	if (q.x > max_one) {
		max_one = q.x;
		max_name = 'y';
	}
	q.y = sqrt(-m00 + m11 - m22 + 1) / 2;
	if (q.y > max_one) {
		max_one = q.y;
		max_name = 'y';
	}
	q.z = sqrt(-m00 - m11 + m22 + 1) / 2;
	if (q.z > max_one) {
		max_one = q.z;
		max_name = 'z';
	}
	switch (max_name) {
	case 'w':
		q.x = (m21 - m12) / (4 * q.w);
		q.y = (m02 - m20) / (4 * q.w);
		q.z = (m10 - m01) / (4 * q.w);
		break;
	case 'x':
		q.w = (m21 - m12) / (4 * q.x);
		q.y = (m10 + m01) / (4 * q.x);
		q.z = (m02 - m20) / (4 * q.x);
		break;
	case 'y':
		q.w = (m02 - m10) / (4 * q.y);
		q.x = (m10 + m01) / (4 * q.y);
		q.z = (m21 + m12) / (4 * q.y);
		break;
	case 'z':
		q.w = (m10 - m01) / (4 * q.z);
		q.x = (m02 + m20) / (4 * q.z);
		q.y = (m21 + m12) / (4 * q.z);
		break;
	}
	return q;
}

void CMatrix081::SetRotate(float seta, CVector081 axis)
{
	axis = axis.Normalize();
	seta = (seta / 180) * PI;
	this->m00 = axis.x * axis.x * (1 - cos(seta)) + cos(seta);
	this->m01 = axis.x * axis.y * (1 - cos(seta)) - axis.z * sin(seta);
	this->m02 = axis.x * axis.z * (1 - cos(seta)) + axis.y * sin(seta);
	this->m03 = 0;

	this->m10 = axis.x * axis.y * (1 - cos(seta)) + axis.z * sin(seta);
	this->m11 = axis.y * axis.y * (1 - cos(seta)) + cos(seta);
	this->m12 = axis.y * axis.z * (1 - cos(seta)) - axis.x * sin(seta);
	this->m13 = 0;

	this->m20 = axis.x * axis.z * (1 - cos(seta)) - axis.y * sin(seta);
	this->m21 = axis.y * axis.z * (1 - cos(seta)) + axis.x * sin(seta);
	this->m22 = axis.z * axis.z * (1 - cos(seta)) + cos(seta);
	this->m23 = 0;

	this->m30 = 0;
	this->m31 = 0;
	this->m32 = 0;
	this->m33 = 1;
}

void CMatrix081::SetTrans(CVector081 trans)
{
	this->m00 = 1;
	this->m10 = 0;
	this->m20 = 0;
	this->m30 = 0;

	this->m01 = 0;
	this->m11 = 1;
	this->m21 = 0;
	this->m31 = 0;

	this->m02 = 0;
	this->m12 = 0;
	this->m22 = 1;
	this->m32 = 0;

	this->m03 = trans.x;
	this->m13 = trans.y;
	this->m23 = trans.z;
	this->m33 = 1;
}

void CMatrix081::SetScale(CVector081 p)
{
	for (int i = 0; i < 16; i++) {
		(*this)[i] = 0;
	}
	this->m00 = p.x;
	this->m11 = p.y;
	this->m22 = p.z;
	this->m33 = 1;

}

float cal(float** det, int n)//det-行列式，n:行列式的阶数
{
	float detVal = 0;//行列式的值

	if (n == 1)//递归终止条件 
		return det[0][0];

	float** tempdet = new float* [n - 1];//用来存储余相应的余子式
	for (int i = 0; i < n - 1; i++)
		tempdet[i] = new float[n - 1];
	for (int i = 0; i < n; i++)//第一重循环，行列式按第一行展开 
	{
		for (int j = 0; j < n - 1; j++)
			for (int k = 0; k < n - 1; k++)
			{
				if (k < i)
					tempdet[j][k] = det[j + 1][k];
				else
					tempdet[j][k] = det[j + 1][k + 1];
			}
		detVal += det[0][i] * pow(-1.0, i) * cal(tempdet, n - 1);

	}
	return detVal;
}

float CMatrix081::Inverse()
{
	//申请：
	float** Matrix = new float* [4];
	for (int i = 0; i < 4; i++) Matrix[i] = new float[4];
	//释放：

	for (int i = 0; i < 4; i++) {
		float p[4];
		for (int j = 0; j < 4; j++) {
			Matrix[i][j] = (*this)[j * 4 + i];
		}
	}

	float res = 0;
	res = cal(Matrix, 4);

	for (int i = 0; i < 4; i++)
		delete[] Matrix[i];//先释放指针数组中的一级指针
	delete[] Matrix;//在释放二级指针
	return res;
}

float dat3D(float m[3][3]) {
	float res =  m[0][0] * m[1][1] * m[2][2]
		+ m[0][1] * m[1][2] * m[2][0]
		+ m[0][2] * m[1][0] * m[2][1]
		- m[0][2] * m[1][1] * m[2][0]
		- m[0][1] * m[1][0] * m[2][2]
		- m[0][0] * m[1][2] * m[2][1];
	return res;
}

CMatrix081 CMatrix081::GetInverse()
{
	float im[4][4] = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			im[i][j] = (*this)[j * 4 + i];
		}
	}
	float adjm[4][4] = { 0 };
	float p[16] = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float mm[3][3] = { 0 };
			int a = 0; 
			for (int n = 0; n < 4; n++) {	
				int b = 0;
				if (n != i) {
					for (int m = 0; m < 4; m++) {
						if (m != j) {
							mm[a][b] = im[n][m];
							b++;
						}
					}
					a++;
				}
			}
			adjm[i][j] = dat3D(mm);
			p[i * 4 + j] = adjm[i][j] / this->Inverse();
		}
	}
	CMatrix081 res = CMatrix081();
	res.Set(p);
	for (int i = 0; i < 16; i++) {
		if ((i / 4 == 0 || i / 4 == 2) && i % 2) {
			res[i] = -res[i];
		}
		if ((i / 4 == 1 || i / 4 == 3) && !(i % 2)) {
			res[i] = -res[i];
		}
		if (abs(res[i]) < 10e-5) {
			res[i] = 0;
		}
	}
	return res;
}

