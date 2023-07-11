#include <fstream>
#include <iostream>
#include<iomanip>
#include <regex>
#include <map>
#include "CVector081.h"
#include "CMatrix081.h"
#include "miniEngine.h"
#include "CEuler081.h"
#include "CQuaternion081.h"

using namespace std;

void test_out();// ��ɵ�һ������ȡ����������������������out.txt

int main(int argc, char* argv[]) {
	//test_out();
	glMain(argc, argv);
}

void Stringsplit(const string& str, const string& split, vector<string>& res)
{
	std::regex reg(split);		// ƥ��split
	std::sregex_token_iterator pos(str.begin(), str.end(), reg, -1);
	decltype(pos) end;              // �Զ��Ƶ����� 
	for (; pos != end; ++pos)
	{
		res.push_back(pos->str());
	}
}

vector<float> string_to_vector(string s) {
	vector<string> str_vals;
	vector<float> f_vals;
	Stringsplit(s, ",", str_vals);
	for (auto sv : str_vals) {
		f_vals.push_back(stof(sv));
	}
	return f_vals;
}

CQuaternion081 qStringConstructor(string s) {
	vector<float> val = string_to_vector(s);
	CQuaternion081 cq;
	cq.Set(val.at(0), val.at(1), val.at(2), val.at(3));
	return cq;
}
	
CEuler081 eStringConstructor(string s) {
	vector<float> val = string_to_vector(s);
	CEuler081 ce;
	ce.Set(val.at(0), val.at(1), val.at(2));
	return ce;
}

CMatrix081 mStringConstructor(string s) {
	vector<float> val = string_to_vector(s);
	float array_val[16];
	for (int i = 0; i < 16; i++) {
		array_val[i] = val.at(i);
	}
	CMatrix081 cm;
	cm.Set(array_val);
	return cm;
}

CVector081 vStringConstructor(string s) {
	vector<float> val = string_to_vector(s);
	float array_val[3];
	for (int i = 0; i < 3; i++) {
		array_val[i] = val.at(i);
	}
	CVector081 cv(array_val[0], array_val[1], array_val[2]);
	return cv;
}

void test_out() {
	map<string, int> TYPE = { //map�����const���Σ���ô��û����[]������
			{"ŷ����ת������", 1},
			{"����ת��ŷ����", 2},
			{"ŷ����ת����Ԫ��", 3},
			{"��Ԫ��ת��ŷ����", 4},
			{"ŷ����ת������", 5},
			{"����ת��ŷ����", 6},
			{"����ת����Ԫ��", 7},
			{"��Ԫ��ת������", 8},
			{"ŷ���Ǳ�׼��", 9},
			{"��Ԫ����λ��", 10},
			{"��Ԫ�����", 11},
			{"��Ԫ�����", 12},
			{"��Ԫ������", 13},
			{"��Ԫ����ǶȺ���ת��", 14},
			{"��Ԫ�����", 15},
			{"��Ԫ����ֵ", 16}
	};
	ifstream fin;
	ofstream fout;
	fin.open("test.txt", ios::in);
	fout.open("out.txt");
	if (!fin.is_open()) {
		cout << "cannot open the file!";
	}
	else {
		string line;
		while (fin >> line) {// ���һ�ζ�һ��
			cout << setprecision(2);
			fout << fixed << setprecision(2);
			fout << fixed << line;
			fout << fixed << "\n";
			switch (TYPE[line]) {
			case 1:
			{
				string sm1;
				fin >> sm1;
				CEuler081 e1 = eStringConstructor(sm1);;
				CVector081 m = e1.ToVector3();
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e1[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << m[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 2:
			{
				string sm1;
				fin >> sm1;
				CVector081 m = vStringConstructor(sm1);;
				CEuler081 e = m.ToEuler();
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << m[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 3:
			{
				string sm1;
				fin >> sm1;
				CEuler081 e = eStringConstructor(sm1);
				CQuaternion081 q = e.ToQuaternion();
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 4:
			{
				string sm1;
				fin >> sm1;
				CQuaternion081 q = qStringConstructor(sm1);
				CEuler081 e = q.ToEuler();
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 5:
			{
				string sm1;
				fin >> sm1;
				CEuler081 e = eStringConstructor(sm1);
				CMatrix081 m = e.ToMatrix();
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e[i];
				}
				fout << fixed << "\t";
				for (int i = 0; i < 16; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << m[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 6:
			{
				string sm1;
				fin >> sm1;
				CMatrix081 m = mStringConstructor(sm1);
				CEuler081 e = m.ToEuler();
				for (int i = 0; i < 16; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << m[i];
				}
				fout << fixed << "\t";
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 7:
			{
				string sm1;
				fin >> sm1;
				CMatrix081 m = mStringConstructor(sm1);
				CQuaternion081 q = m.ToQuaternion();
				for (int i = 0; i < 16; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << m[i];
				}
				fout << fixed << "\t";
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 8:
			{
				string sm1;
				fin >> sm1;
				CQuaternion081 q = qStringConstructor(sm1);
				CMatrix081 m = q.ToMatrix();
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 16; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << m[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 9:
			{
				string sm1;
				fin >> sm1;
				CEuler081 e = eStringConstructor(sm1);
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e[i];
				}
				fout << fixed << '\t';
				e.Normal();
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << e[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 10:
			{
				string sm1;
				fin >> sm1;
				CQuaternion081 q = qStringConstructor(sm1);
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\t';
				q.Normalize();
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 11:
			{
				string sm1, sm2;
				fin >> sm1 >> sm2;
				// ����������ķ��ں��棬���ף�����˵����˳��Ҳ�����ϳ�ʶ
				CQuaternion081 q1 = qStringConstructor(sm1);
				CQuaternion081 q2 = qStringConstructor(sm2);
				CQuaternion081 q3 = q2*q1;
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q1[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q2[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q3[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 12:
			{
				string sm1, sm2;
				fin >> sm1 >> sm2;
				CQuaternion081 q1 = qStringConstructor(sm1);
				CQuaternion081 q2 = qStringConstructor(sm2);
				float res = q1.dotMul(q2);
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q1[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q2[i];
				}
				fout << fixed << '\t';
				fout << fixed << res;
				fout << fixed << '\n';
				break;
			}
			case 13:
			{
				string sm1;
				fin >> sm1;
				CQuaternion081 q = qStringConstructor(sm1);
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\t';
				q.Inverse();
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 14:
			{
				string sm1;
				fin >> sm1;
				CQuaternion081 q = qStringConstructor(sm1);
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q[i];
				}
				fout << fixed << '\t';
				float angle = 0;
				CVector081 axis = CVector081();
				q.Normalize();
				q.GetAngle(angle, axis);
				fout << fixed << angle;
				fout << fixed << '\t';
				for (int i = 0; i < 3; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << axis[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 15:
			{
				string sm1, sm2;
				fin >> sm1 >> sm2;
				CQuaternion081 q1 = qStringConstructor(sm1);
				CQuaternion081 q2 = qStringConstructor(sm2);
				CQuaternion081 q3 = q2.Div(q1);
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q1[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q2[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q3[i];
				}
				fout << fixed << '\n';
				break;
			}
			case 16:
			{
				string s1, s2, s3;
				fin >> s1 >> s2 >> s3;
				CQuaternion081 q1 = qStringConstructor(s1);
				CQuaternion081 q2 = qStringConstructor(s2);
				float t = stof(s3);
				CQuaternion081 q3 = q1.Slerp(q2, t);
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q1[i];
				}
				fout << fixed << '\t';
				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q2[i];
				}
				fout << fixed << '\t';
				fout << fixed << t;
				fout << fixed << '\t';

				for (int i = 0; i < 4; i++) {
					if (i != 0) fout << fixed << ",";
					fout << fixed << q3[i];
				}
				fout << fixed << '\n';
				break;
			}
			}
		}
	}
	fin.close();
	fout.close();
}

