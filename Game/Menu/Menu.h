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

	//メニュー開いた時のアニメーション中
	bool m_isNowOpen = false;
	//メニュー閉じた時のアニメーション中
	bool m_isNowClose = false;
	
	//開いているときのアニメーション更新
	void UpdateOpen();
	//閉じているときのアニメーション更新
	void UpdateClose();
	
	//メニューの初期化
	void MenuInit();
	//メニューの閉じる演出初期化
	void MenuClose();
public:
	Menu(DrawingByRasterize& arg_rasterize);
	bool GetIsMenuOpen(){ return m_isMenuOpen; };
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
};
