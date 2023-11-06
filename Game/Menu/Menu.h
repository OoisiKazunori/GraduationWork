#pragma once
#include "../UI/UI.h"
#include "../KazLibrary/Scene/SceneData.h"

class MenuElement : public UI2DElement
{
public:
	using UI2DElement::UI2DElement;

};

class Menu
{
	enum class MenuOptions
	{
		Return,
		Totitle,
		ToEnd,
		OptionsMax
	};

	MenuOptions nowSelectMenu = MenuOptions::Return;

	MenuElement m_MenuBackTex;

	std::array<MenuElement, (int)MenuOptions::OptionsMax> m_nonSelectBack;
	MenuElement m_selectBack;

	MenuElement returnStrTex;
	MenuElement toTitleStrTex;
	MenuElement toEndStrTex;

	static bool isGameEnd;

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

	const int C_MenuBaseX = 1000;
	const int C_MenuBaseY = 200;
	const int C_MenuDistanceY = 50;

	static SceneName m_SceneName;
	static bool isSceneChange;
	static bool isSceneChangeTrigger;
public:
	Menu(DrawingByRasterize& arg_rasterize);
	static bool GetIsGameEnd(){ return isGameEnd; }
	static void SetIsGameEnd(bool f_flag) { isGameEnd = f_flag; }
	static void SetSceneName(SceneName f_sceneName){ m_SceneName = f_sceneName; }
	static bool IsSceneChange()
	{
		if (!isSceneChange)return false;
		else
		{
			isSceneChange = false;
			return true;
		}
	}
	static SceneName GetNextSceneName(){ return m_SceneName; };
	bool GetIsMenuOpen(){ return m_isMenuOpen; };
	void Init();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);
};
