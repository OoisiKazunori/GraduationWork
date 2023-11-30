#include "RenderScene.h"
#include"../KazLibrary/Imgui/MyImgui.h"

RenderScene::RenderScene(DrawingByRasterize& arg_rasterize) :
	m_sponzaModelRender(arg_rasterize, "Resource/DefferdRendering/Sponza/", "Sponza.gltf")
{
	//書き込まれているGBufferを入手する
	for (int i = 0; i < 3; ++i)
	{
		m_gBufferRender[i].m_tex.Load(
			arg_rasterize,
			RenderTargetStatus::Instance()->GetBuffer(GBufferMgr::Instance()->GetRenderTarget()[i]),
			true
		);
	}

	//モデルの配置
	m_modelInstanceRenderArray[0].m_modelInstanceRender.Load(arg_rasterize, "Resource/DefferdRendering/Avocado/", "Avocado.gltf", false);
	m_modelInstanceRenderArray[0].m_modelInstanceRender.m_transformBuffer.GenerateBuffer(sizeof(CoordinateSpaceMatData), 20);
	m_modelInstanceRenderArray[0].m_modelInstanceRender.m_transformBuffer.m_vramBuffer.rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_modelInstanceRenderArray[0].m_modelInstanceRender.m_transformBuffer.m_vramBuffer.rootParamType = GRAPHICS_PRAMTYPE_DATA;

	m_renderTransform.pos = { WIN_X / 2,WIN_Y / 2 };
	m_gBufferType = 0;
	m_sceneNum = -1;

	m_sponzaModelTransform.scale = { 0.1f,0.1f,0.1f };
}

RenderScene::~RenderScene()
{
}

void RenderScene::Init()
{
}

void RenderScene::PreInit()
{
}

void RenderScene::Finalize()
{
}

void RenderScene::Input()
{
}

void RenderScene::Update(DrawingByRasterize& arg_rasterize)
{
	//デバック用のカメラワーク(操作はBlenderと同じ)
	m_camera.Update();

	ImGui::Begin("DemoInspector");
	ImGui::RadioButton("GBuffer-Albedo", &m_gBufferType, 0);
	ImGui::RadioButton("GBuffer-Normal", &m_gBufferType, 1);
	ImGui::RadioButton("GBuffer-Final", &m_gBufferType, 2);
	ImGui::End();
}

void RenderScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	//スポンザの描画
	m_sponzaModelRender.m_model.Draw(arg_rasterize, arg_blasVec, m_sponzaModelTransform);
	//G-Bufferの描画
	m_gBufferRender[m_gBufferType].m_tex.Draw2D(arg_rasterize, m_renderTransform);

	//for (int x = 0; x < m_modelDrawArray.size(); ++x)
	//{
	//	for (int z = 0; z < m_modelDrawArray[x].size(); ++z)
	//	{
	//		KazMath::Transform3D transform;
	//		transform.pos = { static_cast<float>(x) * 30.0f,0.0f,static_cast<float>(z) * 30.0f };
	//
	//		m_modelDrawArray[x][z].m_model.Draw(arg_rasterize, arg_blasVec, transform);
	//	}
	//}
	//KazMath::Transform3D transform;
	//transform.pos = { static_cast<float>(0) * 30.0f,0.0f,static_cast<float>(0) * 30.0f };
	//m_modelInstanceRenderArray[0].m_model.Draw(arg_rasterize, arg_blasVec, transform);

	//for (auto& obj : m_modelInstanceRenderArray)
	//{
	//	obj.m_modelInstanceRender.Draw(arg_rasterize, arg_blasVec);
	//}
	m_modelInstanceRenderArray[0].m_modelInstanceRender.Draw(arg_rasterize, arg_blasVec);
}

int RenderScene::SceneChange()
{
	if (m_sceneNum != SCENE_NONE)
	{
		int tmp = m_sceneNum;
		m_sceneNum = SCENE_NONE;
		return tmp;
	}
	return SCENE_NONE;
}
