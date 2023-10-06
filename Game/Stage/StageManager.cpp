#include "StageManager.h"

StageManager::StageManager() :m_changeSceneTriggerFlag(false)
{
	//�ŏ��̃X�e�[�W�͂�����œǂݍ���
}

void StageManager::Init(DrawingByRasterize& arg_rasterize)
{
	m_nowStageNumber = 0;
	m_nextStageNumber = 0;
	m_changeSceneTriggerFlag = false;
}

void StageManager::Update(DrawingByRasterize &arg_rasterize)
{
	//�X�e�[�W�̐؂�ւ�����
	if (m_nowStageNumber != m_nextStageNumber)
	{
		arg_rasterize.ReleasePipelineInScene();

		m_stage.reset();
		m_stage = std::make_unique<StageModel>(arg_rasterize, "", "");
		m_changeSceneTriggerFlag = true;
	}

	//�X�e�[�W�̐؂�ւ�����
	m_stage->Update();
}

void StageManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_stage->Draw(arg_rasterize, arg_blasVec);
}

bool StageManager::ChangeSceneTrigger()
{
	if (m_changeSceneTriggerFlag)
	{
		bool tmpFlag = m_changeSceneTriggerFlag;
		m_changeSceneTriggerFlag = false;
		return tmpFlag;
	}
	return false;
}
