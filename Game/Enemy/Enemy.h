#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "PatrolData.h"

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

public:
	std::shared_ptr<
		BasicDraw::BasicModelRender> m_enemyBox;
	std::vector<std::pair<float, float>> m_rootPos;
	std::vector<std::pair<int, int>> m_checkPointDelay;
	KazMath::Transform3D m_trans;
	State m_state;
	int m_delayNum;
	int m_count;
	int m_delay;
	bool m_isCheckPoint;

	std::vector<std::pair<float, float>> m_checkSoundPos;
	int m_checkSoundCount;
	bool m_isReturn;
	KazMath::Vec3<float> m_oldPos;

public:
	Enemy();
	~Enemy();
	void Init();
	void Update();
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

private:
	DirectX::XMVECTOR CalMoveQuaternion(
		KazMath::Vec3<float> arg_pos,
		KazMath::Vec3<float> arg_prevPos);

public:
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
};
