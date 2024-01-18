#include "EnemyDebugManager.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Helper/ConvertEnumToString.h"
#include"../KazLibrary/Camera/CameraMgr.h"
#include"Input/KeyBoradInputManager.h"

EnemyDebugManager::EnemyDebugManager() :m_debugAIFlag(false)
{

}

void EnemyDebugManager::Init(DrawingByRasterize& arg_rasterize)
{
	m_selectEnemyModelRender.Load(
		arg_rasterize,
		ModelLoader::Instance()->Load("Resource/Test/Sphere/", "Sphere.gltf"),
		DrawFuncData::SetDefferdRenderingModelAnimationWireFrame(ModelLoader::Instance()->Load("Resource/Test/Sphere/", "Sphere.gltf")));

	for (auto& obj : m_coneSightArray)
	{
		obj.Generate(arg_rasterize);
	}
}

void EnemyDebugManager::Generate(const EnemyDebugData* arg_enemyData)
{
	static int num = 0;
	m_enemyAIDataArray[num] = *arg_enemyData;
	++num;
}

void EnemyDebugManager::Update()
{
	if (!m_debugAIFlag)
	{
		return;
	}

	//スクリーン座標をワールド座標に変換--------------------------------
	Ray mousePoint;
	KazMath::Vec3<float>mousePointNearPos(
		KeyBoradInputManager::Instance()->GetMousePoint().x,
		KeyBoradInputManager::Instance()->GetMousePoint().y,
		0.0f);
	KazMath::Vec3<float>mousePointFarPos(
		KeyBoradInputManager::Instance()->GetMousePoint().x,
		KeyBoradInputManager::Instance()->GetMousePoint().y,
		1.0f);
	mousePoint.start = KazMath::ConvertScreenPosToWorldPos(
		mousePointNearPos,
		CameraMgr::Instance()->GetViewMatrix(),
		CameraMgr::Instance()->GetPerspectiveMatProjection()
	);
	KazMath::Vec3<float>mousePointWorldFarPos(
		KazMath::ConvertScreenPosToWorldPos(
			mousePointFarPos,
			CameraMgr::Instance()->GetViewMatrix(),
			CameraMgr::Instance()->GetPerspectiveMatProjection()
		));
	mousePoint.dir = mousePointWorldFarPos - mousePoint.start;
	mousePoint.dir.Normalize();
	//スクリーン座標をワールド座標に変換--------------------------------

	//敵の指定
	for (auto& aiData : m_enemyAIDataArray)
	{
		if (!aiData.m_transform)
		{
			break;
		}
		KazMath::Vec3<float>pos = aiData.m_transform->pos;
		Sphere enemyHitBox(&pos, 10.0f);

		//カーソルに合わせたかどうか
		if (CollisionManager::Instance()->CheckRayAndSphere(mousePoint, enemyHitBox))
		{
			m_debugData = aiData;
			break;
		}
	}

}

void EnemyDebugManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (!m_debugAIFlag || !m_debugData.m_transform)
	{
		return;
	}
	if (m_debugData.m_transform)
	{
		KazMath::Transform3D transform = *m_debugData.m_transform;
		transform.scale *= 2.0f;
		m_selectEnemyModelRender.m_model.Draw(arg_rasterize, arg_blasVec, transform);
	}

	KazMath::Color lineColor(255, 255, 255, 255);
	if (*m_debugData.m_isFindFlag)
	{
		lineColor = KazMath::Color::Color(255, 0, 0, 255);
	}

	for (int i = 0; i < m_coneSightArray.size(); ++i)
	{
		KazMath::Vec3<float>startPos(
			m_debugData.m_coneSightPointArray[i * 2]->x,
			m_debugData.m_transform->pos.y - 5.0f,
			m_debugData.m_coneSightPointArray[i * 2]->y
		);
		KazMath::Vec3<float>endPos(
			m_debugData.m_coneSightPointArray[i * 2 + 1]->x,
			m_debugData.m_transform->pos.y - 5.0f,
			m_debugData.m_coneSightPointArray[i * 2 + 1]->y
		);
		m_coneSightArray[i].m_render.Draw(arg_rasterize, arg_blasVec, startPos, endPos, lineColor);
	}
}

void EnemyDebugManager::DrawImGui()
{
	if (!m_debugAIFlag)
	{
		return;
	}

	//カーソル合わせた敵の表示
	ImGui::Begin("EnemyDebugAI");
	ImGui::Text("WaringRate:%.1f", FieldAI::Instance()->GetWaringRate());
	ImGui::Text("EnemyInfomation");
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(450, 200), ImGuiWindowFlags_NoTitleBar);
	ImGui::Text(m_debugData.m_enemyName.c_str());
	if (m_debugData.m_transform)
	{
		KazImGuiHelper::DrawVec3("EnemyPos", m_debugData.m_transform->pos);
	}
	if (m_debugData.m_status)
	{
		State status = *m_debugData.m_status;
		std::string statusName = GetEnumName(status);
		ImGui::Text(statusName.c_str());
	}
	if (ImGui::TreeNode("FindGauge"))
	{
		if (m_debugData.m_gaugeData)
		{
			ImGui::Text("GaugeNowRate:%.2f", m_debugData.m_gaugeData->m_gaugeNowRate);
			ImGui::Text("NearRate:%.2f", m_debugData.m_gaugeData->m_nearRate);
			ImGui::Text("OffsetWaringRate:%.2f", m_debugData.m_gaugeData->m_offsetWaringRate);
			ImGui::Text("GaugeIncreRate:%.2f", m_debugData.m_gaugeData->m_gaugeIncreRate);
			ImGui::Text("GaugeDecreRate:%.2f", m_debugData.m_gaugeData->m_gaugeDecreRate);
		}
		else
		{
			ImGui::Text("NullData");
		}
		ImGui::TreePop();
	}
	ImGui::EndChild();
	ImGui::End();
}
