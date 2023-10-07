#include "StageModel.h"

StageModel::StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName,
	DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale) :
	//ステージに使用する描画情報は右端は必ずフラグを立てるようにする。
	m_stageModelRender(arg_rasterize, arg_fileDir, arg_fileName, true)
{
	m_transform = KazMath::Transform3D({pos.x, pos.y, pos.z}, {scale.x, scale.y, scale.z});
}

void StageModel::Update()
{
}

void StageModel::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_stageModelRender.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}
