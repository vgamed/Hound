#ifndef __HOUND_DRAW_NODE_SHAPE_H__
#define __HOUND_DRAW_NODE_SHAPE_H__

#include "cocos2d.h"

class DrawNodeShape :
	public cocos2d::DrawNode
{
public:
	typedef std::function<void (const cocos2d::Vec2&, const cocos2d::Color4F)>	DRAW_FUNC;

	enum class SHAPE_TYPE
	{
		BOX = 1,
		CIRCLE,
		ARROW
	};

	DrawNodeShape(void);
	~DrawNodeShape(void);

	static DrawNodeShape* create(SHAPE_TYPE type,
		const cocos2d::Vec2 &pos, 
		float radius = 10.0f,
		const cocos2d::Color4F &color = cocos2d::Color4F(1.0f,1.0f,1.0f,0.6f));

	bool init(SHAPE_TYPE type, 
		const cocos2d::Vec2& pos, 
		float radius, 
		const cocos2d::Color4F &color);

	void showUp(bool show);
	
	bool isShownUp(void) 
	{ return m_showUp; }

	CC_SYNTHESIZE(float, m_radius, Radius);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vec2, m_pos, CenterPosition);
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Color4F, m_color, LineColor);

private:
	void drawBox(const cocos2d::Vec2 &pos, const cocos2d::Color4F &color);
	void drawCircle(const cocos2d::Vec2 &pos, const cocos2d::Color4F &color);
	void drawArrow(const cocos2d::Vec2 &pos, const cocos2d::Color4F &color);
	
	 DRAW_FUNC m_drawFunc;

	bool m_showUp;
};

#endif //__HOUND_DRAW_NODE_SHAPE_H__

