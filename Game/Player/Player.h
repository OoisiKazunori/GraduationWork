#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include"../UI/UI.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../Game/UI/UI.h"

class MeshCollision;
class BulletMgr;
class Camera;
class ThrowableObjectController;

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//使用するモデル
	KazMath::Transform3D m_transform;		//モデルの描画に使用するトランスフォーム情報
	KazMath::Vec3<float> m_prevPos;

	BasicDraw::BasicModelRender m_mk23Model;	//使用するモデル
	KazMath::Transform3D m_weaponTransform;
	KazMath::Vec3<float> m_weaponPosOffset;		//銃のモデルを配置するオフセット。ADSしている位置を基準としてADSしていない位置にずらしたりするときに使用する。
	KazMath::Vec3<float> m_gunReaction;
	const float GUN_REACTION = 0.25f;

	bool m_onGround;
	bool m_isADS;		//銃を構えている状態か？

	//当たり判定用のモデル
	BasicDraw::BasicModelRender m_collisionModel;	//使用するモデル
	std::shared_ptr<MeshCollision> m_meshCollision;

	float m_gravity;
	const float GRAVITY = 0.05f;

	//移動速度
	const float MOVE_SPEED_STAND = 0.5f;
	const float MOVE_SPEED_SQUAT = 0.3f;
	const float MOVE_SPEED_CREEPING = 0.15f;

	//発射の遅延
	int m_shotDelay;
	const int SHOT_DELAY = 20;

	//心音のタイマー
	int m_heatbeatTimer;
	const int HEARTBEAT_TIMER = 45;
	const int HEARTBEAT_TIMER_FOUND = 32;
	SoundData m_heartbeatSE;

	bool m_isFoundToEnemy;

	SoundData m_playerShotSE;
	SoundData m_sonarSE;
	SoundData m_adsSE;

	//姿勢のステータス
	enum class PlayerAttitude {
		STAND,
		SQUAT,
	}m_playerAttitude;


public:

	Player(DrawingByRasterize& arg_rasterize, KazMath::Transform3D f_startPos);

	void Init();

	void Update(std::weak_ptr<Camera> arg_camera, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<BulletMgr> arg_bulletMgr, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController, std::list<std::shared_ptr<MeshCollision>> f_stageColliders, HPUI& arg_hpUI);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	KazMath::Transform3D GetTransform() { return m_transform; }
	bool GetIsADS() { return m_isADS; }

private:

	void Input(std::weak_ptr<Camera> arg_camera, std::weak_ptr<BulletMgr> arg_bulletMgr, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController);
	void Rotate(std::weak_ptr<Camera> arg_camera);
	void Collision(std::list<std::shared_ptr<MeshCollision>> f_stageColliders);
	float GetMoveSpeed();

};