#include "FieldAIDebugManager.h"
#include"../KazLibrary/Helper/ConvertEnumToString.h"
#include"../KazLibrary/Camera/CameraMgr.h"
#include"Input/KeyBoradInputManager.h"
#include"../Game/MapLoader/MapLoader.h"

FieldAIDebugManager::FieldAIDebugManager() :m_radioType(0)
{

}

void FieldAIDebugManager::Init(DrawingByRasterize& arg_rasterize, const KazMath::Vec2<int>& arg_mapIDMaxSize, std::vector<std::vector<PatrolConfig::AstarData>>arg_patData)
{
	m_modelInstanceRender.Load(arg_rasterize, "Resource/Test/Sphere/", "Sphere.gltf");


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
	m_astarData = arg_patData;
}

void FieldAIDebugManager::Update()
{
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
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_J))
	{
		m_keepRay = mousePoint;
	}
	//aスターの指定
	for (int x = 0; x < m_astarData.size(); ++x)
	{
		for (int y = 0; y < m_astarData[x].size(); ++y)
		{
			KazMath::Vec3<float>pos = m_astarData[3][12].trans.pos;
			Sphere enemyHitBox(&pos, 10.0f);

			//カーソルに合わせたかどうか
			if (CollisionManager::Instance()->CheckRayAndSphere(m_keepRay, enemyHitBox))
			{
				m_selectPos = pos;

				break;
			}
		}
	}

	KazMath::Vec2<int>selectIndex(std::clamp(m_selectPos.ConvertVec2XZ().Int().x / MapManager::GetMapSizeData(0).x, 0, MapManager::GetMapSizeData(0).x),
		std::clamp(m_selectPos.ConvertVec2XZ().Int().y / MapManager::GetMapSizeData(0).y, 0, MapManager::GetMapSizeData(0).y));

	m_index.x =
		std::clamp(m_index.x, 0, 38);
	m_index.y =
		std::clamp(m_index.y, 0, 38);
	SetGridColorForSearch(
		m_index.x,
		m_index.y,
		KazMath::Color(0, 0, 255, 255));

	if (m_radioType == 2)
	{
		//m_hidePlace.GetPointIndex();
	}
}

void FieldAIDebugManager::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	m_modelInstanceRender.Draw(arg_rasterize, arg_blas);
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
		KazImGuiHelper::InputVec2("SelectPos", &m_index);
		//KazImGuiHelper::DrawVec3("SelectPos", m_selectPos);
	}
	break;
	case 1:
	{

	}
	break;
	default:
		break;
	}
	ImGui::End();
}

void FieldAIDebugManager::SetGridColorForSearch(int arg_x, int arg_y, const KazMath::Color& arg_color)
{
	m_gridColorArray[0][arg_x * static_cast<int>(m_mapChipMaxSize.x) + arg_y] = arg_color;
}

void FieldAIDebugManager::SetGridColorForExistenceEstablishmentMap(int arg_x, int arg_y, const KazMath::Color& arg_color)
{
	m_gridColorArray[1][arg_x * static_cast<int>(m_mapChipMaxSize.x) + arg_y] = arg_color;
}
