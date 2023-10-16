#include "StageModel.h"

StageModel::StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName,
	DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale) :
	//�X�e�[�W�Ɏg�p����`����͉E�[�͕K���t���O�𗧂Ă�悤�ɂ���B
	m_stageModelRender(arg_rasterize, arg_fileDir, arg_fileName)
{
	const float l_stageScle = 5.0f;
	const float yoffset = 50;
	m_transform = KazMath::Transform3D({pos.x * l_stageScle, pos.y * l_stageScle - yoffset, pos.z * l_stageScle }, {scale.x * l_stageScle, scale.y * l_stageScle, scale.z * l_stageScle });
	DirectX::XMVECTOR axis; //��]�p��
	//XZ����]�p�N�I�[�^�j�I���쐬
	axis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_transform.Rotation({axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2]}, KazMath::AngleToRadian(rot.x));
	axis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.y));
	axis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.z ));
}

void StageModel::Update()
{
}

void StageModel::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_stageModelRender.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}
