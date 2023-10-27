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
	m_MenuBackTex.Draw(arg_rasterize);
}

void Menu::UpdateOpen()
{
	//アニメーション終わったら
	if (true)
	{
		m_isNowOpen = false;
	}
}

void Menu::UpdateClose()
{
	//アニメーション終わったら
	if (true)
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
}

void Menu::MenuClose()
{
	m_isNowClose = true;

}

Menu::Menu(DrawingByRasterize& arg_rasterize):
	m_MenuBackTex(arg_rasterize, "Resource/MenuTex/MenuBackTex.png")
{
	m_MenuBackTex.SetPosition({1280.0f / 2.0f, 720.0f / 2.0f});
}
