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
	void Normal();//h,b�Ƕ���-180~180��p�Ƕ���-90~90��������ʱ,h��������b�Ķ�����ŷ���ǹ淶��void 
	CVector081 ToVector3();// ŷ����ת��Ϊ����CVector3
	CMatrix081 ToMatrix(); //ŷ����ת��Ϊ����CMarix  
	CQuaternion081 ToQuaternion();// ŷ����ת��Ϊ��Ԫ��CQuaternion
	CEuler081() { h = 0; p = 0; b = 0; };
	CEuler081(float hh, float pp, float bb) { h = hh; p = pp; b = bb; };
};