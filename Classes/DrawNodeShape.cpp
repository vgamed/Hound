#include "DrawNodeShape.h"

USING_NS_CC;

DrawNodeShape::DrawNodeShape(void)
	: m_showUp(true)
{
}


DrawNodeShape::~DrawNodeShape(void)
{
}

DrawNodeShape* DrawNodeShape::create(SHAPE_TYPE type,
									 const Vec2 &pos, 
									 float radius, 
									 const Color4F &color)
{
	DrawNodeShape* box = new DrawNodeShape();
	if (box && box->init(type, pos, radius, color))
		box->autorelease();
	else
		CC_SAFE_DELETE(box);

	return box;
}

bool DrawNodeShape::init(SHAPE_TYPE type,
						 const Vec2 &pos, 
						 float radius, 
						 const Color4F &color)
{
	if (!DrawNode::init())
		return false;

	setCenterPosition(pos);
	setRadius(radius);
	setLineColor(color);

	switch(type)
	{
	case SHAPE_TYPE::BOX:
		m_drawFunc = CC_CALLBACK_2(DrawNodeShape::drawBox, this);
		break;
	case SHAPE_TYPE::CIRCLE:
		m_drawFunc = CC_CALLBACK_2(DrawNodeShape::drawCircle, this);
		break;
	case SHAPE_TYPE::ARROW:
		m_drawFunc = CC_CALLBACK_2(DrawNodeShape::drawArrow, this);
		break;
	}

	showUp(m_showUp);

	return true;
}

void DrawNodeShape::showUp(bool show)
{
	if( show )
		m_drawFunc(m_pos, m_color);
	else
		m_drawFunc(Vec2(-100.0f,-100.0f), Color4F(0.0f,0.0f,0.0f,0.0f));
}

void DrawNodeShape::drawCircle(const Vec2 &pos, const Color4F &color)
{
	clear();
	drawDot(pos, m_radius, color);
}

void DrawNodeShape::drawBox(const Vec2 &pos, const Color4F &color)
{
	Vec2 verts[4];
	verts[0] = Vec2(pos.x-m_radius/2, pos.y-m_radius/2);
	verts[1] = Vec2(pos.x-m_radius/2, pos.y+m_radius/2);
	verts[2] = Vec2(pos.x+m_radius/2, pos.y+m_radius/2);
	verts[3] = Vec2(pos.x+m_radius/2, pos.y-m_radius/2);

	clear();
	drawPolygon(verts, 4, Color4F(0.0f,0.0f,0.0f,0.0f), 2.0f, color);
}

void DrawNodeShape::drawArrow(const Vec2 &pos, const Color4F &color)
{
	Vec2 verts[4];
	verts[0] = Vec2(pos.x, pos.y+m_radius*2/3);
	verts[1] = Vec2(pos.x-m_radius/3, pos.y-m_radius/3);
	verts[2] = Vec2(pos.x+m_radius/3, pos.y-m_radius/3);

	clear();
	drawPolygon(verts, 3, Color4F(0.0f,0.0f,0.0f,0.0f), 2.0f, color);
}
