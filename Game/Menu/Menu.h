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

	//ファイルの総数
	static const int C_FileCount = 4;
	//ファイルの一覧を見てるとき
	static bool isLookFileList;
	//ファイルを選択して見てるとき
	static bool isLookFile;


	//ファイルを持っているか
	static std::array<bool, C_FileCount> _getFileIndex;
	//ファイルを選択するときの見出し文字
	std::array<MenuElement, C_FileCount> _fileMenus;
	//まだ持ってないファイルはこれで出す横線にする
	std::array<MenuElement, C_FileCount> _doNotHaveFileMenus;
	//ファイル自体
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
	//持ってるファイルを初期化する
	//タイトルのとこで初期化しようかな
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
