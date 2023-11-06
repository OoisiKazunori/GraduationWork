#include "StageSelectScene.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"


StageSelectScene::StageSelectScene(DrawingByRasterize& arg_rasterize) :
	m_backSp(arg_rasterize, "Resource/MenuTex/SelectSceneBack.png"),
	m_backBarSp(arg_rasterize, "Resource/MenuTex/SelectSceneBar.png"),
	m_loadGameSp(arg_rasterize, "Resource/MenuTex/LoadGame.png")
{
	m_backSp.SetPosition({1280 / 2.0f, 720.0f / 2.0f});
	m_backBarSp.SetPosition({ 1280 / 2.0f, 720.0f / 2.0f });
	m_loadGameSp.SetPosition({ (float)UIBaseX, (float)UIBaseY });
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
		m_sceneNum = 1;
	}
}

void StageSelectScene::Update(DrawingByRasterize& arg_rasterize)
{
	
}

void StageSelectScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	
	m_backBarSp.Draw(arg_rasterize);
	m_loadGameSp.Draw(arg_rasterize);
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
