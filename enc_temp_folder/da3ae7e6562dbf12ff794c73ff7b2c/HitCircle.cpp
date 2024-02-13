#include "HitCircle.h"
#include"../KazLibrary/Easing/easing.h"

HitCircle::HitCircle() :m_activeFlag(false)
{
}

void HitCircle::Load(DrawingByRasterize& arg_rasterize)
{
	m_render.Load(arg_rasterize, "Resource/Effect/FireCircle.png", true);
}

void HitCircle::Init(const KazMath::Vec3<float>& arg_pos, float arg_timer, const KazMath::Color& arg_color)
{
	m_transform.pos = KazMath::ConvertWorldPosToScreenPos(
		arg_pos,
		CameraMgr::Instance()->GetViewMatrix(),
		CameraMgr::Instance()->GetPerspectiveMatProjection()
	).ConvertVec2();
	m_baseTransform.pos = m_transform.pos;
	m_transform.scale = {};
	m_baseTransform.scale = { 5.5f,5.5f };
	m_time.Reset(arg_timer);
	m_activeFlag = true;
	m_color = arg_color;
}

void HitCircle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
	if (m_time.UpdateTimer())
	{
		m_activeFlag = false;
	}
	m_transform.scale = m_baseTransform.scale * EasingMaker(Out, Exp, m_time.GetTimeRate());
}

void HitCircle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_render.m_tex.Draw2D(arg_rasterize, m_transform, m_color);
}
