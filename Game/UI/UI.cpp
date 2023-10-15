#include "UI.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../KazLibrary/Easing/easing.h"

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
	m_easePosTimer += 0.1f;
	if (m_easePosTimer >= 1.0f)
	{
		m_easePosTimer = 1.0f;
	}
	auto sub = m_easePosEnd - m_easePosStart;
	m_nowPos = m_easePosStart + (sub * EasingMaker(EasingType::In, EaseInType::Quad, m_easePosTimer));
}

void UI2DElement::Draw(DrawingByRasterize& arg_rasterize)
{
	KazMath::Transform2D l_trans = KazMath::Transform2D(m_nowPos, m_nowScale);

	m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
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

WeponUIManager::WeponUIManager(DrawingByRasterize& arg_rasterize) :
	m_hundgun(arg_rasterize, "Resource/UITexture/UI_handGun.png"),
	m_knife(arg_rasterize, "Resource/UITexture/UI_Knife.png"),
	m_nonWepon(arg_rasterize, "Resource/UITexture/UI_hand.png")
{
	m_nowWepon = e_NonWepon;
	m_haveWepons.push_back({ WeponNumber::e_NonWepon, 0 });
	m_haveWepons.push_back({ WeponNumber::e_Knife, 1 });
	//m_haveWepons.push_back({ WeponNumber::e_Hundgun, 2 });
	m_nowSelectWeponNumber = 0;
	m_showUITime = 0;
}

void WeponUIManager::Init()
{
	m_nowWepon = e_NonWepon;
	m_haveWepons.clear();
	m_haveWepons.push_back({ WeponNumber::e_NonWepon, 0 });
	m_haveWepons.push_back({ WeponNumber::e_Knife, 1 });
	//m_haveWepons.push_back({ WeponNumber::e_Hundgun, 2 });
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
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_E) && easeTimer > 0.6f)
		{
			if (m_nowSelectWeponNumber < m_haveWepons.size() - 1)
			{
				m_nowSelectWeponNumber = m_nowSelectWeponNumber + 1;
				isDirty = true;
			}
			easeTimer = 0.0f;
		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_Q) && easeTimer > 0.6f)
		{
			if (m_nowSelectWeponNumber > 0)
			{
				m_nowSelectWeponNumber = m_nowSelectWeponNumber - 1;
				isDirty = true;
			}
			easeTimer = 0.0f;
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
		auto itr = m_haveWepons.begin();
		//所持している武器までイテレーターを回す
		for (int i = 0; i < m_nowSelectWeponNumber; i++)
		{
			itr++;
		}
		GetUI((*itr).first).Draw(arg_rasterize);
	}
	else
	{
		for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
		{
			GetUI((*itr).first).Draw(arg_rasterize);
		}
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
	else if (f_wepon == WeponNumber::e_Knife)
	{
		return m_knife;
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
	m_HPFrame(arg_rasterize, "Resource/UITexture/UI_HPBarBase.png"),
	m_HPBar(arg_rasterize, "Resource/UITexture/UI_HPBar.png"),
	m_HPBarRed(arg_rasterize, "Resource/UITexture/UI_HPBar_Red.png")/*,
	m_StaminaFrame(arg_rasterize, "Resource/UITexture/UI_hand.png"),
	m_StaminaBar(arg_rasterize, "Resource/UITexture/UI_hand.png")*/
{
	m_hp = 100;
}

void HPUI::Init()
{

}


void HPUI::Update(const int f_playerHP)
{
	static int redWaitTime = 0;
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_P))
	{
		HitDamage(10, 5);
		redWaitTime = 10;
	}
	redWaitTime--;
	if (redWaitTime < 0)
	{
		if (m_redHP > 0)
		{
			m_redHP--;
			redWaitTime = 10;
		}
	}
}

void HPUI::Draw(DrawingByRasterize& arg_rasterize)
{
	float half = c_UITexX / 2.0f;
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
}

void HPUI::HitDamage(int f_mainDamage, int f_redZone)
{
	m_hp -= f_mainDamage;
	m_hp -= m_redHP;
	if (m_hp < 0)
	{
		m_hp = 0;
	}
	if (m_hp > 0)
	{
		if (m_hp > f_redZone)
		{
			m_redHP = f_redZone;
		}
		else
		{
			m_redHP = m_hp;
		}
	}
	else
	{
		m_redHP = 0;
	}
}