#include "Menu.h"
#include "../Input/Input.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include "../Scene/StageSelectScene.h"
bool Menu::isGameEnd = false;

SceneName Menu::m_SceneName;
bool Menu::isSceneChange = false;
bool Menu::isSceneChangeTrigger = false;
bool Menu::isLookFileList;
bool Menu::isLookFile;
std::array<bool, Menu::C_FileCount> Menu::_getFileIndex = { false, false, false, false };

bool Menu::firstLookFile = false;
int Menu::firstLookFileIndex = -1;

void Menu::Init()
{
	
}

void Menu::Update()
{
	//ファイルを初めて開いたとき
	if (firstLookFile)
	{
		if (!m_isMenuOpen)
		{
			SoundManager::Instance()->SoundPlayerWave(paperSE, 0);
		}
		m_isMenuOpen = true;
		if (firstLookFileIndex < 0)
		{
			//
		}
		else
		{
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_ESCAPE) || KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
			{
				firstLookFile = false;
				m_isMenuOpen = false;
				SoundManager::Instance()->SoundPlayerWave(closeSE, 0);
			}
		}
	}
	else
	{
		//メニューのOnOff
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_ESCAPE) && !m_isNowOpen && !m_isNowClose)
		{
			//メニューを開き始める
			if (!m_isMenuOpen)
			{
				SoundManager::Instance()->SoundPlayerWave(openSE, 0);
				MenuInit();
			}
			//メニュー閉じる
			else
			{
				//一気にメニューが閉じないようなシステム
				if (!isLookFileList && !isLookFile)
				{
					SoundManager::Instance()->SoundPlayerWave(closeSE, 0);
					MenuClose();
				}
				else if (isLookFileList && !isLookFile)
				{
					isLookFileList = false;
					isLookFile = false;
					SoundManager::Instance()->SoundPlayerWave(closeSE, 0);
				}
				else if (isLookFile)
				{
					isLookFile = false;
					SoundManager::Instance()->SoundPlayerWave(closeSE, 0);
				}
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
					if (!isLookFileList && !isLookFile)
					{
						int nowNum = static_cast<int>(nowSelectMenu);
						nowNum += 1;
						if (nowNum >= static_cast<int>(MenuOptions::OptionsMax))
						{
							nowNum = 0;
						}
						nowSelectMenu = static_cast<MenuOptions>(nowNum);
						m_selectBack.SetPosition({ (float)C_MenuBaseX, (float)C_MenuBaseY + (C_MenuDistanceY * nowNum) });
						SoundManager::Instance()->SoundPlayerWave(arrowSE, 0);
					}
					else if (isLookFileList && !isLookFile)
					{
						_selectFileIndex++;
						if (_selectFileIndex >= C_FileCount)
						{
							_selectFileIndex = 0;
						}
						SoundManager::Instance()->SoundPlayerWave(arrowSE, 0);
					}
				}
				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_W) || KeyBoradInputManager::Instance()->InputTrigger(DIK_UP))
				{
					if (!isLookFileList && !isLookFile)
					{
						int nowNum = static_cast<int>(nowSelectMenu);
						nowNum -= 1;
						if (nowNum < 0)
						{
							nowNum = (int)MenuOptions::OptionsMax - 1;
						}
						nowSelectMenu = static_cast<MenuOptions>(nowNum);
						m_selectBack.SetPosition({ (float)C_MenuBaseX, (float)C_MenuBaseY + (C_MenuDistanceY * nowNum) });
						SoundManager::Instance()->SoundPlayerWave(arrowSE, 0);
					}
					else if (isLookFileList && !isLookFile)
					{
						_selectFileIndex--;
						if (_selectFileIndex < 0)
						{
							_selectFileIndex = C_FileCount - 1;
						}
						SoundManager::Instance()->SoundPlayerWave(arrowSE, 0);
					}
				}

				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE) || KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
				{
					if (!isLookFileList)
					{
						if (nowSelectMenu == MenuOptions::Return)
						{
							SoundManager::Instance()->SoundPlayerWave(closeSE, 0);
							MenuClose();
						}
						else if (nowSelectMenu == MenuOptions::Totitle)
						{
							isSceneChangeTrigger = true;
							SetSceneName(SceneName::SCENE_TUTORIAL);
							SoundManager::Instance()->SoundPlayerWave(arrowSE, 0);
							StageSelectScene::startStageNum = 0;
							isSceneChange = true;
							m_isMenuOpen = false;
						}
						else if (nowSelectMenu == MenuOptions::File)
						{
							//ファイルをMenuを見せる処理
							SoundManager::Instance()->SoundPlayerWave(arrowSE, 0);
							ShowFilesInit();
						}
						else if (nowSelectMenu == MenuOptions::ToEnd)
						{
							isGameEnd = true;
						}
					}
					else if (isLookFileList && !isLookFile)
					{
						if (_getFileIndex[_selectFileIndex])
						{
							isLookFile = true;
							SoundManager::Instance()->SoundPlayerWave(paperSE, 0);
						}
						else
						{
							//開けない時
							//SEでもならすか
						}
					}
				}
			}
			else
			{
				//メニュー開いてない
				return;
			}
		}
	}
}

void Menu::Draw(DrawingByRasterize& arg_rasterize)
{
	if (firstLookFile)
	{
		if (firstLookFileIndex < 0)
		{
			//
		}
		else
		{
			KazMath::Transform2D l_trans = { {1280.0f / 2.0f ,720.0f / 2.0f}, {1.0f, 1.0f} };
			_files[firstLookFileIndex].m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
		}
		_escTex.m_2DSprite.m_tex.Draw2D(arg_rasterize, { {50.0f, 690.0f}, {0.5f, 0.5f} });
		m_MenuBackTex.Draw(arg_rasterize);
		
		return;
	}
	if (!m_isMenuOpen) return;
	if (!isLookFileList && !isLookFile)
	{
		returnStrTex.Draw(arg_rasterize);
		fileStrTex.Draw(arg_rasterize);
		toTitleStrTex.Draw(arg_rasterize);
		toEndStrTex.Draw(arg_rasterize);

		m_selectBack.Draw(arg_rasterize);

		for (auto itr = m_nonSelectBack.begin(); itr != m_nonSelectBack.end(); ++itr)
		{
			itr->Draw(arg_rasterize);
		}
	}
	else
	{
		if (!isLookFile)
		{
			//ファイルリストを魅せる処理
			for (int i = 0; i < C_FileCount; i++)
			{
				KazMath::Transform2D l_trans = { {(float)C_MenuBaseX ,(float)C_MenuBaseY + (C_MenuDistanceY * i)}, {1.0f, 1.0f} };
				if (_getFileIndex[i])
				{
					_fileMenus[i].m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
				}
				else
				{
					_doNotHaveFileMenus[i].m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
				}
				if (_selectFileIndex == i)
				{
					m_selectBack.m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
				}
				m_nonSelectBack[i].m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
			}
		}
		else
		{
			KazMath::Transform2D l_trans = { {1280.0f / 2.0f ,720.0f / 2.0f}, {1.0f, 1.0f} };
			_files[_selectFileIndex].m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
		}
	}
	//_escTex.SetPosition({20.0f, 680.0f});
	_escTex.m_2DSprite.m_tex.Draw2D(arg_rasterize, {{50.0f, 690.0f}, {0.5f, 0.5f}});
	//_escTex.Draw(arg_rasterize);
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

	returnStrTex.Update();
	fileStrTex.Update();
	toTitleStrTex.Update();
	toEndStrTex.Update();

	//アニメーション終わったら
	if (toEndStrTex.GetPosEaseTimer() >= 0.999f)
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

	returnStrTex.Update();
	fileStrTex.Update();
	toTitleStrTex.Update();
	toEndStrTex.Update();

	//アニメーション終わったら
	if (m_nonSelectBack[(int)MenuOptions::OptionsMax - 1].GetPosEaseTimer() >= 0.999f)
	{
		m_isMenuOpen = false;
		m_isNowClose = false;
	}
}

void Menu::MenuInit()
{
	nowSelectMenu = MenuOptions::Return;
	m_isMenuOpen = true;
	m_isNowOpen = true;
	m_isNowClose = false;
	float diray = 0.3f;
	float l_easeSpeed = 0.08f;
	for (int i = 0; i < static_cast<int>(MenuOptions::OptionsMax); i++)
	{
		m_nonSelectBack[i].EasePosInit({ 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i }, { (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i }, (-diray * (float)i));
		m_nonSelectBack[i].SetEasePosAddTime(l_easeSpeed);
	}
	m_selectBack.EasePosInit({ 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)nowSelectMenu }, { (float)C_MenuBaseX,(float)C_MenuBaseY }, (-diray * (float)nowSelectMenu));
	m_selectBack.SetEasePosAddTime(l_easeSpeed);

	int l_lineCount = 0;
	const float strDiray = 0.4f;
	returnStrTex.EasePosInit({ 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount - strDiray));
	returnStrTex.SetEasePosAddTime(l_easeSpeed);
	l_lineCount++;
	fileStrTex.EasePosInit({ 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount - strDiray));
	fileStrTex.SetEasePosAddTime(l_easeSpeed);
	l_lineCount++;
	toTitleStrTex.EasePosInit({ 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount - strDiray));
	toTitleStrTex.SetEasePosAddTime(l_easeSpeed);
	l_lineCount++;
	toEndStrTex.EasePosInit({ 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount - strDiray));
	toEndStrTex.SetEasePosAddTime(l_easeSpeed);
}

void Menu::MenuClose()
{
	m_isNowClose = true;

	float diray = 0.3f;
	float l_easeSpeed = 0.08f;
	for (int i = 0; i < (int)MenuOptions::OptionsMax; i++)
	{
		m_nonSelectBack[i].EasePosInit({ (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i }, { 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)i }, (-diray * (float)i));
		m_nonSelectBack[i].SetEasePosAddTime(l_easeSpeed);
	}
	m_selectBack.EasePosInit({ (float)C_MenuBaseX,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)nowSelectMenu }, { 1280.0f + 500.0f, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)nowSelectMenu }, (-diray * ((float)nowSelectMenu)));
	m_selectBack.SetEasePosAddTime(l_easeSpeed);

	int l_lineCount = 0;
	returnStrTex.EasePosInit({ (float)C_MenuBaseX, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { 1280.0f + 500.0f,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount));
	l_lineCount++;
	fileStrTex.EasePosInit({ (float)C_MenuBaseX, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { 1280.0f + 500.0f,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount));
	l_lineCount++;
	toTitleStrTex.EasePosInit({ (float)C_MenuBaseX, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { 1280.0f + 500.0f,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount));
	l_lineCount++;
	toEndStrTex.EasePosInit({ (float)C_MenuBaseX, (float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, { 1280.0f + 500.0f,(float)C_MenuBaseY + (float)C_MenuDistanceY * (float)l_lineCount }, (-diray * (float)l_lineCount));
}

void Menu::ShowFilesInit()
{
	isLookFileList = true;
	isLookFile = false;
	_selectFileIndex = 0;
}

void Menu::ShowFilesDraw()
{
}

void Menu::InitGetFileIndex()
{
	for (int i = 0; i < C_FileCount; i++)
	{
		_getFileIndex[i] = false;
	}
}

Menu::Menu(DrawingByRasterize& arg_rasterize) :
	m_MenuBackTex(arg_rasterize, "Resource/MenuTex/MenuBackTex.png"),
	m_selectBack(arg_rasterize, "Resource/MenuTex/MenuSelectBack.png"),
	m_nonSelectBack{
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png"),
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png"),
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png"),
		MenuElement(arg_rasterize, "Resource/MenuTex/MenuNonSelectBack.png")
},
returnStrTex(arg_rasterize, "Resource/MenuTex/MenuReturn.png"),
fileStrTex(arg_rasterize, "Resource/MenuTex/File.png"),
toTitleStrTex(arg_rasterize, "Resource/MenuTex/MenuTitle.png"),
toEndStrTex(arg_rasterize, "Resource/MenuTex/MenuEnd.png"),
_escTex(arg_rasterize, "Resource/UITexture/ESC.png"),
_fileMenus{
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name1.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name2.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name3.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name4.png")
},
_doNotHaveFileMenus{
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name5.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name5.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name5.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File_Name5.png")
},
_files{
	MenuElement(arg_rasterize, "Resource/MenuTex/File1.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File2.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File3.png"),
	MenuElement(arg_rasterize, "Resource/MenuTex/File4.png")
}
{
	m_MenuBackTex.SetPosition({ 1280.0f / 2.0f, 720.0f / 2.0f });
	for (int i = 0; i < (int)MenuOptions::OptionsMax; i++)
	{
		m_nonSelectBack[i].SetPosition({ (float)C_MenuBaseX, (float)C_MenuBaseY + ((float)C_MenuDistanceY * (float)i) });
	}
	m_selectBack.SetPosition({ (float)C_MenuBaseX, (float)C_MenuBaseY });

	returnStrTex.SetPosition({ 1280.0f + 500.0f, 720.0f / 2.0f });
	fileStrTex.SetPosition({ 1280.0f + 500.0f, 720.0f / 2.0f });
	toTitleStrTex.SetPosition({ 1280.0f + 500.0f, 720.0f / 2.0f });
	toEndStrTex.SetPosition({ 1280.0f + 500.0f, 720.0f / 2.0f });
	openSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Menu/Menu_SE_1_.wav");
	openSE.volume = 0.06f;
	closeSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Menu/Menu_SE_2_.wav");
	closeSE.volume = 0.06f;
	paperSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Menu/Paper_SE_1_.wav");
	paperSE.volume = 0.06f;
	arrowSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/UI_Click.wav");
	arrowSE.volume = 0.06f;
}

void Menu::GetFile(int f_index)
{
	if (f_index <= -1 || f_index >= _getFileIndex.size()) return;
	_getFileIndex[f_index] = true;
}
