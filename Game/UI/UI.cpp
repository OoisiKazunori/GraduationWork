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
	const float yOffset = (float)c_UITexY + 10.0f;
	const float xOffset = (float)c_UITexX + 10.0f;
	float yPos = c_BaseUIY;
	float xPos = c_BaseUIX;

	int hoge = 0;
	std::list<UI2DElement> have_lists;
	for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
	{
		//横移動
		if (hoge < m_nowSelectWeponNumber)
		{
			int sub = m_nowSelectWeponNumber - hoge;
			if ((*itr).first == WeponNumber::e_NonWepon)
			{
				m_nonWepon.EasePosInit({ xPos - (xOffset * sub), yPos });
			}
			else if ((*itr).first == WeponNumber::e_Knife)
			{
				m_knife.EasePosInit({ xPos - (xOffset * sub), yPos });
			}
			else if ((*itr).first == WeponNumber::e_Hundgun)
			{
				m_hundgun.EasePosInit({ xPos - (xOffset * sub), yPos });
			}
			hoge++;
		}
		//縦移動
		else
		{
			int sub = m_nowSelectWeponNumber - hoge;
			if ((*itr).first == WeponNumber::e_NonWepon)
			{
				m_nonWepon.EasePosInit({ xPos , yPos + (yOffset * sub) });
			}
			else if ((*itr).first == WeponNumber::e_Knife)
			{
				m_knife.EasePosInit({ xPos , yPos + (yOffset * sub) });
			}
			else if ((*itr).first == WeponNumber::e_Hundgun)
			{
				m_hundgun.EasePosInit({ xPos , yPos + (yOffset * sub) });
			}
			hoge++;
		}
	}
}

void WeponUIManager::Update()
{
	const float yOffset = (float)c_UITexY + 10.0f;
	const float xOffset = (float)c_UITexX + 10.0f;
	float yPos = c_BaseUIY;
	float xPos = c_BaseUIX;
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
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F8))
	{
		AddWepon(WeponNumber::e_Hundgun);
	}
	if (isDirty)
	{
		m_nowWepon = (WeponNumber)m_nowSelectWeponNumber;
		int hoge = 0;
		std::list<UI2DElement> have_lists;
		for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
		{
			//横移動
			if (hoge < m_nowSelectWeponNumber)
			{
				int sub = m_nowSelectWeponNumber - hoge;
				if ((*itr).first == WeponNumber::e_NonWepon)
				{
					m_nonWepon.EasePosInit({ xPos - (xOffset * sub), yPos });
				}
				else if ((*itr).first == WeponNumber::e_Knife)
				{
					m_knife.EasePosInit({ xPos - (xOffset * sub), yPos });
				}
				else if ((*itr).first == WeponNumber::e_Hundgun)
				{
					m_hundgun.EasePosInit({ xPos - (xOffset * sub), yPos });
				}
				hoge++;
			}
			//縦移動
			else
			{
				int sub = m_nowSelectWeponNumber - hoge;
				if ((*itr).first == WeponNumber::e_NonWepon)
				{
					m_nonWepon.EasePosInit({ xPos , yPos + (yOffset * sub) });
				}
				else if ((*itr).first == WeponNumber::e_Knife)
				{
					m_knife.EasePosInit({ xPos , yPos + (yOffset * sub) });
				}
				else if ((*itr).first == WeponNumber::e_Hundgun)
				{
					m_hundgun.EasePosInit({ xPos , yPos + (yOffset * sub) });
				}
				hoge++;
			}

		}
	}

	for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
	{
		if ((*itr).first == WeponNumber::e_NonWepon)
		{
			m_nonWepon.Update();
		}
		else if ((*itr).first == WeponNumber::e_Knife)
		{
			m_knife.Update();
		}
		else if ((*itr).first == WeponNumber::e_Hundgun)
		{
			m_hundgun.Update();
		}
	}
	easeTimer += 0.1f;
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

		if ((*itr).first == WeponNumber::e_NonWepon)
		{
			m_nonWepon.Draw(arg_rasterize);
		}
		else if ((*itr).first == WeponNumber::e_Knife)
		{
			m_knife.Draw(arg_rasterize);
		}
		else if ((*itr).first == WeponNumber::e_Hundgun)
		{
			m_hundgun.Draw(arg_rasterize);
		}
	}
	else
	{
		for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
		{
			if ((*itr).first == WeponNumber::e_NonWepon)
			{
				m_nonWepon.Draw(arg_rasterize);
			}
			else if ((*itr).first == WeponNumber::e_Knife)
			{
				m_knife.Draw(arg_rasterize);
			}
			else if ((*itr).first == WeponNumber::e_Hundgun)
			{
				m_hundgun.Draw(arg_rasterize);
			}
		}
	}
}

void WeponUIManager::AddWepon(WeponNumber f_wepon)
{
	for (auto itr = m_haveWepons.begin(); itr != m_haveWepons.end(); ++itr)
	{
		if ((*itr).first == f_wepon)
		{
			return;
		}
	}
	m_haveWepons.push_back({ f_wepon, (int)m_haveWepons.size() - 1});
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
			if ((*itr).first == GadgetNumber::e_NonGadget)
			{
				m_nonGadget.EasePosInit({ xPos + (xOffset * sub), yPos });
			}
			else if ((*itr).first == GadgetNumber::e_Sonar)
			{
				m_sonar.EasePosInit({ xPos + (xOffset * sub), yPos });
			}
			hoge++;
		}
		//縦移動
		else
		{
			int sub = m_nowSelectGadgetNumber - hoge;
			if ((*itr).first == GadgetNumber::e_NonGadget)
			{
				m_nonGadget.EasePosInit({ xPos , yPos + (yOffset * sub) });
			}
			else if ((*itr).first == GadgetNumber::e_Sonar)
			{
				m_sonar.EasePosInit({ xPos , yPos + (yOffset * sub) });
			}
			hoge++;
		}
	}
}

void GadgetUIManager::Update()
{
	const float yOffset = (float)c_UITexY + 10.0f;
	const float xOffset = (float)c_UITexX + 10.0f;
	float yPos = c_BaseUIY;
	float xPos = c_BaseUIX;
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
		m_nowGadget = (GadgetNumber)m_nowSelectGadgetNumber;
		int hoge = 0;
		std::list<UI2DElement> have_lists;
		for (auto itr = m_haveGadgets.begin(); itr != m_haveGadgets.end(); ++itr)
		{
			//横移動
			if (hoge < m_nowSelectGadgetNumber)
			{
				int sub = m_nowSelectGadgetNumber - hoge;
				if ((*itr).first == GadgetNumber::e_NonGadget)
				{
					m_nonGadget.EasePosInit({ xPos + (xOffset * sub), yPos });
				}
				else if ((*itr).first == GadgetNumber::e_Sonar)
				{
					m_sonar.EasePosInit({ xPos + (xOffset * sub), yPos });
				}
				hoge++;
			}
			//縦移動
			else
			{
				int sub = m_nowSelectGadgetNumber - hoge;
				if ((*itr).first == GadgetNumber::e_NonGadget)
				{
					m_nonGadget.EasePosInit({ xPos , yPos + (yOffset * sub) });
				}
				else if ((*itr).first == GadgetNumber::e_Sonar)
				{
					m_sonar.EasePosInit({ xPos , yPos + (yOffset * sub) });
				}
				hoge++;
			}

		}
	}
	for (auto itr = m_haveGadgets.begin(); itr != m_haveGadgets.end(); ++itr)
	{
		if ((*itr).first == GadgetNumber::e_NonGadget)
		{
			m_nonGadget.Update();
		}
		else if ((*itr).first == GadgetNumber::e_Sonar)
		{
			m_sonar.Update();
		}
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

		if ((*itr).first == GadgetNumber::e_NonGadget)
		{
			m_nonGadget.Draw(arg_rasterize);
		}
		else if ((*itr).first == GadgetNumber::e_Sonar)
		{
			m_sonar.Draw(arg_rasterize);
		}
	}
	else
	{
		for (auto itr = m_haveGadgets.begin(); itr != m_haveGadgets.end(); ++itr)
		{
			if ((*itr).first == GadgetNumber::e_NonGadget)
			{
				m_nonGadget.Draw(arg_rasterize);
			}
			else if ((*itr).first == GadgetNumber::e_Sonar)
			{
				m_sonar.Draw(arg_rasterize);
			}
		}
	}
}

void GadgetUIManager::AddGadget(GadgetNumber f_gadget)
{
	m_haveGadgets.push_back({ f_gadget, (int)m_haveGadgets.size() -1 });
}