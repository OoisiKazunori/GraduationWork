#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Sound/SoundManager.h"

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
	KazMath::Color m_easeAddColor = {0, 0, 0, 1};

	bool m_isColorEase = false;
public:
	KazMath::Color m_color;
	UI2DElement(DrawingByRasterize& arg_rasterize, const char *f_filePath);
	void Init(DrawingByRasterize& arg_rasterize, std::string f_filePath);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
	KazMath::Vec2<float> GetNowPos(){ return m_nowPos; };
	void SetEasePosAddTime(float f_easePosTimer){ m_easePosTimerAdd = f_easePosTimer;}
	float GetPosEaseTimer(){ return m_easePosTimer; }
	void SetPosition(KazMath::Vec2<float> f_nowPos);
	void EasePosInit(KazMath::Vec2<float> f_easeEnd);
	void EasePosInit(KazMath::Vec2<float> f_easeStartPos, KazMath::Vec2<float> f_easeEnd, float f_timer);
	void SetScale(KazMath::Vec2<float> f_nowScale);

	void SetColor(KazMath::Color &f_color);
	void SetAddColor(KazMath::Color &f_addColor);
	void SetColorEaseEnd(KazMath::Color &f_endColor);
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

	UI2DElement m_bulletNum00;
	UI2DElement m_bulletNum01;
	UI2DElement m_bulletNum02;
	UI2DElement m_bulletNum03;
	UI2DElement m_bulletNum04;
	UI2DElement m_bulletNum05;
	UI2DElement m_bulletNum06;
	UI2DElement m_bulletNum07;
	UI2DElement m_bulletNum08;
	UI2DElement m_bulletNum09;
	UI2DElement m_bulletNum10;
	UI2DElement m_bulletNum11;
	UI2DElement m_bulletNum12;
	UI2DElement m_bulletNum13;
	UI2DElement m_bulletNum14;
	UI2DElement m_bulletNum15;
	UI2DElement m_bulletNum16;
	UI2DElement m_bulletNum17;
	UI2DElement m_bulletNum18;
	UI2DElement m_bulletNum19;

	UI2DElement m_magazinNum00;
	UI2DElement m_magazinNum01;
	UI2DElement m_magazinNum02;
	UI2DElement m_magazinNum03;
	UI2DElement m_magazinNum04;
	UI2DElement m_magazinNum05;
	UI2DElement m_magazinNum06;
	UI2DElement m_magazinNum07;
	UI2DElement m_magazinNum08;
	UI2DElement m_magazinNum09;
	UI2DElement m_magazinNum10;
	UI2DElement m_magazinNum11;
	UI2DElement m_magazinNum12;
	UI2DElement m_magazinNum13;
	UI2DElement m_magazinNum14;
	UI2DElement m_magazinNum15;
	UI2DElement m_magazinNum16;
	UI2DElement m_magazinNum17;
	UI2DElement m_magazinNum18;
	UI2DElement m_magazinNum19;
	UI2DElement m_magazinNum20;
	UI2DElement m_magazinNum21;
	UI2DElement m_magazinNum22;
	UI2DElement m_magazinNum23;
	UI2DElement m_magazinNum24;
	UI2DElement m_magazinNum25;
	UI2DElement m_magazinNum26;
	UI2DElement m_magazinNum27;
	UI2DElement m_magazinNum28;
	UI2DElement m_magazinNum29;

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
	
	//��������UI�p
	int m_nowSelectWeponNumber;

	//�I�𒆂̕���ȊO�������܂ł̎���
	const int c_ShowTime = 60;
	int m_showUITime;
	float easeTimer = 0.0f;
	std::list<std::pair<WeponNumber, int>> m_haveWepons;

	UI2DElement &GetUI(WeponNumber f_wepon);
	void EaseInit();
	//���������v���C���[�ɓn����
	WeponNumber m_nowWepon;

	SoundData m_changeWeaponSE;

public:

	WeponUIManager(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);

	WeponNumber GetNowWepon() { return m_nowWepon; };
	
	//�D���ȃ^�C�~���O�ŕ���ǉ�
	void AddWepon(WeponNumber f_wepon);

	static void Shot();
	static void Reload();
	static bool GetCanShot(){ return m_isCanShot; };

	//UI�̃T�C�Y
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
	static const int c_BaseUIX = 205;
	static const int c_BaseUIY = 630;

	//��������UI�p
	int m_nowSelectGadgetNumber;

	//�I�𒆂̕���ȊO�������܂ł̎���
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


	//���������v���C���[�ɓn����
	GadgetNumber m_nowGadget;
	//�D���ȃ^�C�~���O�ŕ���ǉ�
	void AddGadget(GadgetNumber f_gadget);

	//UI�̃T�C�Y
	static KazMath::Vec2<float> GetUITextureSize() { return { c_UITexX , c_UITexY }; }
	//
	static KazMath::Vec2<float> GetUIBasePos() { return { c_BaseUIX , c_BaseUIY }; }
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

	int GetHP(){ return m_hp; };
	int RedHP(){ return m_redHP; };
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
	static const int c_BaseUIY = c_UITexY - c_texOffset ;

	int m_nowRateCount;
	float m_nowRateScale = 0.0f;
	const float m_rateScaleSpeed = 0.02f;
	int m_nowRate = 60;
	bool m_isRateDirty = false;
	int m_nextRate = 60;
	bool m_echoEnd = false;

	//�g�嗦
	float m_rateExt = 0.0f;

public:
	HeartRate(DrawingByRasterize& arg_rasterize);

	void Init();
	void Update(int f_heartRate);
	void Draw(DrawingByRasterize& arg_rasterize);
};

class ResultUI
{
	//�w�i
	UI2DElement m_back;
	//���U���g���ď����Ă�����
	UI2DElement m_ResultStrSp;
	//�~�b�V�����N���A
	UI2DElement m_missionClearSp;
	//�~�b�V�������s
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
	ResultUI(DrawingByRasterize & arg_rasterize);

	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
	bool GetResultShow() { return m_isResultShow; };
	void ShowResult(){ m_isResultShow = true; };
	void SetClear(){ m_isClear = true; };
};