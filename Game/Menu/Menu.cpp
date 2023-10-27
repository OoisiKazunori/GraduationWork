#include "Menu.h"
#include "../Input/Input.h"
#include"../KazLibrary/Render/BasicDraw.h"
void Menu::Init()
{

}

void Menu::Update()
{
	//���j���[��OnOff
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_F3))
	{
		//���j���[���J���n�߂�
		if (!m_isMenuOpen)
		{
			MenuInit();
		}
		//���j���[����
		else
		{
			MenuClose();
		}
	}
	//�J���A�j���[�V������
	if (m_isNowOpen)
	{
		UpdateOpen();
	}
	//����A�j���[�V������
	else if (m_isNowClose)
	{
		UpdateClose();
	}
	else
	{
		//�L�[�̓��͂ɉ����čX�V����
		if (m_isMenuOpen)
		{
			
		}
		else
		{
			//���j���[�J���ĂȂ�
			return;
		}
	}
}

void Menu::Draw(DrawingByRasterize& arg_rasterize)
{
	if (!m_isMenuOpen) return;
	m_MenuBackTex.Draw(arg_rasterize);

	for (auto itr = m_nonSelectBack.begin(); itr != m_nonSelectBack.end(); ++itr)
	{
		itr->Draw(arg_rasterize);
	}
	m_selectBack.Draw(arg_rasterize);
}

void Menu::UpdateOpen()
{
	//�A�j���[�V�����I�������
	if (true)
	{
		m_isNowOpen = false;
	}
}

void Menu::UpdateClose()
{
	//�A�j���[�V�����I�������
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
	m_MenuBackTex(arg_rasterize, "Resource/MenuTex/MenuBackTex.png"),
	m_selectBack(arg_rasterize, "Resource/MenuTex/MenuSelectBack.png")
{
	m_MenuBackTex.SetPosition({1280.0f / 2.0f, 720.0f / 2.0f});
	for (int i = 0; i < MenuOptions::OptionsMax; i++)
	{
		m_nonSelectBack.push_back(MenuElement(arg_rasterize, "Resource/MenuTex/MenuSelectBack.png"));
	}
	int counter = 0;
	for (auto itr = m_nonSelectBack.begin(); itr != m_nonSelectBack.end(); ++itr)
	{
		(*itr).SetPosition({(float)C_MenuBaseX, (float)C_MenuBaseY  + ((float)C_MenuDistanceY * (float)counter)});
	}
	m_selectBack.SetPosition({(float)C_MenuBaseX, (float)C_MenuBaseY});
}
