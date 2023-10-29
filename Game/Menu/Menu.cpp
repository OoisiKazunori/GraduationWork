#include "Menu.h"
#include "../Input/Input.h"
#include"../KazLibrary/Render/BasicDraw.h"
void Menu::Init()
{

}

void Menu::Update()
{
	//メニューのOnOff
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F3))
	{
		//メニューを開き始める
		if (!m_isMenuOpen)
		{
			MenuInit();
		}
		//メニュー閉じる
		else
		{
			MenuClose();
		}
	}
	//開くアニメーション中
	if (m_isNowOpen)
	{
		UpdateOpen();
	}
	//閉じるアニメーション中
	else if (m_isNowClose)
	{
		UpdateClose();
	}
	else
	{
		//キーの入力に応じて更新する
		if (m_isMenuOpen)
		{
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_S) || KeyBoradInputManager::Instance()->InputTrigger(DIK_DOWN))
			{
				int nowNum = static_cast<int>(nowSelectMenu);
				nowNum += 1;
				if (nowNum >= static_cast<int>(MenuOptions::OptionsMax))
				{
					nowNum = 0;
				}
				nowSelectMenu = static_cast<MenuOptions>(nowNum);
				m_selectBack.SetPosition({ (float)C_MenuBaseX, (float)C_MenuBaseY + (C_MenuDistanceY * nowNum)});
			}
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_W) || KeyBoradInputManager::Instance()->InputTrigger(DIK_UP))
			{
				int nowNum = static_cast<int>(nowSelectMenu);
				nowNum -= 1;
				if (nowNum < 0)
				{
					nowNum = (int)MenuOptions::OptionsMax - 1;
				}
				nowSelectMenu = static_cast<MenuOptions>(nowNum);
				m_selectBack.SetPosition({ (float)C_MenuBaseX, (float)C_MenuBaseY + (C_MenuDistanceY * nowNum) });
			}
		}
		else
		{
			//メニュー開いてない
			return;
		}
	}
}

void Menu::Draw(DrawingByRasterize& arg_rasterize)
{
	if (!m_isMenuOpen) return;

	m_selectBack.Draw(arg_rasterize);

	for (auto itr = m_nonSelectBack.begin(); itr != m_nonSelectBack.end(); ++itr)
	{
		itr->Draw(arg_rasterize);
	}
	m_MenuBackTex.Draw(arg_rasterize);


}

void Menu::UpdateOpen()
{
	//イージングの更新
	for (auto itr = m_nonSelectBack.begin(); itr != m_nonSelectBack.end(); ++itr)
	{
		itr->Update();
	}
	m_selectBack.Update();
	//アニメーション終わったら
	if (m_nonSelectBack[(int)MenuOptions::OptionsMax - 1].GetPosEaseTimer() >= 0.999f)
	{
		m_isNowOpen = false;
	}
}

void Menu::UpdateClose()
{
	//イージングの更新
	for (auto itr = m_nonSelectBack.begin(); itr != m_nonSelectBack.end(); ++itr)
	{
		itr->Update();
	}
	m_selectBack.Update();
	//アニメーション終わったら
	if (m_nonSelectBack[(int)MenuOptions::OptionsMax - 1].GetPosEaseTimer() >= 0.999f)
	{
		m_isMenuOpen = false;
	}
}

void Menu::MenuInit()
{
	nowSelectMenu = MenuOptions::Return;
	m_isMenuOpen = true;
	m_isNowOpen = true;
	m_isNowClose = false;
	float diray = 0.3f;
	float l_easeSpeed = 0.05f;
	for (int i = 0; i < static_cast<int>(MenuOptions::OptionsMax); i++)
	{
		m_nonSelectBack[i].EasePosInit({1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i}, {(float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i }, (-diray * (float)i));
		m_nonSelectBack[i].SetEasePosAddTime(l_easeSpeed);
	}
	m_selectBack.EasePosInit({ 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)nowSelectMenu }, { (float)C_MenuBaseX,(float)C_MenuBaseY }, (-diray * (float)nowSelectMenu));
}

void Menu::MenuClose()
{
	m_isNowClose = true;

	float diray = 0.3f;
	float l_easeSpeed = 0.05f;
	for (int i = 0; i < (int)MenuOptions::OptionsMax; i++)
	{
		m_nonSelectBack[i].EasePosInit({ (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i }, { 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i }, (-diray * (float)i));
		m_nonSelectBack[i].SetEasePosAddTime(l_easeSpeed);
	}
	m_selectBack.EasePosInit({ (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)nowSelectMenu }, { 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)nowSelectMenu }, (-diray * ((float)nowSelectMenu)));
}

Menu::Menu(DrawingByRasterize& arg_rasterize):
	m_MenuBackTex(arg_rasterize, "Resource/MenuTex/MenuBackTex.png"),
	m_selectBack(arg_rasterize, "Resource/MenuTex/MenuSelectBack.png"),
	m_nonSelectBack{
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png"),
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png"),
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png"),
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png")
	}
{
	m_MenuBackTex.SetPosition({1280.0f / 2.0f, 720.0f / 2.0f});
	for (int i = 0; i < (int)MenuOptions::OptionsMax; i++)
	{
		m_nonSelectBack[i].SetPosition({ (float)C_MenuBaseX, (float)C_MenuBaseY + ((float)C_MenuDistanceY * (float)i) });
	}
	m_selectBack.SetPosition({(float)C_MenuBaseX, (float)C_MenuBaseY});
}
