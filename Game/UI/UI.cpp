#include "UI.h"
#include "../Input/Input.h"
#include "../KazLibrary/Easing/easing.h"

int HPUI::m_hp = 100;
int HPUI::m_redHP = 0;

int HPUI::redWaitTime = 0;

UI2DElement::UI2DElement(DrawingByRasterize& arg_rasterize, const char* f_filePath) :
	m_2DSprite(arg_rasterize, f_filePath, true)
{
	m_nowPos = { WeponUIManager::GetUIBasePos().x, WeponUIManager::GetUIBasePos().y };
	m_nowScale = { 1.0f, 1.0f };
}

void UI2DElement::Init(DrawingByRasterize& arg_rasterize, std::string f_filePath)
{


}

void UI2DElement::Update()
{
	m_easePosTimer += m_easePosTimerAdd;
	if (m_easePosTimer >= 1.0f)
	{
		m_easePosTimer = 1.0f;
	}
	if (m_easePosTimer < 0.0f)
	{
		m_nowPos = m_easePosStart;
		return;
	}
	auto sub = m_easePosEnd - m_easePosStart;
	m_nowPos = m_easePosStart + (sub * EasingMaker(EasingType::In, EaseInType::Quad, m_easePosTimer));
	if (!m_isColorEase)return;
	if (m_easeEndColor.color.a - m_color.color.a < 0)
	{
		m_color.color.a -= m_easeAddColor.color.a;
	}
	else
	{
		m_color.color.a += m_easeAddColor.color.a;
	}
	if (m_color.color.a - m_easeEndColor.color.a == 0)
	{
		m_isColorEase = false;
	}
}

void UI2DElement::Draw(DrawingByRasterize& arg_rasterize)
{
	KazMath::Transform2D l_trans = KazMath::Transform2D(m_nowPos, m_nowScale);

	m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans, m_color);
}

void UI2DElement::SetScale(KazMath::Vec2<float> f_nowScale)
{
	m_nowScale = f_nowScale;
}

void UI2DElement::SetPosition(KazMath::Vec2<float> f_nowPos)
{
	m_nowPos = f_nowPos;
}

void UI2DElement::EasePosInit(KazMath::Vec2<float> f_easeEnd)
{
	m_easePosTimer = 0.0f;

	m_easePosEnd = f_easeEnd;
	m_easePosStart = m_nowPos;
}

void UI2DElement::EasePosInit(KazMath::Vec2<float> f_easeStartPos, KazMath::Vec2<float> f_easeEnd, float f_timer)
{
	m_easePosTimer = f_timer;

	m_easePosEnd = f_easeEnd;
	m_easePosStart = f_easeStartPos;
}

void UI2DElement::SetColor(KazMath::Color& f_color)
{
	m_color = f_color;
}

void UI2DElement::SetAddColor(KazMath::Color& f_addColor)
{
	m_easeAddColor = f_addColor;
}

void UI2DElement::SetColorEaseEnd(KazMath::Color& f_endColor)
{
	m_easeStartColor = m_color;
	m_easeEndColor = f_endColor;
	m_isColorEase = true;
}

WeponUIManager::WeponUIManager(DrawingByRasterize& arg_rasterize) :
	m_hundgun(arg_rasterize, "Resource/UITexture/UI_handGun.png"),
	m_echo(arg_rasterize, "Resource/UITexture/Weapon_UI_ECHO.png"),
	m_nonWepon(arg_rasterize, "Resource/UITexture/UI_hand.png"),
	m_TabSp(arg_rasterize, "Resource/UITexture/Tab.png"),
	m_qSp(arg_rasterize, "Resource/UITexture/Q.png"),
	m_eSp(arg_rasterize, "Resource/UITexture/E.png"),
	m_aimTop(arg_rasterize, "Resource/UITexture/gunAim.png"),
	m_aimSideR(arg_rasterize, "Resource/UITexture/aimSideR.png"),
	m_aimSideL(arg_rasterize, "Resource/UITexture/aimSideL.png"),
	m_aimSideU(arg_rasterize, "Resource/UITexture/aimSideU.png"),
	m_aimSideB(arg_rasterize, "Resource/UITexture/aimSideB.png"),
	m_echoBulletInf(arg_rasterize, "Resource/UITexture/Infinity.png"),
	m_hundgunBulletInf(arg_rasterize, "Resource/UITexture/Infinity.png")
{
	m_nowWepon = e_NonWepon;
	m_haveWepons.push_back({ WeponNumber::e_NonWepon, 0 });
	m_haveWepons.push_back({ WeponNumber::e_Echo, 1 });
	m_haveWepons.push_back({ WeponNumber::e_Hundgun, 2 });
	m_nowSelectWeponNumber = 0;
	m_showUITime = 0;
	m_TabSp.SetPosition({ 1243.0f, 675.0f });
	m_TabSp.SetScale({ 0.5f, 0.5f });
	m_qSp.SetPosition({ 920.0f, 675.0f });
	m_qSp.SetScale({ 0.5f, 0.5f });
	m_eSp.SetPosition({ 1230.0f, 580.0f });
	m_eSp.SetScale({ 0.5f, 0.5f });

	m_aimTop.SetPosition({ 1280.0f / 2.0f, 720.0f / 2.0f });
	m_aimTop.SetScale({ 0.06f, 0.06f });

	m_aimSideR.SetPosition({ 1280.0f / 2.0f + c_aimDis, 720.0f / 2.0f });
	m_aimSideL.SetPosition({ 1280.0f / 2.0f - c_aimDis, 720.0f / 2.0f });
	m_aimSideU.SetPosition({ 1280.0f / 2.0f, 720.0f / 2.0f - c_aimDis });
	m_aimSideB.SetPosition({ 1280.0f / 2.0f, 720.0f / 2.0f + c_aimDis });
	float scl = 0.3f;
	m_aimSideR.SetScale({ scl, scl });
	m_aimSideL.SetScale({ scl, scl });
	m_aimSideU.SetScale({ scl, scl });
	m_aimSideB.SetScale({ scl, scl });

	m_changeWeaponSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/ChangeWeapon.wav");
	m_changeWeaponSE.volume = 0.05f;

}

void WeponUIManager::Init()
{
	m_nowWepon = e_NonWepon;
	m_haveWepons.clear();
	m_haveWepons.push_back({ WeponNumber::e_NonWepon, 0 });
	m_haveWepons.push_back({ WeponNumber::e_Echo, 1 });
	m_haveWepons.push_back({ WeponNumber::e_Hundgun, 2 });
	m_nowSelectWeponNumber = 0;
	m_showUITime = 0;
	EaseInit();
}

void WeponUIManager::Update()
{
	bool isDirty = false;
	if (KeyBoradInputManager::Instance()->InputState(DIK_TAB))
	{
		m_showUITime = c_ShowTime;
	}
	if (m_showUITime > 0)
	{
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_E) && easeTimer > 0.6f)
		{
			if (m_nowSelectWeponNumber < m_haveWepons.size() - 1)
			{
				m_nowSelectWeponNumber = m_nowSelectWeponNumber + 1;
				isDirty = true;
				m_showUITime = c_ShowTime;
			}
			easeTimer = 0.0f;

			SoundManager::Instance()->SoundPlayerWave(m_changeWeaponSE, 0);

		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_Q) && easeTimer > 0.6f)
		{
			if (m_nowSelectWeponNumber > 0)
			{
				m_nowSelectWeponNumber = m_nowSelectWeponNumber - 1;
				isDirty = true;
				m_showUITime = c_ShowTime;
			}
			easeTimer = 0.0f;

			SoundManager::Instance()->SoundPlayerWave(m_changeWeaponSE, 0);

		}
	}
	//てすと
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F8))
	{
		AddWepon(WeponNumber::e_Hundgun);
		isDirty = true;
	}
	if (isDirty)
	{
		EaseInit();
	}

	for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
	{
		GetUI((*itr).first).Update();
	}
	easeTimer += 0.1f;
}

void WeponUIManager::EaseInit()
{
	const float yOffset = (float)c_UITexY + 10.0f;
	const float xOffset = (float)c_UITexX + 10.0f;
	float yPos = c_BaseUIY;
	float xPos = c_BaseUIX;
	int hoge = 0;
	for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
	{
		//横移動
		if (hoge < m_nowSelectWeponNumber)
		{
			int sub = m_nowSelectWeponNumber - hoge;
			GetUI((*itr).first).EasePosInit({ xPos - (xOffset * sub), yPos });
			hoge++;
		}
		//縦移動
		else
		{
			int sub = m_nowSelectWeponNumber - hoge;
			if (sub == 0)
			{
				m_nowWepon = (*itr).first;
			}
			GetUI((*itr).first).EasePosInit({ xPos , yPos + (yOffset * sub) });
			hoge++;
		}
	}
}

void WeponUIManager::Draw(DrawingByRasterize& arg_rasterize)
{

	m_showUITime--;
	if (m_showUITime < 0)
	{
		m_TabSp.m_color = { 255, 255, 255, 150 };
		m_TabSp.Draw(arg_rasterize);
		auto itr = m_haveWepons.begin();
		//所持している武器までイテレーターを回す
		for (int i = 0; i < m_nowSelectWeponNumber; i++)
		{
			itr++;
		}
		if ((*itr).first == e_Echo)
		{
			m_echoBulletInf.SetPosition({ m_echo.GetNowPos().x + (float)c_BulletNumOffsetX,
				m_echo.GetNowPos().y + (float)c_BulletNumOffsetY });
			m_echoBulletInf.Draw(arg_rasterize);
		}
		else if ((*itr).first == e_Hundgun)
		{
			m_hundgunBulletInf.SetPosition({ m_hundgun.GetNowPos().x + (float)c_BulletNumOffsetX,
				m_hundgun.GetNowPos().y + (float)c_BulletNumOffsetY });
			m_hundgunBulletInf.Draw(arg_rasterize);
		}
		GetUI((*itr).first).Draw(arg_rasterize);
	}
	else
	{
		m_TabSp.m_color = { 255, 255, 255, 255 };
		m_TabSp.Draw(arg_rasterize);
		m_qSp.Draw(arg_rasterize);
		m_eSp.Draw(arg_rasterize);
		for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
		{
			if ((*itr).first == e_Echo)
			{
				m_echoBulletInf.SetPosition({m_echo.GetNowPos().x + (float)c_BulletNumOffsetX,
					m_echo.GetNowPos().y + (float)c_BulletNumOffsetY });
				m_echoBulletInf.Draw(arg_rasterize);
			}
			else if ((*itr).first == e_Hundgun)
			{
				m_hundgunBulletInf.SetPosition({ m_hundgun.GetNowPos().x + (float)c_BulletNumOffsetX,
					m_hundgun.GetNowPos().y + (float)c_BulletNumOffsetY });
				m_hundgunBulletInf.Draw(arg_rasterize);
			}
			GetUI((*itr).first).Draw(arg_rasterize);
		}
	}
	if (m_nowSelectWeponNumber == e_Echo || m_nowSelectWeponNumber == e_Hundgun)
	{
		m_aimTop.Draw(arg_rasterize);
		m_aimSideR.Draw(arg_rasterize);
		m_aimSideL.Draw(arg_rasterize);
		m_aimSideU.Draw(arg_rasterize);
		m_aimSideB.Draw(arg_rasterize);
	}
}

void WeponUIManager::AddWepon(WeponNumber f_wepon)
{
	for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
	{
		if ((*itr).first == f_wepon)
		{
			//既に持っている武器は再度追加しない
			return;
		}
	}
	m_haveWepons.push_back({ f_wepon, (int)m_haveWepons.size() - 1 });

	GetUI(f_wepon).SetPosition({ c_BaseUIX , c_BaseUIY });
}

UI2DElement& WeponUIManager::GetUI(WeponNumber f_wepon)
{
	if (f_wepon == WeponNumber::e_NonWepon)
	{
		return m_nonWepon;
	}
	else if (f_wepon == WeponNumber::e_Echo)
	{
		return m_echo;
	}
	else if (f_wepon == WeponNumber::e_Hundgun)
	{
		return m_hundgun;
	}
	/*else if (f_wepon == WeponNumber::e_Rifle)
	{
		return;
	}
	else
	{
		return;
	}*/
	return m_nonWepon;
}

GadgetUIManager::GadgetUIManager(DrawingByRasterize& arg_rasterize) :
	m_nonGadget(arg_rasterize, "Resource/UITexture/UI_hand.png"),
	m_sonar(arg_rasterize, "Resource/UITexture/UI_sonar.png")
{
	m_nowGadget = e_NonGadget;
	m_haveGadgets.push_back({ GadgetNumber::e_NonGadget, 0 });
	m_haveGadgets.push_back({ GadgetNumber::e_Sonar, 1 });
	m_nowSelectGadgetNumber = 0;
	m_showUITime = 0;
}

void GadgetUIManager::Init()
{
	m_nowGadget = e_NonGadget;
	m_haveGadgets.clear();
	m_haveGadgets.push_back({ GadgetNumber::e_NonGadget, 0 });
	m_haveGadgets.push_back({ GadgetNumber::e_Sonar, 1 });
	m_nowSelectGadgetNumber = 0;
	m_showUITime = 0;
	EaseInit();
}

void GadgetUIManager::Update()
{
	bool isDirty = false;
	if (KeyBoradInputManager::Instance()->InputState(DIK_LSHIFT))
	{
		m_showUITime = c_ShowTime;
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_E) && easeTimer > 0.6f)
		{
			if (m_nowSelectGadgetNumber < m_haveGadgets.size() - 1)
			{
				m_nowSelectGadgetNumber = m_nowSelectGadgetNumber + 1;
				isDirty = true;
			}
			easeTimer = 0.0f;
		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_Q) && easeTimer > 0.6f)
		{
			if (m_nowSelectGadgetNumber > 0)
			{
				m_nowSelectGadgetNumber = m_nowSelectGadgetNumber - 1;
				isDirty = true;
			}
			easeTimer = 0.0f;
		}
	}
	if (isDirty)
	{
		EaseInit();
	}
	for (auto itr = m_haveGadgets.begin(); itr != m_haveGadgets.end(); ++itr)
	{
		GetUI((*itr).first).Update();
	}
	easeTimer += 0.1f;
}

void GadgetUIManager::Draw(DrawingByRasterize& arg_rasterize)
{
	m_showUITime--;
	if (m_showUITime < 0)
	{
		auto itr = m_haveGadgets.begin();
		//所持している武器までイテレーターを回す
		for (int i = 0; i < m_nowSelectGadgetNumber; i++)
		{
			itr++;
		}
		GetUI((*itr).first).Draw(arg_rasterize);
	}
	else
	{
		for (auto itr = m_haveGadgets.begin(); itr != m_haveGadgets.end(); ++itr)
		{
			GetUI((*itr).first).Draw(arg_rasterize);
		}
	}
}

void GadgetUIManager::EaseInit()
{
	const float yOffset = (float)c_UITexY + 10.0f;
	const float xOffset = (float)c_UITexX + 10.0f;
	float yPos = c_BaseUIY;
	float xPos = c_BaseUIX;

	int hoge = 0;
	std::list<UI2DElement> have_lists;
	for (auto itr = m_haveGadgets.begin(); itr != m_haveGadgets.end(); ++itr)
	{
		//横移動
		if (hoge < m_nowSelectGadgetNumber)
		{
			int sub = m_nowSelectGadgetNumber - hoge;
			GetUI((*itr).first).EasePosInit({ xPos + (xOffset * sub), yPos });
			hoge++;
		}
		//縦移動
		else
		{
			int sub = m_nowSelectGadgetNumber - hoge;
			GetUI((*itr).first).EasePosInit({ xPos , yPos + (yOffset * sub) });
			hoge++;
		}
	}
}

void GadgetUIManager::AddGadget(GadgetNumber f_gadget)
{
	for (auto itr = m_haveGadgets.begin(); itr != m_haveGadgets.end(); ++itr)
	{
		if ((*itr).first == f_gadget)
		{
			//既に持っている武器は再度追加しない
			return;
		}
	}
	m_haveGadgets.push_back({ f_gadget, (int)m_haveGadgets.size() - 1 });
}

UI2DElement& GadgetUIManager::GetUI(GadgetNumber f_gadget)
{
	if (f_gadget == GadgetNumber::e_NonGadget)
	{
		return m_nonGadget;
	}
	else if (f_gadget == GadgetNumber::e_Sonar)
	{
		return m_sonar;
	}

	return m_nonGadget;
}

HPUI::HPUI(DrawingByRasterize& arg_rasterize) :
	m_HPFrame(arg_rasterize, "Resource/UITexture/HPBer4.png"),
	m_HPBar(arg_rasterize, "Resource/UITexture/HPBer2.png"),
	m_HPBarRed(arg_rasterize, "Resource/UITexture/HPBer3.png"),
	m_HPFrame2(arg_rasterize, "Resource/UITexture/HPBer1.png")
{
	m_hp = 100;
}

void HPUI::Init()
{

}


void HPUI::Update(const int f_playerHP)
{
	//HP減らすときはここを参照！
	/*if (KeyBoradInputManager::Instance()->InputTrigger(DIK_P))
	{
		HitDamage(10, 10);
	}*/
	redWaitTime--;
	if (redWaitTime < 0)
	{
		if (m_redHP > 0)
		{
			if (m_hp > 0)
			{
				redWaitTime = redTime;
				m_redHP--;
			}
			else
			{
				redWaitTime = 1;
				m_redHP -= 2;
			}
			if (m_redHP < 0)
			{
				m_redHP = 0;
			}
		}
	}
}

void HPUI::Draw(DrawingByRasterize& arg_rasterize)
{
	/*float half = c_UITexX / 2.0f;
	half = half / MaxHP * m_hp;
	m_HPBar.SetScale({ (float)m_hp / (float)MaxHP, 1.0f });
	m_HPBar.SetPosition({ half + (float)c_texOffset, c_BaseUIY });
	m_HPBar.Draw(arg_rasterize);


	float redXSize = (float)m_redHP / (float)MaxHP;
	float redPos = half * 2.0f - (c_UITexX * (float)m_redHP / (float)MaxHP / 2.0f);
	m_HPBarRed.SetScale({ redXSize, 1.0f });
	m_HPBarRed.SetPosition({ redPos + (float)c_texOffset, c_BaseUIY });
	m_HPBarRed.Draw(arg_rasterize);

	m_HPFrame.SetPosition({ c_BaseUIX , c_BaseUIY });
	m_HPFrame.Draw(arg_rasterize);
	m_HPFrame2.SetPosition({ c_BaseUIX , c_BaseUIY });
	m_HPFrame2.Draw(arg_rasterize);*/

	//描画順が反転してるっぽい
	float half = c_UITexX / 2.0f;
	half = half / MaxHP * m_hp;
	float redXSize = (float)m_redHP / (float)MaxHP;
	float redPos = half * 2.0f + (c_UITexX * (float)m_redHP / (float)MaxHP / 2.0f);
	m_HPFrame2.SetPosition({ c_BaseUIX , c_BaseUIY });
	m_HPFrame2.Draw(arg_rasterize);

	m_HPBarRed.SetScale({ redXSize, 1.0f });
	m_HPBarRed.SetPosition({ redPos + (float)c_texOffset, c_BaseUIY });
	m_HPBarRed.Draw(arg_rasterize);

	m_HPBar.SetScale({ (float)m_hp / (float)MaxHP, 1.0f });
	m_HPBar.SetPosition({ half + (float)c_texOffset, c_BaseUIY });
	m_HPBar.Draw(arg_rasterize);

	m_HPFrame.SetPosition({ c_BaseUIX , c_BaseUIY });
	m_HPFrame.Draw(arg_rasterize);
}

void HPUI::HitDamage(int f_mainDamage, int f_redZone)
{
	m_hp -= f_mainDamage;
	//m_hp -= m_redHP;
	if (m_hp < 0)
	{
		m_hp = 0;
	}
	if (m_hp > 0)
	{
		if (m_hp >= f_redZone)
		{
			m_redHP += f_redZone;
		}
		/*else
		{
			m_redHP = m_hp;
		}*/
	}
	else
	{
		//m_redHP = 0;
	}
	if (m_hp > 0)
	{
		redWaitTime = 45;
	}
	else
	{
		redWaitTime = 0;
	}
}

void HPUI::InitHP()
{
	m_hp = 100;
	m_redHP = 0;
}

HeartRate::HeartRate(DrawingByRasterize& arg_rasterize) :
	m_HeartRateBaseUI(arg_rasterize, "Resource/UITexture/heartBase.png"),
	m_HeartRateUI(arg_rasterize, "Resource/UITexture/heartRate.png"),
	m_HeartRateFrameUI(arg_rasterize, "Resource/UITexture/heartFrame.png")
{

}

void HeartRate::Update(int f_heartRate)
{
	if (m_nowRate != f_heartRate)
	{
		m_isRateDirty = true;
		m_nextRate = f_heartRate;
	}

	m_nowRateCount--;
	if (m_nowRateCount <= 0)
	{
		m_nowRateCount = m_nextRate;
		m_nowRate = m_nextRate;
		m_nowRateScale = 0.0f;
		m_echoEnd = false;
	}
	//エコーを広げてく
	else
	{
		if (!m_echoEnd)
		{
			m_nowRateScale += m_rateScaleSpeed;
			if (m_nowRateScale > 1.0f)
			{
				m_echoEnd = true;
				m_nowRateScale = 0.0f;
			}
		}
	}
}

void HeartRate::Draw(DrawingByRasterize& arg_rasterize)
{
	m_HeartRateBaseUI.SetPosition({ c_BaseUIX, c_BaseUIY });
	m_HeartRateBaseUI.Draw(arg_rasterize);

	m_HeartRateUI.SetScale({ m_nowRateScale , m_nowRateScale });
	m_HeartRateUI.SetPosition({ c_BaseUIX, c_BaseUIY });
	m_HeartRateUI.Draw(arg_rasterize);

	m_HeartRateFrameUI.SetPosition({ c_BaseUIX , c_BaseUIY });
	m_HeartRateFrameUI.Draw(arg_rasterize);


}

ResultUI::ResultUI(DrawingByRasterize& arg_rasterize) :
	m_back(arg_rasterize, "Resource/MenuTex/MenuBackTex.png"),
	m_ResultStrSp(arg_rasterize, "Resource/UITexture/Result.png"),
	m_missionClearSp(arg_rasterize, "Resource/UITexture/Succses.png"),
	m_missionFailedSp(arg_rasterize, "Resource/UITexture/Defeat.png"),
	m_pushSpaceSp(arg_rasterize, "Resource/UITexture/PushSpace.png")
{
	m_pushSpaceSp.SetPosition({ 1280.0 / 2.0f, 800.0f });
	m_pushSpaceSp.EasePosInit({ 1280.0 / 2.0f, 720.0f / 2.0f + 250.0f });
	m_back.SetPosition({ 1280.0 / 2.0f, 720.0f / 2.0f });
	m_ResultStrSp.SetPosition({ -300.0f, 100.0f });
	m_ResultStrSp.EasePosInit({ 300.0f, 100.0f });

	m_missionFailedSp.SetPosition({ 1280.0f / 2.0f, 720.0f / 2.0f + 60.0f });
	m_missionFailedSp.EasePosInit({ 1280.0f / 2.0f, 720.0f / 2.0f });
	m_missionFailedSp.SetScale({ 2.0f, 2.0f });
	m_missionFailedSp.SetEasePosAddTime(0.04f);
	m_faliedColor = 100;

	m_missionClearSp.SetPosition({ 1280.0f / 2.0f, 0.0f });
	m_missionClearSp.EasePosInit({ 1280.0f / 2.0f, 720.0f / 2.0f });
	m_missionClearSp.SetScale({ 2.0f, 2.0f });
	m_missionClearSp.SetEasePosAddTime(0.5f);
}

void ResultUI::Init()
{
}

void ResultUI::Update()
{
	m_pushSpaceSp.Update();
	m_ResultStrSp.Update();

	//m_missionFailedSp.Update();

}

void ResultUI::Draw(DrawingByRasterize& arg_rasterize)
{
	if (m_spaceColor > C_spaceColorUpper || m_spaceColor < C_spaceColorUnder)
	{
		m_spaceAddColor = -m_spaceAddColor;
	}

	m_spaceColor += m_spaceAddColor;
	m_pushSpaceSp.m_color = { 255, 255, 255, m_spaceColor };
	m_pushSpaceSp.Draw(arg_rasterize);
	m_ResultStrSp.Draw(arg_rasterize);

	if (m_isResultShow && !m_isClear)
	{
		m_faliedColor += 1;
		m_missionFailedSp.m_color = { 255, 255, 255, m_faliedColor };
		m_missionFailedSp.Update();
		m_missionFailedSp.Draw(arg_rasterize);
	}
	else if (m_isClear)
	{
		m_missionClearSp.Update();
		m_missionClearSp.Draw(arg_rasterize);
	}
	m_back.Draw(arg_rasterize);
}
