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
	const int C_MenuBaseY = 200;
	const int C_MenuDistanceY = 50;

	static SceneName m_SceneName;
	static bool isSceneChange;
	static bool isSceneChangeTrigger;
public:
	Menu(DrawingByRasterize& arg_rasterize);
	static bool GetIsGameEnd(){ return isGameEnd; }
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