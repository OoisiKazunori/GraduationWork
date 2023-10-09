#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "../KazLibrary/Math/KazMath.h"

class UI2DElement
{
	BasicDraw::BasicTextureRender m_2DSprite;
	
	KazMath::Vec2<float> m_easePosStart;
	KazMath::Vec2<float> m_easePosEnd;
	KazMath::Vec2<float> m_nowPos;
	float m_easePosTimer;

	KazMath::Vec2<float> m_easeScaleStart;
	KazMath::Vec2<float> m_easeScaleEnd;
	KazMath::Vec2<float> m_nowScale;
	float m_easeScaleTimer;
public:
	UI2DElement(DrawingByRasterize& arg_rasterize, const char *f_filePath);
	void Init(DrawingByRasterize& arg_rasterize, std::string f_filePath);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);


	void EasePosInit(KazMath::Vec2<float> f_easeEnd);
};

class WeponUIManager
{
	std::list<UI2DElement> m_UIs;
	UI2DElement m_knife;
	UI2DElement m_hundgun;
	static const int c_UITexX = 272;
	static const int c_UITexY = 105;
	static const int c_baseUIX = 1095;
	static const int c_baseUIY = 630;
public:
	enum WeponNumber
	{
		e_Knife,
		e_Hundgun,
		e_Rifle,
	};

	WeponUIManager(DrawingByRasterize& arg_rasterize);
	void Init(DrawingByRasterize& arg_rasterize);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);

	WeponNumber m_nowWepon;

	//UIÇÃÉTÉCÉY
	static KazMath::Vec2<float> GetUITextureSize(){return { c_UITexX , c_UITexY }; }
	//
	static KazMath::Vec2<float> GetUIBasePos() { return { c_baseUIX , c_baseUIY }; }
};