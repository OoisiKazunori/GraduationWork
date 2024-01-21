#include "FieldAIDebugManager.h"

FieldAIDebugManager::FieldAIDebugManager() :m_congestedZoneFlag(false), m_existenceEstablishmentMapFlag(false), m_filteringVisualizationFlag(false)
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
		}
	}
	m_modelInstanceRender.UploadTransformMatrix(transformArray);
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
	ImGui::Checkbox("existence-establishment map", &m_existenceEstablishmentMapFlag);
	ImGui::SameLine();
	ImGui::Checkbox("Filtering Visualization", &m_filteringVisualizationFlag);
	ImGui::SameLine();
	ImGui::Checkbox("congested zone", &m_congestedZoneFlag);
	ImGui::SameLine();
	ImGui::End();
}
