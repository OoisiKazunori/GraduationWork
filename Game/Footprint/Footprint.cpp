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
	m_inEcho = false;
	m_exitTimer = 0;
	m_invisibleTimer = 0;
}

void Footprint::Generate(KazMath::Transform3D arg_generateTransform)
{
	m_transform = arg_generateTransform;
	m_isActive = true;
	m_exitTimer = 0;
	m_invisibleTimer = 0;
}

void Footprint::Update()
{
	m_inEcho = false;
	for (auto& index : EchoArray::Instance()->GetEcho()) {

		if (!index.GetIsActive()) continue;
		if (index.GetNowRadius() <= 0.1f) continue;

		//球の範囲内にあるかをチェック。
		float distance = KazMath::Vec3<float>(m_transform.pos - index.GetPos()).Length();
		if (index.GetNowRadius() < distance) continue;

		m_inEcho = true;
		m_invisibleTimer = INVISIBLE_TIMER;
		break;

	}

	if (!m_inEcho) {
		m_invisibleTimer = std::clamp(m_invisibleTimer - 1, 0, INVISIBLE_TIMER);
	}

	++m_exitTimer;
	if (EXIT_TIMER < m_exitTimer) {
		Init();
	}
}

void Footprint::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (m_invisibleTimer <= 0) return;
	m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}
