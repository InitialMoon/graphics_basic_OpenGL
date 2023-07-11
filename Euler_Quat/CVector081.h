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
		CVector081 operator+(const CVector081& p); // ������ҲҪ��const����Ȼû����
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
		CVector081 Slerp(CVector081& Vend, float t); //��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
		void Slerp(CVector081& Vend, int n, float* t, CVector081* Result);//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result�С�
};
