#ifndef __HOUND_CIRCLE_H__
#define __HOUND_CIRCLE_H__

#include "cocos2d.h"

class Circle
{
public:
	cocos2d::Vec2 center;
	float radius;

public:
	Circle(void);
	Circle(const cocos2d::Vec2 &c, float r);
	~Circle(void);

	bool equals(const Circle& circle) const;

	bool containsPoint(const cocos2d::Vec2& point) const;
    
	bool intersectsRect(const cocos2d::Rect& rect) const;

	bool intersectsCircle(const Circle& circle) const;

	static const Circle UNIT_CIRCLE;
};

#endif //__HOUND_CIRCLE_H__