#pragma once
class CVector081;
class CMatrix081;
class CQuaternion081;
class CEuler081;

class CModel{
public:
	// ��ģ�����в��洢ģ�͵ľ���λ�ã���Ҫ����λ��ʱ����÷���getAbsPos();
	CVector081 pos; // �����Լ�������ϵ�е��˶�λ��
	CModel* parentModel; // ������
	CVector081 getAbsPos();
	CVector081 size; // ����ߵĴ洢
	GLuint texture[1] = { 0 }; //��Ҫ�����ģ�ʹ洢����ı���
	CEuler081 self_rotate; // �����ǽǶ���ֵ
	float R; // ���������ϵͳ�������а뾶ΪR��Բ���˶��Ļ�����¼��뾶
	float color[3] = { 1.0, 1.0, 1.0 }; // ģ�ͱ������ɫ��ע�⻰�����ʱ��Ҫʹ�ã���Ϊ��ƴ�ӵ�
	void setColor(float r, float g, float b);
	void setSize(float x, float y, float z);
	void setSize(float r);
	void Draw(int mode);
	CModel();
	CModel(char* img_path);
	// ��������
	int LoadGLTextures(char* img_path);
	void DrawBox();
};
