#pragma once
#include"StageModel.h"

/// <summary>
/// �X�e�[�W�S�̂̊Ǘ�����
/// MapLoader�̃f�[�^�Ȃǂ������ŕ`�悵�Ă���
/// </summary>
class StageManager
{
private:
	int m_nowStageNumber, m_nextStageNumber;
	bool m_changeSceneTriggerFlag;
	//�n�`�̃��f��

	std::list<std::unique_ptr<StageModel>> m_tree;
	std::list<std::unique_ptr<StageModel>> m_stone;

	std::list<std::unique_ptr<StageModel>> m_block01;
	//���̑��ؓ��̊O���I�u�W�F�N�g���f��--------------------------------

	//�X�e�[�W�؂�ւ��O�ɕK���Ăяo���Ă�������
	void ChangeScene(DrawingByRasterize& arg_rasterize);
public:
	StageManager();

	void Init(DrawingByRasterize& arg_rasterize, int f_stageNum);
	void Update(DrawingByRasterize &arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//�X�e�[�W���؂�ւ�����g���K�[
	bool ChangeSceneTrigger();
	void AddMapDatas(DrawingByRasterize& arg_rasterize, int f_stageNum);

	std::unique_ptr<StageModel> m_stage;
};

