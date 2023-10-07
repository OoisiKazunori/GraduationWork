#pragma once
#include"StageModel.h"

/// <summary>
/// �X�e�[�W�S�̂̊Ǘ�����
/// </summary>
class StageManager
{
public:
	StageManager();

	void Init(DrawingByRasterize& arg_rasterize);
	void Update(DrawingByRasterize &arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//�X�e�[�W���؂�ւ�����g���K�[
	bool ChangeSceneTrigger();
private:
	int m_nowStageNumber, m_nextStageNumber;
	bool m_changeSceneTriggerFlag;
	//�n�`�̃��f��
	std::unique_ptr<StageModel> m_stage;

	std::list<std::unique_ptr<StageModel>> m_tree;
	std::list<std::unique_ptr<StageModel>> m_stone;

	//���̑��ؓ��̊O���I�u�W�F�N�g���f��--------------------------------

	//�X�e�[�W�؂�ւ��O�ɕK���Ăяo���Ă�������
	void ChangeScene(DrawingByRasterize&arg_rasterize);
};

