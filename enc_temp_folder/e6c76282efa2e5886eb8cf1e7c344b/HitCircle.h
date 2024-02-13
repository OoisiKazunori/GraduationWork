#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Raytracing/BlasVector.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/Effect/HitEffect/HitWallParticle.h"

class HitCircle
{
public:
	HitCircle();

	void Load(DrawingByRasterize& arg_rasterize);
	void Init(const KazMath::Vec3<float>& arg_pos, float m_timer, const KazMath::Color& arg_color = KazMath::Color(255, 255, 255, 255));
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector);


private:
	KazMath::Vec3<float>m_pos;
	KazMath::Vec3<float>m_vel;
	float m_fallSpeed;
	bool m_activeFlag;

	KazMath::Transform2D m_transform;
	KazMath::Transform2D m_baseTransform;
	BasicDraw::BasicTextureRender m_render;
	KazMath::Timer m_time;

	KazMath::Color m_color;
};

