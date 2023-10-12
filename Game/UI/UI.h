#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "../KazLibrary/Math/KazMath.h"

class UI2DElement
{
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

	void SetPosition(KazMath::Vec2<float> f_nowPos);
	void EasePosInit(KazMath::Vec2<float> f_easeEnd);
	void SetScale(KazMath::Vec2<float> f_nowScale);
	BasicDraw::BasicTextureRender m_2DSprite;
};

class WeponUIManager
{
public:
	enum WeponNumber
	{
		e_NonWepon,
		e_Knife,
		e_Hundgun,
		e_Rifle,
		e_WeponMax,
	};
private:
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
	float easeTimer = 0.0f;
	std::list<std::pair<WeponNumber, int>> m_haveWepons;

	UI2DElement &GetUI(WeponNumber f_wepon);
	void EaseInit();
public:

	WeponUIManager(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);

	
	//こっちがプレイヤーに渡す方
	WeponNumber m_nowWepon;
	//好きなタイミングで武器追加
	void AddWepon(WeponNumber f_wepon);

	//UIのサイズ
	static KazMath::Vec2<float> GetUITextureSize(){return { c_UITexX , c_UITexY }; }
	//
	static KazMath::Vec2<float> GetUIBasePos() { return { c_BaseUIX , c_BaseUIY }; }
};

class GadgetUIManager
{
public:
	enum GadgetNumber
	{
		e_NonGadget,
		e_Sonar,
		e_GadgetMax,
	};
private:
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
	float easeTimer = 0.0f;
	std::list<std::pair<GadgetNumber, int>> m_haveGadgets;

	UI2DElement& GetUI(GadgetNumber f_gadget);
	void EaseInit();
public:

	GadgetUIManager(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);


	//こっちがプレイヤーに渡す方
	GadgetNumber m_nowGadget;
	//好きなタイミングで武器追加
	void AddGadget(GadgetNumber f_gadget);

	//UIのサイズ
	static KazMath::Vec2<float> GetUITextureSize() { return { c_UITexX , c_UITexY }; }
	//
	static KazMath::Vec2<float> GetUIBasePos() { return { c_BaseUIX , c_BaseUIY }; }
};

class HPUI
{
	UI2DElement m_HPFrame;
	UI2DElement m_HPBar;
	UI2DElement m_HPBarRed;
	/*UI2DElement m_StaminaFrame;
	UI2DElement m_StaminaBar;*/
	static const int c_texOffset = 30;
	static const int c_UITexX = 342;
	static const int c_UITexY = 14;
	static const int c_BaseUIX = c_UITexX / 2 + c_texOffset;
	static const int c_BaseUIY = c_UITexY / 2 + c_texOffset;
	int m_hp = 100;
	int m_redHP = 0;
	const int MaxHP = 100;
public:
	HPUI(DrawingByRasterize& arg_rasterize);

	void Init();
	void Update(const int f_playerHP);
	void Draw(DrawingByRasterize& arg_rasterize);

};

class CamouflagePercent
{
	UI2DElement m_baseUI;

};