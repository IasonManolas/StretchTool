#pragma once
#include <vector>
#include "AnchorPoint.h"
#include <iostream>
#include <glut.h> 

class AnchorList
{
public:
	std::vector<AnchorPoint> anchorList;
	float radius;
	void add(AnchorPoint);
	void clear();
	void doubleRadius();
	void halfRadius();
	bool Contains(int intersectionIndex);
	void draw();
	AnchorList();
	~AnchorList();
};

