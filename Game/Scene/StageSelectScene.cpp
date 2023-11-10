#include "StageSelectScene.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Menu/Menu.h"

StageSelectScene::StageSelectScene(DrawingByRasterize& arg_rasterize) :
	m_backSp(arg_rasterize, "Resource/MenuTex/SelectSceneBack.png"),
	m_backBarSp(arg_rasterize, "Resource/MenuTex/SelectSceneBar.png"),
	m_loadGameSp(arg_rasterize, "Resource/MenuTex/LoadGame.png"),
	m_exitGameSp(arg_rasterize, "Resource/MenuTex/exitGame.png"),
	m_selectBackSp(arg_rasterize, "Resource/MenuTex/nowSelectBack.png")
{
	m_backSp.SetPosition({1280 / 2.0f, 720.0f / 2.0f});
	m_backBarSp.SetPosition({ 1280 / 2.0f, 720.0f / 2.0f });

	m_loadGameSp.SetPosition({ -200.0f, (float)UIBaseY });
	m_loadGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY });

	m_exitGameSp.SetPosition({ -200.0f, (float)UIBaseY + (float)UIDistance });
	m_exitGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + (float)UIDistance });
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
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE))
	{
		if (m_nowSelectNum == 0)
		{
			m_sceneNum = 1;
		}
		//ˆê’UƒQ[ƒ€I‚í‚è‚Æ‚·‚é
		else if (m_nowSelectNum == 1)
		{
			Menu::SetIsGameEnd(true);
		}
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_S) || KeyBoradInputManager::Instance()->InputTrigger(DIK_DOWN))
	{
		m_nowSelectNum++;
		if (m_nowSelectNum > C_MaxSelectNum)
		{
			m_nowSelectNum = 0;
		}
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_W) || KeyBoradInputManager::Instance()->InputTrigger(DIK_UP))
	{
		m_nowSelectNum--;
		if (m_nowSelectNum < 0)
		{
			m_nowSelectNum = C_MaxSelectNum;
		}
	}
}

void StageSelectScene::Update(DrawingByRasterize& arg_rasterize)
{
	m_loadGameSp.Update();
	m_exitGameSp.Update();
}

void StageSelectScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_backBarSp.Draw(arg_rasterize);
	m_exitGameSp.Draw(arg_rasterize);
	m_loadGameSp.Draw(arg_rasterize);

	m_selectBackSp.SetPosition({ (float)UIBaseX, (float)UIBaseY + (float)(UIDistance * m_nowSelectNum)});
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
