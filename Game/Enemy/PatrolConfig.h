#pragma once
#include "../KazLibrary/Math/KazMath.h"

class PatrolConfig
{
public:
	enum struct AstarType
	{
		Move,
		UnMove,
		Root
	};

	struct AstarData
	{
		int cost = 1;
		AstarType type = AstarType::Move;
		KazMath::Transform3D trans;
		KazMath::Color color;
	};

private:
	std::vector<std::vector<AstarData>> m_astarDatas;
	size_t m_size_x;	//Chip_X
	size_t m_size_y;	//Chip_Y
	size_t m_offset_x;
	size_t m_offset_y;
	float m_chipSize;	//Scale

public:
	PatrolConfig(
		size_t arg_x = 0,
		size_t arg_y = 0,
		float arg_chipSize = 0);

private:
	void Update();

public:
	AstarData& GetData(int arg_x, int arg_y) {
		return m_astarDatas[arg_x][arg_y];
	}

	size_t GetSizeX() { return m_size_x; }
	size_t GetSizeY() { return m_size_y; }
	size_t GetOffsetX() { return m_offset_x; }
	size_t GetOffsetY() { return m_offset_y; }
	float GetChipSize() { return m_chipSize; }

public:
	void SetSizeX(size_t arg_x) { m_size_x = arg_x; }
	void SetSizeY(size_t arg_y) { m_size_y = arg_y; }
	void SetOffsetX(size_t arg_x) { m_offset_x = arg_x; }
	void SetOffsetY(size_t arg_y) { m_offset_y = arg_y; }
	void SetChipSize(float arg_chipSize) {
		m_chipSize = arg_chipSize;
		Update();
	}
	void SetType(
		size_t arg_x, size_t arg_y, AstarType arg_type) {
		m_astarDatas[arg_x][arg_y].type = arg_type;
	}
	void SetTrans(
		size_t arg_x,
		size_t arg_y,
		KazMath::Transform3D arg_trans) {
		m_astarDatas[arg_x][arg_y].trans = arg_trans;
	}
	void SetColor(
		size_t arg_x,
		size_t arg_y,
		KazMath::Color arg_color) {
		m_astarDatas[arg_x][arg_y].color = arg_color;
	}
};