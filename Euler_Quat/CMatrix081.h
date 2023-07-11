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
	void Set(float* p);	//������16��Ԫ�ظ�ֵ
	CMatrix081& operator=(const CMatrix081& p);//����ֵ
	CMatrix081 operator*(float d);//��������
	CMatrix081 operator*(CMatrix081& p);//�������
	CVector081 vecMul(CVector081& p); //�������������
	CVector081 posMul(CVector081& p);//������λ�����
	CEuler081 ToEuler();// ����ת��Ϊŷ����
	CQuaternion081 ToQuaternion();// ����ת��Ϊ��Ԫ��
	void SetRotate(float seta, CVector081 axis);	//����Ϊ��ת����
	void SetTrans(CVector081 trans);		//����Ϊƽ�ƾ���
	void SetScale(CVector081 p);		//����Ϊ���ž���
	float Inverse();//��������,�ɹ���������ʽ��ֵ�����򷵻�0
	CMatrix081 GetInverse();//���������
	CMatrix081() {};
	CMatrix081(float* p);
};
