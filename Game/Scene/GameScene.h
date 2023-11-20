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
#include "../UI/UI.h"
#include "../Menu/Menu.h"

class Player;
class Camera;
class MeshCollision;
class BulletMgr;
class ThrowableObjectController;

//デバッグ用
class PreEnemy;

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

	bool OrderGeneratePipeline()
	{
		return m_stageManager.ChangeSceneTrigger();
	}

private:

	//カメラ--------------------------
	DebugCamera m_debuCamera;
	std::shared_ptr<Camera> m_camera;

	std::array<std::shared_ptr<PreEnemy>, 3> m_preEnemy;

	KazMath::Transform3D m_stageTransform;

	//音--------------------------
	SoundData m_bgmHandle, m_seHandle;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<MeshCollision> m_stageMeshCollision;
	std::shared_ptr<BulletMgr> m_bulletMgr;
	std::shared_ptr<ThrowableObjectController> m_throwableObjectController;

	int m_sceneNum;

	int GetDigits(int arg_value, int arg_m, int arg_n) {
		int mod_value;
		int result;

		/* n桁目以下の桁を取得 */
		mod_value = arg_value % (int)pow(10, arg_n + 1);

		/* m桁目以上の桁を取得 */
		result = mod_value / static_cast<int>(pow(10, arg_m));

		return result;

	}

	BasicDraw::BasicLineRender m_line;
	BasicDraw::BasicModelRender m_stage;
	BasicDraw::BasicTextureRender m_outlineTex;

	StageManager m_stageManager;
	
	WeponUIManager m_uiManager;
	GadgetUIManager m_gadgetMaanager;
	HPUI m_HPBarManager;

	std::array<BasicDraw::SilhouetteModelRender,2> m_silhoutteModelArray;
	HeartRate m_heartRateManager;
	Menu m_menu;
};