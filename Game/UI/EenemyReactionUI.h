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
	bool m_appearFlag;			//�o��
	bool m_showEffectFlag;		//�����N�����̂��������茩����
	bool m_preDissappearFlag;	//�����鏀��
	bool m_disappearFlag;		//����

	bool finishFlag;

	void Find();
};