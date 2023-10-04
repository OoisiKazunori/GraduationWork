#pragma once

class Enemy
{
private:
	enum struct State
	{
		Patrol,
		Alert,
		Combat,
		Holdup,
		Death
	};

public:
	Enemy();
	~Enemy();
	void Init();
	void Update();
	void Draw();
};

