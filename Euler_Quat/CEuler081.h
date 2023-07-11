#pragma once
class CVector081;
class CMatrix081;
class CQuaternion081;
class CEuler081
{
public:
	float h, p, b;
	void Set(float fh, float fp, float fb);
	operator float* () { return &h; }
	void Normal();//h,b角度在-180~180，p角度在-90~90。万向锁时,h代替所有b的度数。欧拉角规范化void 
	CVector081 ToVector3();// 欧拉角转换为向量CVector3
	CMatrix081 ToMatrix(); //欧拉角转换为矩阵CMarix  
	CQuaternion081 ToQuaternion();// 欧拉角转换为四元数CQuaternion
	CEuler081() { h = 0; p = 0; b = 0; };
	CEuler081(float hh, float pp, float bb) { h = hh; p = pp; b = bb; };
};