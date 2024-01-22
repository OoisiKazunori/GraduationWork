#include "FieldAIDebugManager.h"

FieldAIDebugManager::FieldAIDebugManager() :m_radioType(0)
{

}

void FieldAIDebugManager::Init(DrawingByRasterize& arg_rasterize)
{
	m_modelInstanceRender.Load(arg_rasterize, "Resource/Test/Sphere/", "Sphere.gltf");


	m_modelInstanceRender.GenerateTransformBuffer(10000);
	m_modelInstanceRender.m_model.m_drawCommand.drawMultiMeshesIndexInstanceCommandData.drawIndexInstancedData[0].instanceCount = 10000;
	std::vector<KazMath::Transform3D>transformArray;
	for (int x = 0; x < 100; ++x)
	{
		for (int z = 0; z < 100; ++z)
		{
			transformArray.emplace_back(KazMath::Transform3D({ 0.0f,0.0f,static_cast<float>(x * 10 + z * 10) }));
			m_gridColorArray.emplace_back(KazMath::Color(1, 1, 1, 1));
		}
	}
	m_modelInstanceRender.UploadTransformMatrix(transformArray);
	m_modelInstanceRender.UploadColor(m_gridColorArray);
}

void FieldAIDebugManager::Update()
{
}

void FieldAIDebugManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	m_modelInstanceRender.Draw(arg_rasterize, arg_blas);
}

void FieldAIDebugManager::DrawImGui()
{
	ImGui::Begin("FieldAIDebug");
	ImGui::RadioButton("existence-establishment map", &m_radioType,0);
	ImGui::RadioButton("Filtering Visualization", &m_radioType,1);
	ImGui::RadioButton("congested zone", &m_radioType,2);

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
}

void FieldAIDebugManager::SetGridColor(int x, int y, const KazMath::Color& arg_color)
{
	m_gridColorArray[x * static_cast<int>(m_gridColorArray.size()) + y] = arg_color;
	m_modelInstanceRender.UploadColor(m_gridColorArray);
}
