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

	const std::string filePass = "Resource/DefferdRendering/";
	std::array<std::string, 12> modelFilePassArray;
	modelFilePassArray[0] = filePass + "Avocado/";
	modelFilePassArray[1] = filePass + "BoomBox/";
	modelFilePassArray[2] = filePass + "Corset/";
	modelFilePassArray[3] = filePass + "Suzanne/";
	modelFilePassArray[4] = filePass + "WaterBottle/";
	modelFilePassArray[5] = filePass + "BarramundiFish/";
	modelFilePassArray[6] = filePass + "Lantern/";
	std::array<std::string, 12> modelFileNameArray;
	modelFileNameArray[0] = "Avocado.gltf";
	modelFileNameArray[1] = "BoomBox.gltf";
	modelFileNameArray[2] = "Corset.gltf";
	modelFileNameArray[3] = "Suzanne.gltf";
	modelFileNameArray[4] = "WaterBottle.gltf";
	modelFileNameArray[5] = "BarramundiFish.gltf";
	modelFileNameArray[6] = "Lantern.gltf";
	std::array<float, 12>scaleArray;
	scaleArray[0] = 100.0f;
	scaleArray[1] = 500.0f;
	scaleArray[2] = 100.0f;
	scaleArray[3] = 5.0f;
	scaleArray[4] = 25.0f;
	scaleArray[5] = 10.0f;
	scaleArray[6] = 0.5f;

	int index = 0;
	for (auto& modelType : m_models)
	{
		modelType.Load(arg_rasterize, modelFilePassArray[index], modelFileNameArray[index], scaleArray[index]);
		++index;
	}
	for (auto& modelType : m_lights)
	{
		modelType.Load(arg_rasterize, "Resource/DefferdRendering/BoxTextured/", "BoxTextured.gltf", 1.0f);
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
	//モデル配置
	{
		KazMath::Transform3D transform;
		transform.pos = { -100.0f,4.5f,-55.0f };
		for (int z = 0; z < m_models.size(); ++z)
		{
			transform.pos.z = -55.0f + static_cast<float>(z) * 17.0f;
			m_models[z].Draw(arg_rasterize, arg_blasVec, transform);
		}
	}


	//ライトの配置
	{
		KazMath::Transform3D transform;
		transform.pos = { -100.0f,15.0f,-55.0f };
		for (int z = 0; z < m_lights.size(); ++z)
		{
			transform.pos.z = -55.0f + static_cast<float>(z) * 10.0f;
			m_lights[z].Draw(arg_rasterize, arg_blasVec, transform);
		}
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

void RenderScene::ParallelModels::Load(DrawingByRasterize& arg_rasterize, std::string arg_filePass, std::string arg_fileName, float arg_scale)
{
	for (auto& modelType : m_modelDrawArray)
	{
		for (auto& modelInstance : modelType)
		{
			modelInstance.Load(arg_rasterize, arg_filePass, arg_fileName, false);
		}
	}
	m_scale = arg_scale;
}

void RenderScene::ParallelModels::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, const KazMath::Transform3D& arg_baseTransform)
{
	//アボガド描画
	for (int x = 0; x < m_modelDrawArray.size(); ++x)
	{
		for (int y = 0; y < m_modelDrawArray[x].size(); ++y)
		{
			KazMath::Transform3D transform = arg_baseTransform;
			transform.pos.x += static_cast<float>(x) * 30.0f;
			transform.pos.y += static_cast<float>(y) * 20.0f;
			transform.scale = { m_scale ,m_scale ,m_scale };
			m_modelDrawArray[x][y].m_model.Draw(arg_rasterize, arg_blasVec, transform);
		}
	}
}
