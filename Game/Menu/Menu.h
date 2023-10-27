#pragma once
#include "../UI/UI.h"

class MenuElement : public UI2DElement
{
public:
	using UI2DElement::UI2DElement;
};

class Menu
{
	enum MenuOptions
	{
		Return,
		Retry,
		Totitle,
		ToEnd,
		OptionsMax,
	};
	MenuOptions nowSelectMenu = MenuOptions::Return;

	MenuElement m_MenuBackTex;

	std::array<MenuElement, OptionsMax> m_nonSelectBack;
	MenuElement m_selectBack;

	bool m_isMenuOpen = false;

	//���j���[�J�������̃A�j���[�V������
	bool m_isNowOpen = false;
	//���j���[�������̃A�j���[�V������
	bool m_isNowClose = false;
	
	//�J���Ă���Ƃ��̃A�j���[�V�����X�V
	void UpdateOpen();
	//���Ă���Ƃ��̃A�j���[�V�����X�V
	void UpdateClose();
	
	//���j���[�̏�����
	void MenuInit();
	//���j���[�̕��鉉�o������
	void MenuClose();

	const int C_MenuBaseX = 1000;
	const int C_MenuBaseY = 100;
	const int C_MenuDistanceY = 50;
public:
	Menu(DrawingByRasterize& arg_rasterize);
	bool GetIsMenuOpen(){ return m_isMenuOpen; };
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
};