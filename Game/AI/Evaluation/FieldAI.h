#pragma once
#include"../KazLibrary/Helper/ISinglton.h"
#include"../Gauge/Gauge.h"

/// <summary>
/// �X�e�[�W���ɐݒ肳��Ă���l
/// </summary>
class FieldAI:public ISingleton<FieldAI>
{
public:
	FieldAI();

	/// <summary>
	// �G�S�̂̌x���x
	// -1.0f ~ 1.0f
	/// </summary>
	float GetWaringRate();

	void DebugUpdate();
private:
	//�G�S�̂̌x���x
	Gauge m_waringGauge;
};

