#pragma once
class CVector081;
class CEuler081;
class CQuaternion081;
#define PI 3.14159265358
class CMatrix081 {
public:
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;
	operator float* () { return &m00; }
	void Set(float* p);	//给矩阵16个元素赋值
	CMatrix081& operator=(const CMatrix081& p);//矩阵赋值
	CMatrix081 operator*(float d);//矩阵数乘
	CMatrix081 operator*(CMatrix081& p);//矩阵相乘
	CVector081 vecMul(CVector081& p); //矩阵与向量相乘
	CVector081 posMul(CVector081& p);//矩阵与位置相乘
	CEuler081 ToEuler();// 矩阵转换为欧拉角
	CQuaternion081 ToQuaternion();// 矩阵转换为四元数
	void SetRotate(float seta, CVector081 axis);	//设置为旋转矩阵
	void SetTrans(CVector081 trans);		//设置为平移矩阵
	void SetScale(CVector081 p);		//设置为缩放矩阵
	float Inverse();//矩阵求逆,成功返回行列式的值，否则返回0
	CMatrix081 GetInverse();//返回逆矩阵
	CMatrix081() {};
	CMatrix081(float* p);
};
