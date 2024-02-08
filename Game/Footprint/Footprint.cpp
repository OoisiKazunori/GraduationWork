#include "Footprint.h"
#include "../Echo/EchoArray.h"
#include "../KazLibrary/Easing/easing.h"

Footprint::Footprint(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Footprint/", "footprint.gltf")
{
	Init();
}

void Footprint::Init()
{
	m_isActive = false;
	m_exitTimer = 0;
}

void Footprint::Generate(KazMath::Transform3D arg_generateTransform)
{
	m_transform = arg_generateTransform;
	m_transform.scale = KazMath::Vec3<float>(2.5f, 2.5f, 2.5f);
	m_isActive = true;
	m_exitTimer = 0;
}

void Footprint::Update()
{
	++m_exitTimer;
	if (EXIT_TIMER < m_exitTimer) {
		Init();
	}
}

void Footprint::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	float alpha = static_cast<float>(EXIT_TIMER - m_exitTimer) / static_cast<float>(EXIT_TIMER);

	float easingAmount = EasingMaker(Out, Quart, alpha);

	//アルファが0になったら描画しない。
	if (easingAmount <= 0.05f) return;

	m_model.m_model.DrawRasterize(arg_rasterize, m_transform, KazMath::Color(static_cast<int>(93 * easingAmount), static_cast<int>(44 * easingAmount), static_cast<int>(44 * easingAmount), 255));


	//m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform, KazMath::Color(93, 44, 44, 4));
}
