#include "DrawNodeCircle.h"

USING_NS_CC;

DrawNodeCircle::DrawNodeCircle(void)
	: m_showUp(true)
{
}


DrawNodeCircle::~DrawNodeCircle(void)
{
}

DrawNodeCircle* DrawNodeCircle::create(const Vec2 &pos, float radius, const Color4F &color)
{
	DrawNodeCircle* box = new DrawNodeCircle();
	if (box && box->init(pos, radius, color))
		box->autorelease();
	else
		CC_SAFE_DELETE(box);

	return box;
}

bool DrawNodeCircle::init(const Vec2 &pos, float radius, const Color4F &color)
{
	if (!DrawNode::init())
		return false;

	setCenterPosition(pos);
	setRadius(radius);
	setLineColor(color);

	showUp(m_showUp);

	return true;
}

void DrawNodeCircle::showUp(bool show)
{
	if( show )
		this->drawCircle(m_pos, m_color);
	else
		this->drawCircle(Vec2(-100.0f,-100.0f), Color4F(0.0f,0.0f,0.0f,0.0f));
}

void DrawNodeCircle::drawCircle(const Vec2 &pos, const Color4F &color)
{
	clear();
	drawDot(pos, m_radius, color);
}

//void DrawNodeCircle::drawBox(const Vec2 &pos, const Color4F &color)
//{
//	Vec2 verts[4];
//	verts[0] = Vec2( pos.x-m_width/2, pos.y );
//	verts[1] = Vec2( pos.x-m_width/2, pos.y+m_height );
//	verts[2] = Vec2( pos.x+m_width/2, pos.y+m_height );
//	verts[3] = Vec2( pos.x+m_width/2, pos.y );
//
//	this->clear();
//	//this->drawPolygon( verts, 4, Color4F(0.0f,0.0f,0.0f,0.0f), 2.0f, color );
//	this->drawSegment( verts[0], Vec2(verts[0].x+15.0f,verts[0].y), 2.0f, color );
//	this->drawSegment( verts[0], Vec2(verts[0].x,verts[0].y+15.0f), 2.0f, color );
//
//	this->drawSegment( verts[1], Vec2(verts[1].x+15.0f,verts[1].y), 2.0f, color );
//	this->drawSegment( verts[1], Vec2(verts[1].x,verts[1].y-15.0f), 2.0f, color );
//
//	this->drawSegment( verts[2], Vec2(verts[2].x-15.0f,verts[2].y), 2.0f, color );
//	this->drawSegment( verts[2], Vec2(verts[2].x,verts[2].y-15.0f), 2.0f, color );
//
//	this->drawSegment( verts[3], Vec2(verts[3].x-15.0f,verts[3].y), 2.0f, color );
//	this->drawSegment( verts[3], Vec2(verts[3].x,verts[3].y+15.0f), 2.0f, color );
//}
