#include "CheckPoint.h"

CheckPoint::CheckPoint(DrawingByRasterize& arg_rasterize) :
	m_render(arg_rasterize, "Resource/Test/texas.png")
{
}

void CheckPoint::Init(const KazMath::Vec3<float>& arg_pointWorldPos)
{
	m_worldPos = arg_pointWorldPos;
}

void CheckPoint::Update()
{
	m_screenPos = KazMath::ConvertWorldPosToScreenPos(
		m_worldPos,
		CameraMgr::Instance()->GetViewMatrix(),
		CameraMgr::Instance()->GetPerspectiveMatProjection()
	);
}

void CheckPoint::Draw(DrawingByRasterize& arg_rasterize)
{
	KazMath::Transform2D transform;
	transform.pos = m_screenPos.ConvertVec2();
	m_render.m_tex.Draw2D(arg_rasterize, transform);
}
