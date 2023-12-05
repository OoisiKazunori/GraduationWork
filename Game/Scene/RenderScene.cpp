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

	for (auto& modelType : m_models)
	{
		modelType.Load(arg_rasterize, "Resource/DefferdRendering/Avocado/", "Avocado.gltf");
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

	m_sponzaModelTransform.scale = { 0.1f,0.1f,0.1f };
	m_axisRender.Load(arg_rasterize, "Resource/DefferdRendering/Axis/", "Axis.gltf", false);
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

	//�A�{�K�h�`��
	for (int x = 0; x < m_modelDrawArray.size(); ++x)
	{
		for (int z = 0; z < m_modelDrawArray[x].size(); ++z)
		{
			KazMath::Transform3D transform;
			transform.pos = { -100.0f + static_cast<float>(x) * 30.0f,static_cast<float>(z) * 25.0f,-55.0f };
			const float scale = 100.0f;
			transform.scale = { scale,scale,scale };
			//m_modelDrawArray[x][z].m_model.Draw(arg_rasterize, arg_blasVec, transform);
		}
	}

	for (int z = 0; z < m_models.size(); ++z)
	{
		m_models[z].Draw(arg_rasterize, arg_blasVec, -55.0f + static_cast<float>(z) * 10.0f);
	}
	KazMath::Transform3D t;
	t.scale.z = 5.0f;
	m_axisRender.m_model.Draw(arg_rasterize, arg_blasVec, t);
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

void RenderScene::ParallelModels::Load(DrawingByRasterize& arg_rasterize, std::string arg_filePass, std::string arg_fileName)
{
	for (auto& modelType : m_modelDrawArray)
	{
		for (auto& modelInstance : modelType)
		{
			modelInstance.Load(arg_rasterize, arg_filePass, arg_fileName, false);
		}
	}
}

void RenderScene::ParallelModels::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, float arg_zPos)
{
	//�A�{�K�h�`��
	for (int x = 0; x < m_modelDrawArray.size(); ++x)
	{
		for (int y = 0; y < m_modelDrawArray[x].size(); ++y)
		{
			KazMath::Transform3D transform;
			transform.pos = { -100.0f + static_cast<float>(x) * 30.0f,static_cast<float>(y) * 25.0f,arg_zPos };
			const float scale = 100.0f;
			transform.scale = { scale,scale,scale };
			m_modelDrawArray[x][y].m_model.Draw(arg_rasterize, arg_blasVec, transform);
		}
	}
}
