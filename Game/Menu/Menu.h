#pragma once
#include "../UI/UI.h"
#include "../KazLibrary/Scene/SceneData.h"
#include "../KazLibrary/Sound/SoundManager.h"


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
		File,
		Totitle,
		ToEnd,
		OptionsMax
	};

	MenuOptions nowSelectMenu = MenuOptions::Return;

	MenuElement m_MenuBackTex;

	std::array<MenuElement, (int)MenuOptions::OptionsMax> m_nonSelectBack;
	MenuElement m_selectBack;

	MenuElement returnStrTex;
	MenuElement fileStrTex;
	MenuElement toTitleStrTex;
	MenuElement toEndStrTex;
	MenuElement _escTex;

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

	//�t�@�C���̑���
	static const int C_FileCount = 4;
	//�t�@�C���̈ꗗ�����Ă�Ƃ�
	static bool isLookFileList;
	//�t�@�C����I�����Č��Ă�Ƃ�
	static bool isLookFile;


	//�t�@�C���������Ă��邩
	static std::array<bool, C_FileCount> _getFileIndex;
	//�t�@�C����I������Ƃ��̌��o������
	std::array<MenuElement, C_FileCount> _fileMenus;
	//�܂������ĂȂ��t�@�C���͂���ŏo�������ɂ���
	std::array<MenuElement, C_FileCount> _doNotHaveFileMenus;
	//�t�@�C������
	std::array<MenuElement, C_FileCount> _files;
	int _selectFileIndex = 0;
	void ShowFilesInit();
	void ShowFilesDraw();

	SoundData openSE;
	SoundData closeSE;
	SoundData paperSE;
	SoundData arrowSE;
public:
	static bool firstLookFile;
	static int firstLookFileIndex;
	//�����Ă�t�@�C��������������
	//�^�C�g���̂Ƃ��ŏ��������悤����
	static void InitGetFileIndex();
	Menu(DrawingByRasterize& arg_rasterize);
	static bool GetIsGameEnd(){ return isGameEnd; }
	static void SetIsGameEnd(bool f_flag) { isGameEnd = f_flag; }
	static void SetSceneName(SceneName f_sceneName){ m_SceneName = f_sceneName; }
	static void GetFile(int f_index);
	static std::array<bool, C_FileCount> GetNowGetFiles(){return _getFileIndex;};
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
