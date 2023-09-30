#include "GameScene.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"
#include"Imgui/MyImgui.h"
#include"Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"Math/KazMath.h"

GameScene::GameScene(DrawingByRasterize& arg_rasterize)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
}

void GameScene::PreInit()
{
}

void GameScene::Finalize()
{
}

void GameScene::Input()
{
}

void GameScene::Update()
{
}

void GameScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
}

int GameScene::SceneChange()
{
	return SCENE_NONE;
}
