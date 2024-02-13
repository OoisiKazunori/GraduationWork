#include "HitCircle.h"
#include"../KazLibrary/Easing/easing.h"

HitCircle::HitCircle() :m_activeFlag(false)
{
}

void HitCircle::Load(DrawingByRasterize& arg_rasterize)
{
	m_render.Load(true, arg_rasterize, "Resource/Effect/FireCircle.png");
}

void HitCircle::Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>* arg_playerPos, float arg_timer, const KazMath::Color& arg_color)
{
	m_transform.pos = arg_pos;
	m_baseTransform.pos = arg_pos;
	m_transform.scale = {};
	m_baseTransform.scale = { 1.5f,1.5f,1.5f };
	m_time.Reset(arg_timer);
	m_activeFlag = true;
	m_color = arg_color;

	m_playerPos = arg_playerPos;
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
	const float MAX_DISTANCE = 300.0f;
	const float DISTANCE = m_playerPos->Distance(m_transform.pos);
	//距離によってスケールを調整する。近距離なら小さく、遠距離なら大きくする。
	const float OFFSET = std::clamp(DISTANCE / MAX_DISTANCE, 0.01f, 1.0f);

	m_transform.scale = (m_baseTransform.scale * OFFSET) * EasingMaker(Out, Exp, m_time.GetTimeRate());
}

void HitCircle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_render.m_tex.Draw3DBillBoard(arg_rasterize, arg_blasVector, m_transform, m_color);
}
