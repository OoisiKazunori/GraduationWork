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
	void Update(KazMath::Vec2<float> f_pos);
	void Draw(DrawingByRasterize& arg_rasterize);


	void EasePosInit(KazMath::Vec2<float> f_easeEnd);
};

class WeponUIManager
{
	std::list<UI2DElement> m_UIs;
	UI2DElement m_nonWepon;
	UI2DElement m_knife;
	UI2DElement m_hundgun;
	static const int c_UITexX = 272;
	static const int c_UITexY = 105;
	static const int c_BaseUIX = 1095;
	static const int c_BaseUIY = 630;
	
	//こっちはUI用
	int m_nowSelectWeponNumber;

	//選択中の武器以外を消すまでの時間
	const int c_ShowTime = 60;
	int m_showUITime;
public:
	enum WeponNumber
	{
		e_NonWepon,
		e_Knife,
		e_Hundgun,
		e_Rifle,
		e_WeponMax,
	};

	WeponUIManager(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);

	
	//こっちがプレイヤーに渡す方
	WeponNumber m_nowWepon;

	std::list<std::pair<WeponNumber, int>> m_haveWepons;

	//UIのサイズ
	static KazMath::Vec2<float> GetUITextureSize(){return { c_UITexX , c_UITexY }; }
	//
	static KazMath::Vec2<float> GetUIBasePos() { return { c_BaseUIX , c_BaseUIY }; }
};

class GadgetUIManager
{
	std::list<UI2DElement> m_UIs;
	UI2DElement m_nonGadget;
	UI2DElement m_sonar;
	static const int c_UITexX = 272;
	static const int c_UITexY = 105;
	static const int c_BaseUIX = 185;
	static const int c_BaseUIY = 630;

	//こっちはUI用
	int m_nowSelectGadgetNumber;

	//選択中の武器以外を消すまでの時間
	const int c_ShowTime = 60;
	int m_showUITime;
public:
	enum GadgetNumber
	{
		e_NonGadget,
		e_Sonar,
		e_GadgetMax,
	};

	GadgetUIManager(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);


	//こっちがプレイヤーに渡す方
	GadgetNumber m_nowGadget;

	std::list<std::pair<GadgetNumber, int>> m_haveGadgets;

	//UIのサイズ
	static KazMath::Vec2<float> GetUITextureSize() { return { c_UITexX , c_UITexY }; }
	//
	static KazMath::Vec2<float> GetUIBasePos() { return { c_BaseUIX , c_BaseUIY }; }
};