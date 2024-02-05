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
	BasicDraw::BasicModelRender m_mk23MagModel;	//使用するモデル
	KazMath::Transform3D m_weaponTransform;
	KazMath::Transform3D m_magTransform;
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
	float m_shotDelay;
	const float SHOT_DELAY = 20;

	//心音のタイマー
	float m_heatbeatTimer;
	const float HEARTBEAT_TIMER = 60;
	const float HEARTBEAT_TIMER_FOUND = 32;
	SoundData m_heartbeatSE;

	bool m_isFoundToEnemy;

	SoundData m_playerShotSE;
	SoundData m_sonarSE;
	SoundData m_adsSE;
	SoundData m_changeWeaponSE;
	SoundData m_noBullet;

	//姿勢のステータス
	enum class PlayerAttitude {
		STAND,
		SQUAT,
	}m_playerAttitude;

	//リロードに関する変数
	enum class RELOAD_MOTION {
		PHASE_1,	//武器をリロードするとき用の角度に傾ける。(ちょっと左側に傾けて銃口を上に向ける。)
		PHASE_2,	//マガジンを引っこ抜く。
		PHASE_3,	//マガジンを挿入する。
		PHASE_4,	//銃の方向を正しい向きに直す。
	}m_reloadMotionPhase;
	bool m_isReloadMotionNow;
	bool m_isPhase3ShowMag;
	KazMath::Transform3D m_reloadMotionTransform;	//リロードモーションで武器をデフォルトの位置から変えたいときに使用する。
	KazMath::Transform3D m_reloadMotionMagTransform;	//リロードモーションでマガジンをデフォルトの位置から変えたいときに使用する。
	float m_reloadMotionTimer;	//リロードモーションでいろんな使い方をする便利タイマー
	const float PHASE1_UPPER_TIMER = 15;
	const float RELOAD_MOTION_PHASE1_TIMER = 20;
	const float PHASE2_DOWN_TIMER = 10;
	const float RELOAD_MOTION_PHASE2_TIMER = 15;
	const float RELOAD_MOTION_PHASE3_SHOWMAG_TIMER = 5;
	const float RELOAD_MOTION_PHASE3_INSERT_TIMER = 15;
	const float RELOAD_MOTION_PHASE4_TIMER = 20;

	const float RELOAD_MOTION_POSITION_Y_IN_MAG = 0.6f;	//リロードモーション時にちょっとだけ上にあげるための変数。
	const float RELOAD_MOTION_POSITION_Y_OUT_MAG = 0.1f;	//リロードモーション時にちょっとだけ上にあげるための変数。
	const float RELOAD_MOTION_POSITION_Y_INSERT_MAG = 0.2f;	//リロードモーション時にちょっとだけ上にあげるための変数。
	const float RELOAD_MOTION_POSITION_Y_INSERT_END_MAG = 0.25f;	//リロードモーション時にちょっとだけ上にあげるための変数。

	//仮で足跡を描画する用。
	float m_footprintSpan;
	const float FOOTPRINT_SPAN = 1;
	bool m_footprintSide;

	//外と中のBGMを切り替える地点
	const KazMath::Vec3<float> BGM_OUTDOOR_POS = KazMath::Vec3<float>(171.0f, -43.0f, 168.0f);
	const KazMath::Vec3<float> BGM_ROOM_POS = KazMath::Vec3<float>(171.0f, -43.0f, 141.0f);
	const float BGM_CHANGE_SPHERE_RADIUS = 5.0f;

	bool m_isDebug = false;	//壁突き抜け高速移動するやつ。




public:
	//BGMを室内用と室外用で切り替えるための変数
	bool m_inRoom;
	Player(DrawingByRasterize& arg_rasterize, KazMath::Transform3D f_startPos, bool arg_toStartPos);

	void Init();

	void TitleUpdate(std::weak_ptr<Camera> arg_camera, DrawingByRasterize& arg_rasterize, std::list<std::shared_ptr<MeshCollision>> f_stageColliders);

	void Update(std::weak_ptr<Camera> arg_camera, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<BulletMgr> arg_bulletMgr, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController, std::list<std::shared_ptr<MeshCollision>> f_stageColliders, HPUI& arg_hpUI, bool arg_isTitle, bool arg_is1F);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	KazMath::Transform3D GetTransform() { return m_transform; }
	void SetPosition(KazMath::Vec3<float> f_pos) { m_transform.pos = f_pos; }
	bool GetIsADS() { return m_isADS; }


private:

	void Input(std::weak_ptr<Camera> arg_camera, std::weak_ptr<BulletMgr> arg_bulletMgr, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController);
	void UpdateReload();
	void Rotate(std::weak_ptr<Camera> arg_camera);
	void Collision(std::list<std::shared_ptr<MeshCollision>> f_stageColliders);
	float GetMoveSpeed();

};