#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Sound/SoundManager.h"
#include "../Stage/StageManager.h"

class UI2DElement
{
	KazMath::Vec2<float> m_easePosStart;
	KazMath::Vec2<float> m_easePosEnd;
	KazMath::Vec2<float> m_nowPos;
	float m_easePosTimer;
	float m_easePosTimerAdd = 0.1f;

	KazMath::Vec2<float> m_easeScaleStart;
	KazMath::Vec2<float> m_easeScaleEnd;
	KazMath::Vec2<float> m_nowScale;
	float m_easeScaleTimer;


	KazMath::Color m_easeStartColor;
	KazMath::Color m_easeEndColor;
	KazMath::Color m_easeAddColor = { 0, 0, 0, 1 };

	bool m_isColorEase = false;
public:
	KazMath::Color m_color;
	UI2DElement(DrawingByRasterize& arg_rasterize, const char* f_filePath);
	UI2DElement();
	void Init(DrawingByRasterize& arg_rasterize, std::string f_filePath);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
	KazMath::Vec2<float> GetNowPos() { return m_nowPos; };
	void SetEasePosAddTime(float f_easePosTimer) { m_easePosTimerAdd = f_easePosTimer; }
	float GetPosEaseTimer() { return m_easePosTimer; }
	void SetPosition(KazMath::Vec2<float> f_nowPos);
	void EasePosInit(KazMath::Vec2<float> f_easeEnd);
	void EasePosInit(KazMath::Vec2<float> f_easeStartPos, KazMath::Vec2<float> f_easeEnd, float f_timer);
	void SetScale(KazMath::Vec2<float> f_nowScale);

	void SetColor(KazMath::Color& f_color);
	void SetAddColor(KazMath::Color& f_addColor);
	void SetColorEaseEnd(KazMath::Color& f_endColor);
	BasicDraw::BasicTextureRender m_2DSprite;
	KazMath::Vec2<float> GetNowScale() { return m_nowScale; };
	KazMath::Vec2<float> GetNowPosition() { return m_nowPos; };
};

class WeponUIManager
{
public:
	enum WeponNumber
	{
		e_NonWepon,
		//e_Echo,
		e_Hundgun,
		e_WeponMax,
	};
private:
	UI2DElement m_nonWepon;
	UI2DElement m_StoneInf;
	UI2DElement m_echo;
	UI2DElement m_echoBulletInf;
	UI2DElement m_hundgun;
	UI2DElement m_hundgunBulletInf;

	UI2DElement m_TabSp;
	UI2DElement m_qSp;
	UI2DElement m_eSp;

	UI2DElement m_aimTop;
	UI2DElement m_aimSideR;
	UI2DElement m_aimSideL;
	UI2DElement m_aimSideU;
	UI2DElement m_aimSideB;
	UI2DElement m_slash;
	UI2DElement m_stoneSlash;
	std::array<BasicDraw::BasicTextureRender, 50> m_bulletNum;
	std::array<BasicDraw::BasicTextureRender, 50> m_stoneNum;

	static const int c_UITexX = 272;
	static const int c_UITexY = 105;
	static const int c_BaseUIX = 1075;
	static const int c_BaseUIY = 630;

	const float c_aimDis = 6.0f;

	static const int c_BulletNumOffsetX = 85;
	static const int c_BulletNumOffsetY = 30;

	static int m_magazinSize;
	static int m_haveBulletNum;
	static int m_bulletCount;
	static bool m_isCanShot;

	static int m_haveStone;

	//こっちはUI用
	int m_nowSelectWeponNumber;

	//選択中の武器以外を消すまでの時間
	const int c_ShowTime = 60;
	int m_showUITime;
	float easeTimer = 0.0f;
	std::list<std::pair<WeponNumber, int>> m_haveWepons;

	UI2DElement& GetUI(WeponNumber f_wepon);
	void EaseInit();
	//こっちがプレイヤーに渡す方
	WeponNumber m_nowWepon;

	SoundData m_changeWeaponSE;

public:

	WeponUIManager(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update(StageManager& f_stageManager, KazMath::Transform3D& f_playerTrans);
	void Draw(DrawingByRasterize& arg_rasterize);
	void GetStone(int f_getStone) { m_haveStone += f_getStone; };
	void GetMagazin(int f_getStone) { m_haveBulletNum += f_getStone; };
	WeponNumber GetNowWepon() { return m_nowWepon; };

	//好きなタイミングで武器追加
	void AddWepon(WeponNumber f_wepon);

	static void Shot();
	static void Reload();
	static bool GetCanShot() { return m_isCanShot; };

	static bool UseStone();
	static bool HaveStone()
	{
		if (m_haveStone > 0)return true;
		else return false;
	}

	//UIのサイズ
	static KazMath::Vec2<float> GetUITextureSize() { return { c_UITexX , c_UITexY }; }
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
	static const int c_BaseUIX = 205;
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

class DangerUIManager
{
	UI2DElement m_dangerTex;
	UI2DElement m_cautionTex;
	std::array<BasicDraw::BasicTextureRender, 50> m_timerTex;

	float m_dangerTimer;
	float m_cautionTimer;

	bool m_isLook;
	bool m_isOldLook;

public:
	DangerUIManager(DrawingByRasterize& arg_rasterize);

	void Init();
	void Update(bool f_isLook);
	void Draw(DrawingByRasterize& arg_rasterize);
};

class HPUI
{
	UI2DElement m_HPFrame;
	UI2DElement m_HPBar;
	UI2DElement m_HPBarRed;
	UI2DElement m_HPFrame2;

	SoundData m_hitDamage[4];
	/*UI2DElement m_StaminaFrame;
	UI2DElement m_StaminaBar;*/
	static const int c_texOffset = 30;
	static const int c_UITexX = 342;
	static const int c_UITexY = 14;
	static const int c_BaseUIX = c_UITexX / 2 + c_texOffset;
	static const int c_BaseUIY = c_UITexY / 2 + c_texOffset;
	static int m_hp;
	static int m_redHP;
	const int MaxHP = 100;

	static int redWaitTime;
	static const int redTime = 1;
public:
	HPUI(DrawingByRasterize& arg_rasterize);

	void Init();
	void Update(const int f_playerHP);
	void Draw(DrawingByRasterize& arg_rasterize);

	void HitDamage(int f_mainDamage, int f_redZone);

	static void InitHP();

	int GetHP() { return m_hp; };
	int RedHP() { return m_redHP; };
};

class HeartRate
{
	UI2DElement m_HeartRateBaseUI;
	UI2DElement m_HeartRateUI;
	UI2DElement m_HeartRateFrameUI;

	float rateEx = 0.0f;

	static const int c_texOffset = 40;
	static const int c_UITexX = 136;
	static const int c_UITexY = 136;
	static const int c_BaseUIX = 1280 - (c_UITexX / 2) - c_texOffset;
	static const int c_BaseUIY = c_UITexY - c_texOffset;

	int m_nowRateCount;
	float m_nowRateScale = 0.0f;
	const float m_rateScaleSpeed = 0.02f;
	int m_nowRate = 60;
	bool m_isRateDirty = false;
	int m_nextRate = 60;
	bool m_echoEnd = false;

	//拡大率
	float m_rateExt = 0.0f;

public:
	HeartRate(DrawingByRasterize& arg_rasterize);

	void Init();
	void Update(int f_heartRate);
	void Draw(DrawingByRasterize& arg_rasterize);
};

class ResultUI
{
	//背景
	UI2DElement m_back;
	//リザルトって書いてあるやつ
	UI2DElement m_ResultStrSp;
	//ミッションクリア
	UI2DElement m_missionClearSp;
	//ミッション失敗
	UI2DElement m_missionFailedSp;
	UI2DElement m_pushSpaceSp;

	bool m_isResultShow = false;

	bool m_isClear = false;

	int m_faliedColor = 100;

	int m_spaceAddColor = 1;
	int m_spaceColor = 180;
	const int C_spaceColorUnder = 180;
	const int C_spaceColorUpper = 255;
public:
	ResultUI(DrawingByRasterize& arg_rasterize);

	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
	bool GetResultShow() { return m_isResultShow; };
	void ShowResult() { m_isResultShow = true; };
	void SetClear() { m_isClear = true; };
};