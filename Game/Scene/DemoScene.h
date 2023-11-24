#pragma once
#include"Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../KazLibrary/Render/BasicDraw.h"

class DemoScene :public SceneBase
{
public:
	DemoScene(DrawingByRasterize& arg_rasterize);
	~DemoScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize &arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:
	//ÉJÉÅÉâ--------------------------
	DebugCamera m_camera;

	//ï`âÊ--------------------------
	BasicDraw::BasicTextureRender m_2DSprite, m_3DSprite;
	KazMath::Transform3D m_3DSpriteTransform;
	KazMath::Transform2D m_2DSpriteTransform;

	BasicDraw::BasicModelRender m_modelAnimationRender,m_modelRender;
	KazMath::Transform3D m_modelAnimationTransform,m_modelTransform;

	//âπ--------------------------
	SoundData m_bgmHandle,m_seHandle;

	int m_sceneNum;
};

