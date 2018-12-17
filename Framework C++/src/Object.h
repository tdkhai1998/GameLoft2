#pragma once

struct Point {
	int x,y;
};
class Object
{
public:
	int m_iposX=20;
	int m_iposY=20;
	int m_imass;
	float m_fvelocity;
public:
	Object();
	void Render();
	void Update();
	~Object();
};

