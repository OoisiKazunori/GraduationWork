#pragma once
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../EnemyAIData.h"
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/AI/Debug/FieldAIDebugManager.h"
#include"../Game/AI/Sight/ISight.h"

/// <summary>
/// 存在確立マップ
/// </summary>
class ExistenceEstablishmentMap :public ISingleton<ExistenceEstablishmentMap>
{
public:
	void Init(const KazMath::Vec2<int>& arg_mapIDMaxSize);
	void Update();
	//視界の頂点を元に探索済みかどうかの判定を取ります。
	void Find(SightCollision& arg_sightPoint);

	struct FieldData
	{
		float m_rate;//プレイヤーが存在する可能性の高さ
		bool m_alreadySelectedFlag;	//誰かがこの場所に向かおうとしている
		FieldData() :m_rate(0.0f), m_alreadySelectedFlag(false)
		{};
	};
private:
	int m_x, m_z;
	std::vector<FieldData>m_mapDataArray;
	SightCollision m_sightCollision;
};

