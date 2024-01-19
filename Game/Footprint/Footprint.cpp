#include "Footprint.h"
#include "../Echo/EchoArray.h"

Footprint::Footprint(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Test/", "footprint.gltf")
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
	m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}
