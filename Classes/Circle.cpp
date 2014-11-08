#include "Circle.h"

USING_NS_CC;

const Circle Circle::UNIT_CIRCLE(Vec2::ZERO, 1.0f);

Circle::Circle(void)
	: center(0.0f,0.0f)
	, radius(0.0f)
{
}

Circle::Circle(const Vec2 &c, float r)
	: center(c)
	, radius(r)
{
}

Circle::~Circle(void)
{
}

bool Circle::equals(const Circle& circle) const
{
	return ((radius==circle.radius)&&(center==circle.center) ? true : false);
}

bool Circle::containsPoint(const Vec2& point) const
{
	return ((center.distance(point)<=radius) ? true : false);
}

bool Circle::intersectsRect(const Rect& rect) const
{
	return (containsPoint(Vec2(rect.getMaxX(),rect.getMaxY()))||
		containsPoint(Vec2(rect.getMinX(),rect.getMaxY()))||
		containsPoint(Vec2(rect.getMaxX(),rect.getMinY()))||
		containsPoint(Vec2(rect.getMinX(),rect.getMinY())));
}

bool Circle::intersectsCircle(const Circle& circle) const
{
	return ((center.distance(circle.center)<=(radius+circle.radius)) ? true : false);
}
