#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Math/Timer.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Render/BasicDraw.h"

class SmokeParticle
{
public:
	SmokeParticle();

	void Load(DrawingByRasterize& arg_rasterize);
	void Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_scale, float arg_emittTimer, const KazMath::Color& arg_color);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);

	const bool& IsActive();
private:
	KazMath::Transform3D m_transform, m_baseTransform;
	BasicDraw::BasicTextureRender m_smokeRender;
	KazMath::Timer m_timer;
	float m_speed;
	bool m_activeFlag;
	float m_angle, m_angleVel;
	KazMath::Color m_color;
};

