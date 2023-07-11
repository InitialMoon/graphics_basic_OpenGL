#pragma once
class CMatrix081;
class CEuler081;
class CQuaternion081;

class CVector081 {
	public:
		float x;
		float y;
		float z;
		void Set(float x, float y, float z);
		CVector081();
		CVector081(float X, float Y, float Z);
		CVector081(const CVector081& vec);
		CVector081 operator+(const CVector081& p); // 其他的也要加const，不然没法用
		CVector081 operator-(const CVector081& p);
		CVector081 operator*(const float p);
		CVector081 crossMul(CVector081& p);
		CEuler081 ToEuler();
		//void operator=(CVector081& p);
		//void operator=(CVector081* p);
		CVector081& operator=(const CVector081& p);
		//CVector081 operator=(CVector081& p);
		bool operator==(CVector081& p);
		bool operator!=(CVector081& p);
		float dotMul(CVector081& n);
		CVector081 Normalize();
		float len();
		CVector081 project(CVector081& n);
		operator float* () { return &x; }	
		CVector081 Slerp(CVector081& Vend, float t); //插值。从当前四元数插值到Vend四元数,t是参数[0,1]
		void Slerp(CVector081& Vend, int n, float* t, CVector081* Result);//插值。一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中。
};
