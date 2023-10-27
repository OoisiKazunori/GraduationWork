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
	MenuOptions nowSelectMenu;

	MenuElement m_MenuBackTex;

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
public:
	Menu(DrawingByRasterize& arg_rasterize);
	bool GetIsMenuOpen(){ return m_isMenuOpen; };
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
};
