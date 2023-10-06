#pragma once
#include"../KazLibrary/Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Stage/StageManager.h"

class Player;
class Camera;
class MeshCollision;

class GameScene :public SceneBase
{
public:
	GameScene(DrawingByRasterize& arg_rasterize);
	~GameScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize &arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:

	//ƒJƒƒ‰--------------------------
	DebugCamera m_debuCamera;
	std::shared_ptr<Camera> m_camera;

	//•`‰æ--------------------------
	BasicDraw::BasicTextureRender m_2DSprite, m_3DSprite;
	KazMath::Transform3D m_3DSpriteTransform;
	KazMath::Transform2D m_2DSpriteTransform;

	BasicDraw::BasicModelRender m_modelAnimationRender, m_modelRender;
	KazMath::Transform3D m_modelAnimationTransform, m_modelTransform;

	KazMath::Transform3D m_stageTransform;

	//‰¹--------------------------
	SoundData m_bgmHandle, m_seHandle;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<MeshCollision> m_stageMeshCollision;

	int m_sceneNum;

	int GetDigits(int arg_value, int arg_m, int arg_n) {
		int mod_value;
		int result;

		/* nŒ…–ÚˆÈ‰º‚ÌŒ…‚ğæ“¾ */
		mod_value = arg_value % (int)pow(10, arg_n + 1);

		/* mŒ…–ÚˆÈã‚ÌŒ…‚ğæ“¾ */
		result = mod_value / static_cast<int>(pow(10, arg_m));

		return result;

	}

	BasicDraw::BasicLineRender m_line;
	BasicDraw::BasicModelRender m_stage;

	StageManager m_stageManager;
};