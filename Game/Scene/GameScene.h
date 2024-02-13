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
#include"../Game/UI/CheckPoint.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/Effect/TurretFireEffect.h"
#include"../Game/Effect/InformEnemy.h"
#include"../Game/Effect/Smoke/SmokeEmitter.h"
#include"../Game/Effect/HitEffect/BulletHitWallEffect.h"

class EnemyManager;
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
	GameScene(DrawingByRasterize& arg_rasterize, int f_mapNumber, bool f_isGoal = false);
	~GameScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

	bool OrderGeneratePipeline()
	{
		return m_stageManager.ChangeSceneTrigger();
	}

private:
	std::array<SmokeEmitter::EmittData, 11> m_serverEmittData;
	std::array<SmokeEmitter, 11> m_serverSmokeEmitter;

	bool m_interactFlag;
	KazMath::Timer m_serverErrorTime;
	SoundData m_keySound;
	SoundData m_serverErrorSound;

	//カメラ--------------------------
	bool m_debugCameraFlag;
	DebugCamera m_debuCamera;
	std::shared_ptr<Camera> m_camera;

	KazMath::Transform3D m_stageTransform;

	BasicDraw::BasicModelRender m_titleLogoModel;	//使用するモデル
	BasicDraw::BasicModelRender m_clickToStart;		//使用するモデル
	KazMath::Transform3D m_titleLogoTransform;
	KazMath::Transform3D m_clickToStartTransform;

	//音--------------------------
	SoundData m_bgmHandle, m_seHandle;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<MeshCollision> m_stageMeshCollision;
	std::shared_ptr<BulletMgr> m_bulletMgr;
	std::shared_ptr<EnemyManager> m_enemyManager;
	std::shared_ptr<ThrowableObjectController> m_throwableObjectController;

	int m_sceneNum;
	int m_stageNum;
	bool m_isGoal = false;
	bool m_isToStartPos = false;

	int GetDigits(int arg_value, int arg_m, int arg_n) {
		int mod_value;
		int result;

		/* n桁目以下の桁を取得 */
		mod_value = arg_value % (int)pow(10, arg_n + 1);

		/* m桁目以上の桁を取得 */
		result = mod_value / static_cast<int>(pow(10, arg_m));

		return result;

	}
	BasicDraw::BasicModelRender m_axis;
	KazMath::Transform3D m_axixTransform;

	StageManager m_stageManager;

	WeponUIManager m_uiManager;
	//GadgetUIManager m_gadgetMaanager;
	HPUI m_HPBarManager;
	ResultUI m_resultManager;
	DangerUIManager m_dangerManager;
	IntractUI m_intractUI;

	std::array<BasicDraw::SilhouetteModelRender, 2> m_silhoutteModelArray;
	HeartRate m_heartRateManager;
	Menu m_menu;
	ToDoUI m_todo;

	CheckPoint m_goalPoint;
	bool m_isClear;

	KazMath::Vec3<float>p;
	TurretFireEffect m_turret;
	BasicDraw::BasicTextureRender m_titleTex;
	KazMath::Transform2D m_titleTrans;
	bool m_isTitle = true;

	KazMath::Vec3<float>pos;
	bool isClip;


	//警戒が終わっても一定期間警戒BGMを流すための変数
	float m_emergencyTimer;
	const float EMERGENCY_TIMER = 300.0f;
	
	//一定時間経過したらタイトルロゴを消すためのタイマー
	int m_titleLogoDrawTimer;
	const int TITLELOGO_DRAWTIMER = 120;

	//タイトル用
	const KazMath::Vec3<float> TITLELOGO_POS = KazMath::Vec3<float>(-200.0f, -43.0f, 335.0f);
	float m_titleLogoSineTimer;
	float m_titleLogoSIneRotationTimer;
	const float TITLELOGO_SINE_MOVE = 0.05f;
	int m_titleLogoExitTimer;
	const int TITLELOGO_EXIT_TIMER = 0;
	float m_titleLogoExitEasingTimer;

};