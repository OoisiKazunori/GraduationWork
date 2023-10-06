#include "TitleScene.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"

TitleScene::TitleScene(DrawingByRasterize& arg_rasterize) :
	m_2DSprite(arg_rasterize, "Resource/SceneUI/Title/TitleLogo.png", true)
{
	m_2DSpriteTransform.pos = { 1280.0f / 2.0f,720.0f / 2.0f - 200.0f };
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	m_sceneNum = SCENE_NONE;
}

void TitleScene::PreInit()
{
}

void TitleScene::Finalize()
{
}

void TitleScene::Input()
{
	//ƒQ[ƒ€ƒV[ƒ“‚Ö
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE))
	{
		m_sceneNum = 1;
	}
}

void TitleScene::Update()
{
}

void TitleScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_2DSprite.m_tex.Draw2D(arg_rasterize, m_2DSpriteTransform);
}

int TitleScene::SceneChange()
{
	if (m_sceneNum != SCENE_NONE)
	{
		int tmp = m_sceneNum;
		m_sceneNum = SCENE_NONE;
		return tmp;
	}
	return SCENE_NONE;
}
