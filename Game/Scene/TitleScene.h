#pragma once
#include"../KazLibrary/Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"
#include"../KazLibrary/Render/BasicDraw.h"

class Player;
class Camera;

class TitleScene :public SceneBase
{
public:
	TitleScene(DrawingByRasterize& arg_rasterize);
	~TitleScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:
		//•`‰æ--------------------------
	BasicDraw::BasicTextureRender m_2DSprite;
	BasicDraw::BasicTextureRender m_space;
	KazMath::Transform2D m_2DSpriteTransform;
	KazMath::Transform2D m_spaceTransform;

	std::shared_ptr<Player> m_player;

	int m_sceneNum;
	int m_inputDiray = 0;
	const int C_InputDiray = 45;
	int m_spaceColor = 255;
	int m_spaceAddColor = 2;
};