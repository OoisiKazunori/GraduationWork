#include "RenderScene.h"
#include"../KazLibrary/Imgui/MyImgui.h"

RenderScene::RenderScene(DrawingByRasterize& arg_rasterize) :
	m_sponzaModelRender(arg_rasterize, "Resource/DefferdRendering/Sponza/", "Sponza.gltf")
{
	//�������܂�Ă���GBuffer����肷��
	for (int i = 0; i < 3; ++i)
	{
		m_gBufferRender[i].m_tex.Load(
			arg_rasterize,
			RenderTargetStatus::Instance()->GetBuffer(GBufferMgr::Instance()->GetRenderTarget()[i]),
			true
		);
	}

	for (auto& modelType : m_modelDrawArray)
	{
		for (auto& modelInstance : modelType)
		{
			modelInstance.Load(arg_rasterize, "Resource/DefferdRendering/Avocado/", "Avocado.gltf", false);
		}
	}

	m_renderTransform.pos = { WIN_X / 2,WIN_Y / 2 };
	m_gBufferType = 0;
	m_sceneNum = -1;
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
	//�f�o�b�N�p�̃J�������[�N(�����Blender�Ɠ���)
	m_camera.Update();

	ImGui::Begin("DemoInspector");
	ImGui::RadioButton("GBuffer-Albedo", &m_gBufferType, 0);
	ImGui::RadioButton("GBuffer-Normal", &m_gBufferType, 1);
	ImGui::RadioButton("GBuffer-Final", &m_gBufferType, 2);
	ImGui::End();
}

void RenderScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	//�X�|���U�̕`��
	m_sponzaModelRender.m_model.Draw(arg_rasterize, arg_blasVec, m_sponzaModelTransform);
	//G-Buffer�̕`��
	m_gBufferRender[m_gBufferType].m_tex.Draw2D(arg_rasterize, m_renderTransform);

	for (int x = 0; x < m_modelDrawArray.size(); ++x)
	{
		for (int z = 0; z < m_modelDrawArray[x].size(); ++z)
		{
			KazMath::Transform3D transform;
			transform.pos = { static_cast<float>(x) * 30.0f,0.0f,static_cast<float>(z) * 30.0f };

			m_modelDrawArray[x][z].m_model.Draw(arg_rasterize, arg_blasVec, transform);
		}
	}
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
