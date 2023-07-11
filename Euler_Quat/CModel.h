#pragma once
class CVector081;
class CMatrix081;
class CQuaternion081;
class CEuler081;

// ������д洢��ģ�͵ĸ���λ����Ϣ��������Ϣ�����ڻ��Ƶ�ͼ�ζ��ֶ��֣�
// Ҫͳһ��������Ҫ�����࣬������switch��ѡ����������鷳��ģ�͵������С
class CModel{
public:
	// ��ģ�����в��洢ģ�͵ľ���λ�ã���Ҫ����λ��ʱ����÷���getAbsPos();
	CVector081 pos; // �����Լ�������ϵ�е��˶�λ��
	CMatrix081 parentMatrix; // ������
	CVector081 getAbsPos();
	CVector081 size; // ����ߵĴ洢
	GLuint texture[1] = { 0 }; //��Ҫ�����ģ�ʹ洢����ı���
	float self_rotate_theta; // �����ǻ�����ֵ
	CVector081 self_rotate_axis;
	//void* originShape; // ģ�͵Ļ�����״���ƺ�����ͳһ��������������ƾͿ����ֺϲ����ˣ������渳�費ͬ�Ļ��ƺ���
	float color[3] = { 1.0, 1.0, 1.0 }; // ģ�ͱ������ɫ��ע�⻰�����ʱ��Ҫʹ�ã���Ϊ��ƴ�ӵ�
	void setColor(float r, float g, float b);
	void setSize(float x, float y, float z);
	void Draw(int mode, bool on_xyz);
	CModel(){};
	CModel(char* img_path);
	// ��������
	int LoadGLTextures(char* img_path);
	void DrawBox();
};
