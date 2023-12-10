#pragma once
#include"Gauge.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/AI/Evaluation/FieldAI.h"

class FindGauge
{
public:
	FindGauge();

	void Update(
		const KazMath::Vec3<float>& arg_playerPos,
		const KazMath::Vec3<float>& arg_enemyPos,
		float arg_warningLevel,
		bool arg_isFindFlag
	);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool IsFind();
private:
	Gauge m_gauge;

	//�G�����F�����ۂɂǂꂭ�炢�̋�������Q�[�W�����ʂ𑝂₷��
	const float DISTANCE_MAX;
	//��̃Q�[�W�����ɑ΂��ő�łǂꂭ�炢���₷��
	const float DISTANCE_GAUGE_INCRE_MAX_NUM;
	//��������ĂȂ��Ƃ��ɃQ�[�W���ǂꂭ�炢���炷��
	const float GAUGE_DECRE_NUM;
	//�x�����ɔ������ꂽ�ꍇ�ɂǂꂭ�炢�l�𑝂₷�ׂ���
	const float WARING_INCRE_MAX_NUM;
};

