#pragma once
class Point3f
{
public:
	Point3f(void);
	~Point3f(void);
	Point3f(float, float, float);

	float x, y, z;
};

class Point2f
{
public:
	Point2f(void);
	~Point2f(void);
	Point2f(float, float);

	float x, y;
};