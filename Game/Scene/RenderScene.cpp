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
