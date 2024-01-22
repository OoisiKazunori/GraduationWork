#pragma once
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../EnemyAIData.h"
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/AI/Debug/FieldAIDebugManager.h"
#include"../Game/AI/Sight/ISight.h"

/// <summary>
/// ���݊m���}�b�v
/// </summary>
class ExistenceEstablishmentMap :public ISingleton<ExistenceEstablishmentMap>
{
public:
	void Init(const KazMath::Vec2<int>& arg_mapIDMaxSize);
	void Update();
	//���E�̒��_�����ɒT���ς݂��ǂ����̔�������܂��B
	void Find(SightCollision& arg_sightPoint);

	struct FieldData
	{
		float m_rate;//�v���C���[�����݂���\���̍���
		bool m_alreadySelectedFlag;	//�N�������̏ꏊ�Ɍ��������Ƃ��Ă���
		FieldData() :m_rate(0.0f), m_alreadySelectedFlag(false)
		{};
	};
private:
	int m_x, m_z;
	std::vector<FieldData>m_mapDataArray;
	SightCollision m_sightCollision;
};

