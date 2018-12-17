#pragma once

class Object
{
	
public:
	int m_iposX = 0;
	int m_iposY;
	int m_imass;
	float m_fvelocity;
	Object();
	void Render();
	void Update();
	~Object();
};

