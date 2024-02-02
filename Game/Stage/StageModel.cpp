#include "StageModel.h"

StageModel::StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool f_useOffset,
	DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, float f_echo) :
	//ステージに使用する描画情報は右端は必ずフラグを立てるようにする。
	m_stageModelRender(
		arg_rasterize,
		ModelLoader::Instance()->Load(arg_fileDir, arg_fileName),
		DrawFuncData::SetDefferdRenderingModelAnimationAppearByEcho(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)),
		true)
{
	const float l_stageScle = 5.2f;
	const float yoffset = 50;
	//m_transform = KazMath::Transform3D({ pos.x * l_stageScle, (pos.y * l_stageScle) - yoffset, pos.z * l_stageScle }, { scale.x * l_stageScle, scale.y * l_stageScle , scale.z * l_stageScle });
	if (f_useOffset)
	{
		m_transform = KazMath::Transform3D({ pos.x, pos.y - yoffset, pos.z }, { scale.x * l_stageScle, scale.y * l_stageScle , scale.z * l_stageScle });
	}
	else
	{
		m_transform = KazMath::Transform3D({ pos.x, pos.y , pos.z }, { scale.x * l_stageScle, scale.y * l_stageScle , scale.z * l_stageScle });
	}
	DirectX::XMVECTOR axis; //回転用軸
	//XZ軸回転用クオータニオン作成
	axis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.x));
	axis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.y));
	axis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.z));


	m_stageModelRender.m_model.m_drawCommand.extraBufferArray[3] = EchoArray::Instance()->GetEchoMemoryStructuredBuffer();
	m_stageModelRender.m_model.m_drawCommand.extraBufferArray[3].rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_stageModelRender.m_model.m_drawCommand.extraBufferArray[3].rootParamType = GRAPHICS_PRAMTYPE_DATA;

}

StageModel::StageModel(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool f_useOffset,
						int enemyIndex, int roadIndex, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, float f_echo):
	m_stageModelRender(
		arg_rasterize,
		ModelLoader::Instance()->Load(arg_fileDir, arg_fileName),
		DrawFuncData::SetDefferdRenderingModelAnimationAppearByEcho(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)),
		true)
{
	const float l_stageScle = 5.2f;
	const float yoffset = 50;
	//m_transform = KazMath::Transform3D({ pos.x * l_stageScle, (pos.y * l_stageScle) - yoffset, pos.z * l_stageScle }, { scale.x * l_stageScle, scale.y * l_stageScle , scale.z * l_stageScle });
	if (f_useOffset)
	{
		m_transform = KazMath::Transform3D({ pos.x, pos.y - yoffset, pos.z }, { scale.x * l_stageScle, scale.y * l_stageScle , scale.z * l_stageScle });
	}
	else
	{
		m_transform = KazMath::Transform3D({ pos.x, pos.y , pos.z }, { scale.x * l_stageScle, scale.y * l_stageScle , scale.z * l_stageScle });
	}
	DirectX::XMVECTOR axis; //回転用軸
	//XZ軸回転用クオータニオン作成
	axis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.x));
	axis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.y));
	axis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_transform.Rotation({ axis.m128_f32[0], axis.m128_f32[1], axis.m128_f32[2] }, KazMath::AngleToRadian(rot.z));


	m_stageModelRender.m_model.m_drawCommand.extraBufferArray[3] = EchoArray::Instance()->GetEchoMemoryStructuredBuffer();
	m_stageModelRender.m_model.m_drawCommand.extraBufferArray[3].rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_stageModelRender.m_model.m_drawCommand.extraBufferArray[3].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_roadIndex = roadIndex;
	m_enemyIndex = enemyIndex;
}

void StageModel::Update()
{
	if (!m_echoFlag)
	{
		return;
	}
	if (m_isDrawFlag != m_prevIsDrawFlag)
	{
		m_drawTimer = 100;
	}
	--m_drawTimer;
	m_prevIsDrawFlag = m_isDrawFlag;
}

void StageModel::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (m_drawTimer <= 0 && m_echoFlag)
	{
		return;
	}
	m_stageModelRender.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}
