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

	static int delay = 120;
	static float speed = 0.5f;
	static float eyeCheckDist = 50.0f;
	static float soundCheckDist = 50.0f;
}