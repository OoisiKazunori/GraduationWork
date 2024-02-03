#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "PatrolData.h"
#include "../Echo/EchoArray.h"
#include "../Game/Collision/MeshCollision.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../Game/UI/Reaction.h"
#include"../Game/AI/EnemyAIData.h"
#include <memory>
#include"../Game/Effect/InformEnemy.h"
#include<memory>

class MeshCollision;
class BulletMgr;

class Enemy
{
private:
	std::shared_ptr<
		BasicDraw::BasicModelRender> m_enemyBox;
	std::shared_ptr<
		BasicDraw::BasicModelRender> m_pedestal;

	std::shared_ptr<MeshCollision> m_meshCol;

	BasicDraw::BasicLineRender m_line;

	bool m_isCombat;

private:
	std::list<KazMath::Transform3D> m_positions;
	const int CHECK_POINT_DELAY = 240;
	int m_currentPoint = 0;
	KazMath::Vec3<float> m_moveVec;
	KazMath::Vec3<float> m_nextPos;
	float m_rotRate = 0.0f;

	std::vector<std::pair<int, int>> m_checkPointDelay;
	KazMath::Transform3D m_trans;
	KazMath::Vec3<float> m_prevPos;	//前フレーム座標
	State m_state, m_oldState;
	int m_delayNum;
	//int m_count;
	int m_delay;
	bool m_isCheckPoint;
	bool m_onGround;

	SoundData m_enemyShotSE;

	std::vector<std::pair<float, float>> m_checkSoundPos;
	int m_checkSoundCount;
	bool m_isReturn;
	KazMath::Vec3<float> m_oldPos;

	float m_offset_x;
	float m_offset_y;

	float m_gravity;
	const float GRAVITY = 0.05f;

	const  int MAX_RATE = 300;
	const int MAX_HP = 2;
	int m_hp;
	int m_rate;

	//バレるまでの時間
	const int MAX_EYE_DELAY = 120;
	int m_checkEyeDelay;

	//どうしよう
	float m_angle = 0.0f;
	DirectX::XMVECTOR m_oldQuaternion;


	//敵が弾を撃つ遅延 デバッグ用 後で書き換えてください
	int m_shotDelay;
	const int SHOT_DELAY = 15;

	bool m_inEcho = false;
	int m_appearTimer; //敵が表示中の時間
	static const int APPEAR_TIMER = 180;
	//UI
	Reaction m_reaction;

	//仮で足跡を描画する用。
	float m_footprintSpan;
	const float FOOTPRINT_SPAN = 5;
	bool m_footprintSide;


	bool m_isInSightFlag;//視界内に入ったか

	InformEnemy m_inform;

public:
	Enemy();
	~Enemy();
	void Init(
		std::list<KazMath::Transform3D> arg_enemyList);
	void Update(
		std::list<std::shared_ptr<MeshCollision>>
		arg_stageColliders,
		std::weak_ptr<BulletMgr> arg_bulletMgr,
		KazMath::Vec3<float> arg_playerPos,
		std::weak_ptr<MeshCollision> arg_stageMeshCollision
	);
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

	void CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision)
	{
		//全てのEchoとチェック
		for (auto& index : EchoArray::Instance()->GetEcho()) {

			//エコーが生成されていなかったら。
			if (!index.GetIsActive()) continue;
			if (index.GetNowRadius() <= 0.1f) continue;

			//まずは球で当たり判定を行う。
			KazMath::Vec3<float> echoVec = m_trans.pos - index.GetPos();
			float distance = echoVec.Length();
			if (index.GetNowRadius() <= distance) continue;

			//次にレイを飛ばして当たり判定を行う。
			MeshCollision::CheckHitResult result = arg_stageMeshCollision.lock()->CheckHitRay(index.GetPos(), echoVec.GetNormal());

			//当たっていたら
			if (!result.m_isHit || (result.m_isHit && distance <= fabs(result.m_distance))) {

				m_inEcho = true;
				break;

			}

		}

	}

private:
	DirectX::XMVECTOR CalMoveQuaternion(
		KazMath::Vec3<float> arg_pos,
		KazMath::Vec3<float> arg_prevPos);
	void Collision(
		std::list<std::shared_ptr<MeshCollision>>
		arg_stageColliders,
		std::weak_ptr<BulletMgr> arg_bulletMgr);
	void RotateEye();
	bool CheckDistXZ(
		std::pair<float, float> arg_checkPos, float arg_dist);
	bool CheckEye(
		KazMath::Vec3<float> arg_playerPos,
		std::list<std::shared_ptr<MeshCollision>>
		arg_stageColliders);

public:
	KazMath::Transform3D GetTrans() { return m_trans; }
	KazMath::Vec3<float> GetPos() { return m_trans.pos; }
	bool IsDiscovery() {
		if (m_isCombat) { return true; }
		return false;
	}

public:
	void SetData(
		DrawingByRasterize& arg_rasterize);
	void SetCheckPointDelay(
		std::vector<std::pair<int, int>> arg_checkPointDelay);
	void SetState(State arg_state) { m_state = arg_state; }

public:
	void SetRootPos(
		std::vector<std::pair<float, float>> arg_rootPos) {
		//m_rootPos = arg_rootPos;
	}
	void SetCheckSoundPos(
		std::vector<std::pair<float, float>> arg_checkSoundPos) {
		m_isReturn = false;
		m_checkSoundCount = 0;
		m_checkSoundPos = arg_checkSoundPos;
	}

public:
	void SetOffset(std::pair<float, float> arg_offsets) {
		m_offset_x = arg_offsets.first;
		m_offset_y = arg_offsets.second;
	}


private:
	void Move();
	void CalcMoveVec();
};
