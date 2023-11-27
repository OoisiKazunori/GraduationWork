#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "PatrolData.h"

class MeshCollision;

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

private:
	std::vector<std::pair<float, float>> m_rootPos;
	std::vector<std::pair<int, int>> m_checkPointDelay;
	KazMath::Transform3D m_trans;
	State m_state;
	int m_delayNum;
	int m_count;
	int m_delay;
	bool m_isCheckPoint;
	bool m_onGround;

	std::vector<std::pair<float, float>> m_checkSoundPos;
	int m_checkSoundCount;
	bool m_isReturn;
	KazMath::Vec3<float> m_oldPos;

public:
	Enemy();
	~Enemy();
	void Init();
	void Update(
		std::weak_ptr<MeshCollision> arg_meshCollision);
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

private:
	DirectX::XMVECTOR CalMoveQuaternion(
		KazMath::Vec3<float> arg_pos,
		KazMath::Vec3<float> arg_prevPos);
	void Collision(
		std::weak_ptr<MeshCollision> arg_meshCollision);

public:
	KazMath::Transform3D GetTrans() { return m_trans; }
	KazMath::Vec3<float> GetPos() { return m_trans.pos; }

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
	void AddOffset(std::pair<size_t, size_t> arg_offsets) {
		m_trans.pos.x += arg_offsets.first;
		m_trans.pos.z += arg_offsets.second;
	}
};
