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
	CEuler081 ToEuler(); // ��Ԫ��ת��Ϊŷ����CEuler 
	CMatrix081 ToMatrix(); // ��Ԫ��ת��Ϊ����CMarix 
	void SetAngle(float angle, CVector081 axis); //��Ԫ������
	CQuaternion081& operator=(const CQuaternion081& p); //���ظ�ֵ
	CQuaternion081 operator+(const CQuaternion081& p); //���ء�+��
	CQuaternion081 operator*(float data); //��������
	CQuaternion081 operator*(const CQuaternion081& p); //��Ԫ���˷�
	float dotMul(const CQuaternion081& p); //���
	float len(); //��ģ
	bool Normalize();	//��λ��
	CQuaternion081& Inverse();//������Ԫ��,��ı�����
	CQuaternion081 GetInverse();//������Ԫ��,���ı����������µ���Ԫ��
	CQuaternion081 Div(const CQuaternion081& b); //��� ��ǰΪa,��c=a-b
	void GetAngle(float& angle, CVector081& axis); //����ת��ͽǶ�
	CQuaternion081 Slerp(const CQuaternion081& Vend, float t); //��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
	void Slerp(const CQuaternion081& Vend, int n, float* t, CQuaternion081* Result);//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result�С�
};

