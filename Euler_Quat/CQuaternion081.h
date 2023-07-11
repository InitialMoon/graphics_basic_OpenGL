#pragma once
class CVector081;
class CMatrix081;
class CEuler081;

class CQuaternion081
{
public:
	float x, y, z, w;
	void Set(float fx, float fy, float fz, float fw);
	operator float* () { return &x; }
	CEuler081 ToEuler(); // 四元数转换为欧拉角CEuler 
	CMatrix081 ToMatrix(); // 四元数转换为矩阵CMarix 
	void SetAngle(float angle, CVector081 axis); //四元数设置
	CQuaternion081& operator=(const CQuaternion081& p); //重载赋值
	CQuaternion081 operator+(const CQuaternion081& p); //重载‘+’
	CQuaternion081 operator*(float data); //重载数乘
	CQuaternion081 operator*(const CQuaternion081& p); //四元数乘法
	float dotMul(const CQuaternion081& p); //点乘
	float len(); //求模
	bool Normalize();	//求单位化
	CQuaternion081& Inverse();//求逆四元数,会改变自身。
	CQuaternion081 GetInverse();//求逆四元数,不改变自身，生成新的四元数
	CQuaternion081 Div(const CQuaternion081& b); //求差 当前为a,求c=a-b
	void GetAngle(float& angle, CVector081& axis); //求旋转轴和角度
	CQuaternion081 Slerp(const CQuaternion081& Vend, float t); //插值。从当前四元数插值到Vend四元数,t是参数[0,1]
	void Slerp(const CQuaternion081& Vend, int n, float* t, CQuaternion081* Result);//插值。一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中。
};

