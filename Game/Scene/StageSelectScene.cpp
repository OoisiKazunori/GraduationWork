#include "StageSelectScene.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Menu/Menu.h"

StageSelectScene::StageSelectScene(DrawingByRasterize& arg_rasterize) :
	m_backSp(arg_rasterize, "Resource/MenuTex/SelectSceneBack.png"),
	m_backBarSp(arg_rasterize, "Resource/MenuTex/SelectSceneBar.png"),
	m_loadGameSp(arg_rasterize, "Resource/MenuTex/LoadGame.png"),
	m_exitGameSp(arg_rasterize, "Resource/MenuTex/exitGame.png"),
	m_selectBackSp(arg_rasterize, "Resource/MenuTex/nowSelectBack.png"),

	m_MouseReveralSp(arg_rasterize, "Resource/MenuTex/nowSelectBack.png"),
	m_MouseSensSp(arg_rasterize, "Resource/MenuTex/nowSelectBack.png"),
	m_VolumeSp(arg_rasterize, "Resource/MenuTex/nowSelectBack.png")
{
	m_backSp.SetPosition({ 1280 / 2.0f, 720.0f / 2.0f });
	m_backBarSp.SetPosition({ 1280 / 2.0f, 720.0f / 2.0f });

	m_loadGameSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ToGame) });
	m_loadGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)ToGame) });

	m_selectBackSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });
	m_selectBackSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });

	m_exitGameSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ExitGame) });
	m_exitGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)ExitGame) });

	m_sceneNum = -1;
	m_nowSelectNum = 0;


}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Init()
{
}

void StageSelectScene::PreInit()
{
}

void StageSelectScene::Finalize()
{
}

void StageSelectScene::Input()
{
	if (!m_isOptionsOpen)
	{
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE))
		{
			if (m_nowSelectNum == ToGame)
			{
				//1”Ô‚ÌƒQ[ƒ€‚És‚­
				m_sceneNum = 1;
			}
			else if (m_nowSelectNum == SelectOpsions::Opsions)
			{
				m_isOptionsOpen = true;
			}
			//ˆê’UƒQ[ƒ€I‚í‚è‚Æ‚·‚éŒã‚Å”Ô†‚ð•Ï‚¦‚é
			else if (m_nowSelectNum == ExitGame)
			{
				Menu::SetIsGameEnd(true);
			}
		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_S) || KeyBoradInputManager::Instance()->InputTrigger(DIK_DOWN))
		{
			m_nowSelectNum++;
			if (m_nowSelectNum > ExitGame)
			{
				m_nowSelectNum = 0;
			}
		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_W) || KeyBoradInputManager::Instance()->InputTrigger(DIK_UP))
		{
			m_nowSelectNum--;
			if (m_nowSelectNum < 0)
			{
				m_nowSelectNum = ExitGame;
			}
		}
	}

	else
	{
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE))
		{
			if (m_OptionsOpenSelect == -1)
			{
				m_OptionsOpenSelect = m_opsionsSelectNum;
			}
			else
			{
				switch (m_OptionsOpenSelect)
				{
				case OptionsOpstions::MouseReversal:
					
					break;
				case OptionsOpstions::MouseSens:

					break;
				case OptionsOpstions::Volume:

					break;
				default:
					break;
				}
			}
		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_W) || KeyBoradInputManager::Instance()->InputTrigger(DIK_UP))
		{
			if (m_OptionsOpenSelect == -1)
			{
				m_opsionsSelectNum--;
				if (m_opsionsSelectNum < 0)
				{
					m_opsionsSelectNum = OptionsOpstions::Volume;
				}
			}
			else
			{

			}
		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_S) || KeyBoradInputManager::Instance()->InputTrigger(DIK_DOWN))
		{
			if (m_OptionsOpenSelect == -1)
			{
				m_opsionsSelectNum++;
				if (m_opsionsSelectNum > OptionsOpstions::Volume)
				{
					m_opsionsSelectNum = OptionsOpstions::MouseReversal;
				}
			}
			else
			{

			}
		}
		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_ESCAPE))
		{
			if (m_OptionsOpenSelect == -1)
			{
				m_isOptionsOpen = false;
			}
		}
	}
}

void StageSelectScene::Update(DrawingByRasterize& arg_rasterize)
{
	m_loadGameSp.Update();
	m_selectBackSp.Update();
	m_exitGameSp.Update();
}

void StageSelectScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_backBarSp.Draw(arg_rasterize);
	m_exitGameSp.Draw(arg_rasterize);
	m_loadGameSp.Draw(arg_rasterize);

	m_selectBackSp.Draw(arg_rasterize);

	m_backSp.Draw(arg_rasterize);
}

int StageSelectScene::SceneChange()
{
	if (m_sceneNum != SCENE_NONE)
	{
		int tmp = m_sceneNum;
		m_sceneNum = SCENE_NONE;
		return tmp;
	}
	return SCENE_NONE;
}

void StageSelectScene::OpenOptionsInit()
{

	m_loadGameSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ToGame) });
	m_selectBackSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });
	m_exitGameSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ExitGame) });


}
