#pragma once
class CVector081;
class CMatrix081;
class CQuaternion081;
class CEuler081;

class CModel{
public:
	// 该模型类中不存储模型的绝对位置，需要绝对位置时请调用方法getAbsPos();
	CVector081 pos; // 是在自己的坐标系中的运动位置
	CModel* parentModel; // 父矩阵
	CVector081 getAbsPos();
	CVector081 size; // 长宽高的存储
	GLuint texture[1] = { 0 }; //需要纹理的模型存储纹理的变量
	float self_rotate_theta{0}; // 这里是弧度制值
	float R; // 如果是在子系统中做绕行半径为R的圆周运动的话，记录其半径
	CVector081 self_rotate_axis;
	//void* originShape; // 模型的基本形状绘制函数，统一调用这个函数绘制就可以弥合差异了，在外面赋予不同的绘制函数
	float color[3] = { 1.0, 1.0, 1.0 }; // 模型本身的颜色，注意话火箭的时候不要使用，因为是拼接的
	void setColor(float r, float g, float b);
	void setSize(float x, float y, float z);
	void setSize(float r);
	void Draw(int mode);
	CModel();
	CModel(char* img_path);
	// 辅助函数
	int LoadGLTextures(char* img_path);
	void DrawBox();
};
