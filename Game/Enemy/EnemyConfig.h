#pragma once
namespace EnemyConfig
{
	enum Direction
	{
		RIGHT,
		UP,
		LEFT,
		DOWN
	};

	//チェックポイント滞在時間
	static const int checkPointDelay = 120;
	//未発見時,巡回に移行するまでの時間
	static const int changePatrolDelay = 300;
	//最大HP
	static const int maxHP = 2;
	//発見から戦闘に移行するまでの時間
	static const int changeCombatDelay = 120;
	//発射頻度(仮)
	static const int shotDelay = 15;
	//見つかった瞬間のスロー時間
	static const int slowDelay = 120;
	//スローの遅くなる倍率(1.0が等倍)
	//static const float slowMag = 0.5f;
	//移動速度
	static const float speed = 0.5f;
	//視界距離
	static const float eyeCheckDist = 5.0f;
	//音を拾う距離
	static const float soundCheckDist = 50.0f;
	//重力
	static const float gravity = 0.05f;
}