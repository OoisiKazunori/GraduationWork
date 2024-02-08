#include "HitCircle.h"
#include"../KazLibrary/Easing/easing.h"

HitCircle::HitCircle() :m_activeFlag(false)
{
}

void HitCircle::Load(DrawingByRasterize &arg_rasterize)
{
	m_render.Load(true,arg_rasterize, "Resource/Effect/FireCircle.png");
}

void HitCircle::Init(const KazMath::Vec3<float> &arg_pos, float arg_timer)
{
	m_transform.pos = arg_pos;
	m_baseTransform.pos = arg_pos;
	m_transform.scale = {};
	m_baseTransform.scale = { 0.5f,0.5f,0.5f };
	m_time.Reset(arg_timer);
	m_activeFlag = true;
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

void HitCircle::Draw(DrawingByRasterize &arg_rasterize, Raytracing::BlasVector &arg_blasVector)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_render.m_tex.Draw3DBillBoard(arg_rasterize, arg_blasVector, m_transform, KazMath::Color(255, 255, 255, 255));
}
