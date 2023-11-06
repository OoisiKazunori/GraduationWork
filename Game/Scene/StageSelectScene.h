#pragma once
#include "../KazLibrary/Scene/SceneBase.h"
#include "../KazLibrary/Buffer/DrawFuncData.h"
#include "../KazLibrary/Render/DrawFunc.h"
#include "../KazLibrary/Sound/SoundManager.h"
#include "../KazLibrary/Loader/ModelLoader.h"
#include "../KazLibrary/Render/DrawFuncHelper.h"
#include "../Game/Debug/DebugCamera.h"
#include "../KazLibrary/Render/BasicDraw.h"
#include "../UI/UI.h"

class Player;
class Camera;

class SelectSceneSprite : public UI2DElement
{
public:
	using UI2DElement::UI2DElement;

};

class StageSelectScene :public SceneBase
{
public:
	StageSelectScene(DrawingByRasterize& arg_rasterize);
	~StageSelectScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//指定のステージに移動する
	int SceneChange();

private:
	//描画--------------------------

	int m_sceneNum;

	int m_nowSelectNum;
	const int C_MaxSelectNum = 1;


	const int UIBaseX = 300;
	const int UIBaseY = 200;

	const int UIDistance = 50;

	SelectSceneSprite m_backSp;
	SelectSceneSprite m_backBarSp;
	SelectSceneSprite m_loadGameSp;
	SelectSceneSprite m_exitGameSp;
	SelectSceneSprite m_selectBackSp;
};