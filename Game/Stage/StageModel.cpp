#include "StageModel.h"

StageModel::StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName) :
	//�X�e�[�W�Ɏg�p����`����͉E�[�͕K���t���O�𗧂Ă�悤�ɂ���B
	m_stageModelRender(arg_rasterize, arg_fileDir, arg_fileName, true)
{
}

void StageModel::Update()
{
}

void StageModel::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_stageModelRender.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}
