#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"
#include"../KazLibrary/Math/Timer.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"

class Reaction
{
public:
	Reaction(DrawingByRasterize &arg_rasterize)
	{
		m_timer.Reset(120);
		m_timer.ForciblyTimeUp();
		m_tex.emplace_back(TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/Reaction/!.png"));
		m_tex.emplace_back(TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/Reaction/Hatena.png"));
		m_index = 0;

		m_render.Load(arg_rasterize, false, false);
	}

	void Init(int index, const KazMath::Vec3<float>& upVec)
	{
		m_index = index;
		m_timer.Reset(120);
		finishFlag = false;
		m_appearFlag = true;
		m_scaleTimer.Reset(5);

		m_baseScale = { 2.0f,5.0f };
		m_downScale = m_baseScale;
		m_upVec = upVec;
	}

	void Update(const KazMath::Vec3<float>& pos)
	{
		float rate = EasingMaker(Out, Back, m_timer.GetTimeRate());
		m_pos = pos + (KazMath::Vec3<float>(2.0f, 2.0f, 2.0f) * m_upVec) * rate;
		m_timer.UpdateTimer();

		Find();
		m_scaleTimer.UpdateTimer();
	}

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
	{
		//m_pos = { 0.0f,-47.2f,0.0f };
		//if (!finishFlag)
		{
			m_render.Draw(arg_rasterize, arg_blasVec, m_pos, m_upScale, m_downScale, m_tex[m_index]);
		}
		//else
		{
		//	m_pos = { 0.0f,0.0f,0.0f };
		}
	}

	bool Done()
	{
		return finishFlag;
	}

private:
	int m_index;
	KazMath::Vec3<float>m_pos;
	KazMath::Vec3<float>m_upVec;
	KazMath::Vec2<float>m_upScale;
	KazMath::Vec2<float>m_downScale;
	KazMath::Vec2<float>m_baseScale;
	KazMath::Timer m_timer;
	std::vector<KazBufferHelper::BufferData>m_tex;

	//ï`âÊ
	DrawFuncHelper::TextureRectRender m_render;


	KazMath::Timer m_scaleTimer;
	bool m_appearFlag;			//ìoèÍ
	bool m_showEffectFlag;		//âΩÇ™ãNÇ´ÇΩÇÃÇ©ÇµÇ¡Ç©ÇËå©ÇπÇÈ
	bool m_preDissappearFlag;	//è¡Ç¶ÇÈèÄîı
	bool m_disappearFlag;		//è¡ñ≈

	bool finishFlag;

	void Find()
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
};