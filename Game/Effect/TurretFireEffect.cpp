#include "TurretFireEffect.h"
#include"../KazLibrary/Imgui/MyImgui.h"

TurretFireEffect::TurretFireEffect(DrawingByRasterize& arg_rasterize) :
	m_emittTimer(0)
{
	for (auto& obj : m_fireEffectArray)
	{
		obj.Load(arg_rasterize);
	}
	for (auto& obj : m_muzzleFlashArray)
	{
		obj.Load(arg_rasterize);
	}
}

void TurretFireEffect::Init(const KazMath::Vec3<float>& arg_pos, float arg_radian, float arg_shotTimer)
{
	m_emittPos = arg_pos;
	m_radian = arg_radian;

	m_emittTimer = 0;
	for (auto& obj : m_muzzleFlashArray)
	{
		obj.Finalize();
	}
	for (auto& obj : m_fireEffectArray)
	{
		obj.Finalize();
	}
	m_shotTimer.Reset(arg_shotTimer);
}

void TurretFireEffect::Update()
{
	if (m_shotTimer.IsTimeUp())
	{
		return;
	}

	for (auto& obj : m_fireEffectArray)
	{
		if (!obj.IsActive())
		{
			obj.Init(m_emittPos, KazMath::AngleToRadian(m_radian));
			break;
		}
	}
	for (auto& obj : m_fireEffectArray)
	{
		obj.Update();
	}

	++m_emittTimer;
	if (m_emittTimer < m_muzzleFlashArray.size())
	{
		m_muzzleFlashArray[m_emittTimer].Init(m_emittPos);
	}
	else
	{
		m_emittTimer = 0;
	}

	m_shotTimer.UpdateTimer();
}

void TurretFireEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	if (m_shotTimer.IsTimeUp())
	{
		return;
	}

	for (auto& obj : m_fireEffectArray)
	{
		obj.Draw(arg_rasterize, arg_blas);
	}

	m_muzzleFlashArray[m_emittTimer].Draw(arg_rasterize, arg_blas);

	ImGui::Begin("Turret");
	ImGui::DragFloat("angle", &m_radian);
	ImGui::End();
}
