#pragma once
#include"../KazLibrary/Helper/ISinglton.h"
#include"../Gauge/Gauge.h"

/// <summary>
/// ステージ環境に設定されている値
/// </summary>
class FieldAI:public ISingleton<FieldAI>
{
public:
	FieldAI();

	/// <summary>
	// 敵全体の警戒度
	// -1.0f ~ 1.0f
	/// </summary>
	float GetWaringRate();

	void DebugUpdate();
private:
	//敵全体の警戒度
	Gauge m_waringGauge;
};

