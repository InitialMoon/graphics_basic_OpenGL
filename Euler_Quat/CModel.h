#pragma once
class CVector081;
class CMatrix081;
class CQuaternion081;
class CEuler081;

// 这个类中存储了模型的各种位置信息，朝向信息，由于绘制的图形多种多种，
// 要统一在类中需要抽象类，或者用switch做选项，反而会变得麻烦，模型的整体大小
class CModel{
public:
	// 该模型类中不存储模型的绝对位置，需要绝对位置时请调用方法getAbsPos();
	CVector081 pos; // 是在自己的坐标系中的运动位置
	CMatrix081 parentMatrix; // 父矩阵
	CVector081 getAbsPos();
	CVector081 size; // 长宽高的存储
	GLuint texture[1] = { 0 }; //需要纹理的模型存储纹理的变量
	float self_rotate_theta; // 这里是弧度制值
	CVector081 self_rotate_axis;
	//void* originShape; // 模型的基本形状绘制函数，统一调用这个函数绘制就可以弥合差异了，在外面赋予不同的绘制函数
	float color[3] = { 1.0, 1.0, 1.0 }; // 模型本身的颜色，注意话火箭的时候不要使用，因为是拼接的
	void setColor(float r, float g, float b);
	void setSize(float x, float y, float z);
	void Draw(int mode, bool on_xyz);
	CModel(){};
	CModel(char* img_path);
	// 辅助函数
	int LoadGLTextures(char* img_path);
	void DrawBox();
};
