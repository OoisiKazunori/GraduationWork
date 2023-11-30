#include "CheckPoint.h"
#include"Imgui/MyImgui.h"
#include"../KazLibrary/Helper/KazHelper.h"

CheckPoint::CheckPoint(DrawingByRasterize& arg_rasterize) :
	m_render(arg_rasterize, "Resource/UI/Goal.png", true),
	m_meterRender(arg_rasterize, "Resource/UI/m_1400Yen.png", true)
{
	for (int i = 0; i < m_distanceRender.size(); ++i)
	{
		std::string numberString(std::to_string(i));
		m_distanceRender[i].m_tex.Load(arg_rasterize, "Resource/Number/Number_" + numberString + ".png", true);
		m_digitTexture[i] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/Number/Number_" + numberString + ".png");
	}
}

void CheckPoint::Init(const KazMath::Vec3<float>& arg_pointWorldPos)
{
	m_worldPos = arg_pointWorldPos;
	m_distance = 0.0f;
}

void CheckPoint::Update()
{
	m_screenPos = KazMath::ConvertWorldPosToScreenPos(
		m_worldPos,
		CameraMgr::Instance()->GetViewMatrix(),
		CameraMgr::Instance()->GetPerspectiveMatProjection()
	);

	m_digit = KazHelper::CountNumber(static_cast<int>(m_distance));
}

void CheckPoint::Draw(DrawingByRasterize& arg_rasterize)
{
#ifdef _DEBUG
	ImGui::Begin("ScreenPos");
	ImGui::Text("X:%f,Y:%f,Z:%f", m_screenPos.x, m_screenPos.y, m_screenPos.z);
	ImGui::End();
#endif // _DEBUG

	if (1.0f <= m_screenPos.z)
	{
		return;
	}
	KazMath::Transform2D transform;
	transform.pos = m_screenPos.ConvertVec2();
	transform.scale.x = 0.25f;
	transform.scale.y = 0.25f;
	m_render.m_tex.Draw2D(arg_rasterize, transform);

	KazMath::Transform2D meterTransform;
	//距離UI
	for (int i = 0; i < m_digit.size(); ++i)
	{
		//最大数越えしたらスキップする
		if (m_distanceRender.size() <= m_digit.size())
		{
			break;
		}
		const float SCALE = 15.0f;
		//中心座標
		const KazMath::Vec2<float>BASE_POS = transform.pos;
		//桁のクランプ
		int index = std::clamp(static_cast<int>(m_digit.size()) - 1, 0, 10);
		//文字全体の長さの半分の値を使って真ん中ぞろえをする
		const float ADJ_DISTANCE = (static_cast<float>(index) * SCALE) / 2.0f;

		//数字のtransform
		KazMath::Transform2D distanceTransform;
		distanceTransform.pos = BASE_POS - KazMath::Vec2<float>(ADJ_DISTANCE, 30.0f);
		//文字を並べる
		distanceTransform.pos.x += SCALE * static_cast<float>(i);
		distanceTransform.scale = {0.8f,0.8f};
		//描画
		m_distanceRender[i].m_tex.Draw2D(arg_rasterize, distanceTransform, m_digitTexture[m_digit[i]]);

		//mの座標を決める
		distanceTransform.pos.x += SCALE;
		distanceTransform.pos.y += 5.0f;
		distanceTransform.scale = { 1.0f,1.0f };
		meterTransform = distanceTransform;
	}
	m_meterRender.m_tex.Draw2D(arg_rasterize, meterTransform);
}

void CheckPoint::CalucurateDistance(const KazMath::Vec3<float>& arg_playerPos)
{
	m_distance = arg_playerPos.Distance(m_worldPos);
}
