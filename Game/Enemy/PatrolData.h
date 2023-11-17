#pragma once
#include <queue>
#include <tuple>
#include "PatrolConfig.h"

class PatrolData
{
private:
	std::weak_ptr<PatrolConfig> m_patrolConfig;
	KazMath::Color m_color;

private:	//è¡ÇµÇΩÇ¢
	std::vector<std::pair<int, int>> m_checkPoints;
	std::vector<std::pair<float, float>> m_rootPos;

	std::vector<std::pair<int, int>> m_checkPointDelay;

public:
	PatrolData();
	~PatrolData();
	void Init();
	void Update();

public:
	const std::vector<std::pair<float, float>>
		& GetRootPos() { return m_rootPos; }
	const std::vector<std::pair<int, int>>
		& GetCheckPointDelay() { return m_checkPointDelay; }

public:
	void SetData(std::weak_ptr<PatrolConfig> arg_config);
	void SetColor(KazMath::Color arg_color) {
		m_color = arg_color;
	}
	void AddCheckPoint(int arg_x, int arg_y) {
		m_checkPoints.push_back(
			std::make_pair(arg_x, arg_y));
	}

private:
	std::vector<std::pair<int, int>>
		CalcChipRoots(
			const std::vector<std::pair<int, int>>
			& arg_checkPoints);

	void SortRoots(
		std::vector<std::pair<int, int>>& arg_base_roots,
		const std::vector<std::pair<int, int>>& arg_roots);
	void CalcRootPos(
		std::vector<std::pair<int, int>> arg_roots);

private:
	int CalcHeuristic(
		int arg_start_x, int arg_start_y,
		int arg_goal_x, int arg_goal_y);
	bool IsIntheArea(int arg_x, int arg_y);
	bool IsGoal(
		int arg_x, int arg_y,
		int arg_goal_x, int arg_goal_y);
	bool IsCheckPoint(int arg_goal_x, int arg_goal_y);
};