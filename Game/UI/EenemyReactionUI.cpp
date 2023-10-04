#include "EenemyReactionUI.h"

EenemyReactionUI::EenemyReactionUI(DrawingByRasterize& arg_rasterize) :
	m_render(arg_rasterize)
{
	m_timer.Reset(120);
	m_timer.ForciblyTimeUp();
	m_tex.emplace_back(TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/Reaction/find.png"));
	m_tex.emplace_back(TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/Reaction/hatena.png"));
	m_index = 0;
}

void EenemyReactionUI::Init(int index, const KazMath::Vec3<float>& arg_upVec)
{
	m_index = index;
	m_timer.Reset(120);
	finishFlag = false;
	m_appearFlag = true;
	m_scaleTimer.Reset(5);

	m_baseScale = { 2.0f,5.0f };
	m_downScale = m_baseScale;
	m_upVec = arg_upVec;
}

void EenemyReactionUI::Update(const KazMath::Vec3<float>& arg_pos)
{
	m_pos = arg_pos + KazMath::Vec3<float>(10.0f, 10.0f, 10.0f) * EasingMaker(Out, Back, 1.0f);
	m_timer.UpdateTimer();

	Find();
	m_scaleTimer.UpdateTimer();
}

void EenemyReactionUI::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (!finishFlag)
	{
		KazMath::Transform3D transform;
		transform.pos = m_pos;
		transform.scale = { 0.01f,0.01f,1.0f };
		m_render.m_tex.Draw3DBillBoard(arg_rasterize, arg_blasVec, transform, m_tex[m_index]);
	}
	else
	{
		m_pos = { 0.0f,0.0f,0.0f };
	}
}

void EenemyReactionUI::Find()
{
	KazMath::Vec2<float>appearScale(1.5f, 0.0f);
	KazMath::Vec2<float>showScale(1.0f, 1.0f);
	KazMath::Vec2<float>preDisappearScale(1.5f, 0.0f);
	KazMath::Vec2<float>disappearScale(0.0f, 1.0f);

	//ìoèÍ
	if (m_appearFlag)
	{
		float rate = EasingMaker(Out, Circ, m_scaleTimer.GetTimeRate());
		m_upScale = KazMath::Vec2<float>(1.5f, -m_baseScale.y) + (showScale * m_baseScale) * rate;

		if (m_scaleTimer.IsTimeUp())
		{
			m_appearFlag = false;
			m_showEffectFlag = true;
			m_scaleTimer.Reset(30);
		}
	}
	//ébÇ≠å©ÇπÇÈ
	if (m_showEffectFlag)
	{
		if (m_scaleTimer.IsTimeUp())
		{
			m_showEffectFlag = false;
			m_preDissappearFlag = true;
			m_scaleTimer.Reset(5);
		}
	}
	//âBÇÍÇÈê°ëO
	if (m_preDissappearFlag)
	{
		float rate = EasingMaker(Out, Circ, m_scaleTimer.GetTimeRate());
		m_upScale = showScale * m_baseScale + (preDisappearScale * m_baseScale) * rate;
		m_downScale.x = m_upScale.x;
		if (m_scaleTimer.IsTimeUp())
		{
			m_preDissappearFlag = false;
			m_disappearFlag = true;
			m_scaleTimer.Reset(5);
		}
	}
	//è¡ñ≈
	if (m_disappearFlag)
	{
		float rate = EasingMaker(Out, Circ, m_scaleTimer.GetTimeRate());
		m_upScale = preDisappearScale * m_baseScale + (disappearScale * m_baseScale) * rate;
		m_downScale.x = m_upScale.x;
		if (m_scaleTimer.IsTimeUp())
		{
			m_disappearFlag = false;
			m_scaleTimer.Reset(60);
		}
	}
}