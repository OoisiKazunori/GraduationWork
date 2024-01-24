#include "FieldAIDebugManager.h"

FieldAIDebugManager::FieldAIDebugManager() :m_radioType(0)
{

}

void FieldAIDebugManager::Init(DrawingByRasterize& arg_rasterize, const KazMath::Vec2<int>& arg_mapIDMaxSize, std::vector<std::vector<PatrolConfig::AstarData>>arg_patData)
{
	m_modelInstanceRender.Load(arg_rasterize, "Resource/Test/Sphere/", "Sphere.gltf");
	m_pointRender.Load(arg_rasterize, "Resource/Test/Sphere/", "Sphere.gltf");


	m_modelInstanceRender.GenerateTransformBuffer(arg_mapIDMaxSize.x * arg_mapIDMaxSize.y);
	m_modelInstanceRender.m_model.m_drawCommand.drawMultiMeshesIndexInstanceCommandData.drawIndexInstancedData[0].instanceCount = arg_mapIDMaxSize.x * arg_mapIDMaxSize.y;
	std::vector<KazMath::Transform3D>transformArray;
	for (int x = 0; x < arg_mapIDMaxSize.x; ++x)
	{
		for (int z = 0; z < arg_mapIDMaxSize.y; ++z)
		{
			transformArray.emplace_back(arg_patData[x][z].trans);
			transformArray.back().pos.y = -45.0f;
			m_gridColorArray[0].emplace_back(KazMath::Color(255, 0, 0, 255));
			m_gridColorArray[1].emplace_back(KazMath::Color(0, 255, 0, 255));
			m_gridColorArray[2].emplace_back(KazMath::Color(0, 0, 255, 255));
			m_gridColorArray[3].emplace_back(KazMath::Color(0, 255, 255, 255));
		}
	}
	m_modelInstanceRender.UploadTransformMatrix(transformArray);
	m_modelInstanceRender.UploadColor(m_gridColorArray[0]);

	m_mapChipMaxSize = arg_mapIDMaxSize;
}

void FieldAIDebugManager::Update()
{
}

void FieldAIDebugManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	m_modelInstanceRender.Draw(arg_rasterize, arg_blas);

	m_pointRender.m_model.Draw(arg_rasterize, arg_blas, m_transform);
}

void FieldAIDebugManager::DrawImGui()
{
	ImGui::Begin("FieldAIDebug");
	ImGui::RadioButton("astar", &m_radioType, 0);
	ImGui::RadioButton("existence-establishment map", &m_radioType, 1);
	ImGui::RadioButton("Filtering Visualization", &m_radioType, 2);
	ImGui::RadioButton("congested zone", &m_radioType, 3);

	m_modelInstanceRender.UploadColor(m_gridColorArray[m_radioType]);
	//グリッドの座標指定
	switch (m_radioType)
	{
	case 0:
	{
		ImGui::Text("");
	}
	break;
	default:
		break;
	}
	ImGui::End();

	KazImGuiHelper::InputTransform3D("Point", &m_transform);
}

void FieldAIDebugManager::SetGridColorForSearch(int arg_x, int arg_y, const KazMath::Color& arg_color)
{
	m_gridColorArray[0][arg_x * static_cast<int>(m_mapChipMaxSize.x) + arg_y] = arg_color;
}

void FieldAIDebugManager::SetGridColorForExistenceEstablishmentMap(int arg_x, int arg_y, const KazMath::Color& arg_color)
{
	m_gridColorArray[1][arg_x * static_cast<int>(m_mapChipMaxSize.x) + arg_y] = arg_color;
}
