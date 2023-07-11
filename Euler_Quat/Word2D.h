#pragma once
class Word2D {
public:
	std::string content;
	Word2D(std::string s);
	Word2D(){};
	void draw(double x, double y, int size);
};
