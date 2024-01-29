#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "PatrolData.h"
#include "../Game/Collision/MeshCollision.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../Game/UI/Reaction.h"
#include<memory>

class MeshCollision;
class BulletMgr;

class Enemy
{
public:
	enum struct State
	{
		Patrol,		//巡回
		Warning,	//警戒
		Combat,		//戦闘
		Holdup,		//ホールドアップ(消えそう)
		Death		//死亡
	};

private:
	std::shared_ptr<
		BasicDraw::BasicModelRender> m_enemyBox;
	std::shared_ptr<MeshCollision> m_meshCol;
	BasicDraw::BasicLineRender m_line;
	SoundData m_enemyShotSE;

private:
	std::vector<std::pair<float, float>> m_rootPos;
	std::vector<std::pair<int, int>> m_checkPointDelays;
	std::vector<std::pair<float, float>> m_checkSoundPos;
	KazMath::Transform3D m_trans;
	KazMath::Vec3<float> m_oldPos;
	KazMath::Vec3<float> m_prevPos;	//前フレーム座標
	State m_state,m_oldState;
	int m_delayNum;
	int m_count;
	int m_checkPointDelay;
	int m_hp;
	int m_changePatrolDelay;
	int m_changeCombatDelay;
	int m_checkSoundCount;
	float m_offset_x;
	float m_offset_y;
	float m_gravity;
	bool m_isCombatTri;
	bool m_isCheckPoint;
	bool m_onGround;
	bool m_isReturn;

	//どうしよう
	float m_angle = 0.0f;
	DirectX::XMVECTOR m_oldQuaternion;

	//敵が弾を撃つ遅延 デバッグ用 後で書き換えてください
	int m_shotDelay;

	bool m_inEcho = false;
	int m_appearTimer; //敵が表示中の時間
	static const int APPEAR_TIMER = 180;
	//UI
	Reaction m_reaction;

	//仮で足跡を描画する用。
	float m_footprintSpan;
	const float FOOTPRINT_SPAN = 5;
	bool m_footprintSide;

public:
	Enemy();
	~Enemy();
	void Init();
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

	void CheckInEcho(
		std::weak_ptr<MeshCollision> arg_stageMeshCollision);

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

private:
	void Patrol(std::pair<float, float> arg_pPos);
	void Combat(
		KazMath::Vec3<float>& arg_pPos,
		std::weak_ptr<BulletMgr>& arg_bulletMgr,
		std::list<std::shared_ptr<MeshCollision>>
		& arg_stageColliders);

public:
	KazMath::Transform3D GetTrans() { return m_trans; }
	KazMath::Vec3<float> GetPos() { return m_trans.pos; }
	bool IsDiscovery() {
		if (m_isCombatTri) { return true; }
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
		m_rootPos = arg_rootPos;
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
};
