#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary/Helper/Timer.h"

class EenemyReactionUI
{
public:
	EenemyReactionUI(DrawingByRasterize& arg_rasterize);

	void Init(int index, const KazMath::Vec3<float>& arg_upVec);
	void Update(const KazMath::Vec3<float>& arg_pos);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool Done()
	{
		return finishFlag;
	}

private:
	BasicDraw::BasicTextureRender m_render;

	int m_index;
	KazMath::Vec3<float>m_pos;
	KazMath::Vec3<float>m_upVec;
	KazMath::Vec2<float>m_upScale;
	KazMath::Vec2<float>m_downScale;
	KazMath::Vec2<float>m_baseScale;
	Timer m_timer;
	std::vector<KazBufferHelper::BufferData>m_tex;


	Timer m_scaleTimer;
	bool m_appearFlag;			//ìoèÍ
	bool m_showEffectFlag;		//âΩÇ™ãNÇ´ÇΩÇÃÇ©ÇµÇ¡Ç©ÇËå©ÇπÇÈ
	bool m_preDissappearFlag;	//è¡Ç¶ÇÈèÄîı
	bool m_disappearFlag;		//è¡ñ≈

	bool finishFlag;

	void Find();
};