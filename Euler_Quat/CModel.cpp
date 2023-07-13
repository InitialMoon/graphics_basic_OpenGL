#include "CEuler081.h"
#include "CVector081.h"
#include "CMatrix081.h"
#include "CQuaternion081.h"
#include <cmath>
#include <atlimage.h>
#include "glew.h"
#include "glut.h"
#include "CModel.h"
#include <iostream>

// 常用的轴
CVector081 axis_x = CVector081(1, 0, 0);
CVector081 axis_y = CVector081(0, 1, 0);
CVector081 axis_z = CVector081(0, 0, 1);

int CModel::LoadGLTextures(char* img_path)								// Load Bitmaps And Convert To Textures
{
	CImage img;
	HRESULT hResult = img.Load(img_path);
	if (FAILED(hResult))
	{
		return 0;
	}
	glGenTextures(1, &texture[0]);					// Create The Texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Generate The Texture
	int pitch = img.GetPitch();
	if (pitch < 0)
		gluBuild2DMipmaps(GL_TEXTURE_2D, img.GetBPP() / 8, img.GetWidth(), img.GetHeight(), GL_BGR, GL_UNSIGNED_BYTE, img.GetPixelAddress(0, img.GetHeight() - 1));
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, img.GetBPP() / 8, img.GetWidth(), img.GetHeight(), GL_BGR, GL_UNSIGNED_BYTE, img.GetBits());
	//filter 参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Linear Filteringd
	//glDisable(GL_CULL_FACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//颜色直接相乘
	return TRUE;
}

// 画单位盒
void CModel::DrawBox()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor3f(1, 1, 1);
	//glScaled(x, y, z);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3fv(axis_z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Texture and Quad

	// Back Face
	glNormal3fv(axis_z * -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
	// Top Face
	glNormal3fv(axis_y);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);	// Top Right Of The Texture and Quad
	// Bottom Face
	glNormal3fv(axis_y * -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);	// Bottom Right Of The Texture and Quad
	// Right face
	glNormal3fv(axis_x);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	// Bottom Left Of The Texture and Quad
	// Left Face
	glNormal3fv(axis_x * -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

CModel::CModel(char* img_path) {
	setSize(1, 1, 1);
	if (img_path != NULL) {
		LoadGLTextures(img_path);
	}
}

void CModel::setColor(float r, float g, float b) {
	color[0] = r / 255.0;
	color[1] = g / 255.0;
	color[2] = b / 255.0;
}

void CModel::setSize(float x, float y, float z)
{
	size.x = x;
	size.y = y;
	size.z = z;
}

void CModel::setSize(float r)
{
	size.x = r;
	size.y = r;
	size.z = r;
}

CVector081 CModel::getAbsPos()
{
	if (parentModel != NULL) {
		CMatrix081 transM = CMatrix081();
		transM.SetTrans(parentModel->getAbsPos());
		return transM.posMul(pos);
	}
	else {
		return pos;
	}
}

void CModel::Draw(int mode)
{
	glPushMatrix();
	CVector081 abs_pos = getAbsPos();

	// 注意这里的移动缩放和旋转的次序，不能混乱，不然都没法生效了，之后研究为啥
	glTranslatef(abs_pos.x, abs_pos.y, abs_pos.z);
	glScaled(size.x, size.y, size.z);
	glRotatef(self_rotate_theta / 20 * 180 / 3.14, self_rotate_axis.x, self_rotate_axis.y, self_rotate_axis.z);

	switch (mode)
	{
	case 0: // 不带贴图的球
		glColor3f(color[0], color[1], color[2]);
		gluSphere(gluNewQuadric(), 1, min(size.x * 10.0, 100.0), min(size.x * 10.0, 100.0));
		break;
	case 1: // 带贴图的球
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glPolygonMode(GL_BACK, GL_FILL);
		glEnable(GL_TEXTURE_GEN_T);
		glColor3f(1, 1, 1);
		gluSphere(gluNewQuadric(), 1, min(size.x * 10.0, 100.0), min(size.x * 10.0, 100.0));
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
		break;
	case 2: // 贴图立方体
		DrawBox();
		break;
	case 3: // 画火箭上半部分，零点坐标高度在圆柱体的底部，方便降落时
		//火箭头
		glTranslatef(0, 0.5, 0);
		glPushMatrix();
		glColor3f(0.5, 0, 0);
		glTranslatef(0, 0.45, 0);
		glRotatef(-90, 1, 0, 0); // 是用自己的旋转函数实现旋转
		glutSolidCone(0.1, 0.3, 10, 10);
		glPopMatrix();
		//火箭身
		glPushMatrix();
		glColor3f(119 / 255.0, 137 / 255.0, 173 / 255.0);
		glScalef(0.1, 1, 0.1);
		glRotatef(-90, 1, 0, 0);
		glutSolidTorus(0.5, 0.7, 10, 10);
		glPopMatrix();
		break;
	case 4: // 画火箭下半部分,零点坐标高度在圆柱体的头部
		glColor3f(53 / 255.0, 42 / 255.0, 85 / 255.0);
		glTranslatef(0, -0.7, 0);
		glScalef(0.15, 1.5, 0.15);
		glRotatef(-90, 1, 0, 0);
		glutSolidTorus(0.5, 0.7, 10, 10);
		break;
	case 5: // 画坐标系
	{
		float r = 0.1;
		glBegin(GL_LINES);
		glLineWidth(30);

		//x
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(r, 0, 0);
		glVertex3f(r, 0, 0);
		glVertex3f(r * 0.8, 0, r * 0.2);
		glVertex3f(r, 0, 0);
		glVertex3f(r * 0.8, 0, -r * 0.2);

		//y
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, r, 0);
		glVertex3f(0, r, 0);
		glVertex3f(0, r * 0.8, r * 0.2);
		glVertex3f(0, r, 0);
		glVertex3f(0, r, -r * 0.2);

		//z
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, r);
		glVertex3f(0, 0, r);
		glVertex3f(r * 0.2, 0, r * 0.8);
		glVertex3f(0, 0, r);
		glVertex3f(-r * 0.2, 0, r * 0.8);

		glEnd();
	}
	break;
	default:
		std::cout << "error! please select right mode from 0-3" << std::endl;
		break;
	}
	glTranslatef(-abs_pos.x, -abs_pos.y, -abs_pos.z);
	glPopMatrix();
}

CModel::CModel()
{
	parentModel = NULL;
	R = 1;
	self_rotate_axis = CVector081();
	self_rotate_theta = 0;
}
