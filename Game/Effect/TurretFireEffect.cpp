#include "TurretFireEffect.h"
#include"../KazLibrary/Imgui/MyImgui.h"

TurretFireEffect::TurretFireEffect(DrawingByRasterize& arg_rasterize) :
	m_emittTimer(0), m_isActiveFlag(false)
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

void TurretFireEffect::Init(const KazMath::Vec3<float>* arg_pos,const float *arg_radian, float arg_shotTimer)
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
	m_isActiveFlag = true;
	m_stopFlag = false;
}

void TurretFireEffect::Update()
{
	if (m_shotTimer.IsTimeUp() || !m_isActiveFlag)
	{
		return;
	}

	for (auto& obj : m_fireEffectArray)
	{
		if (!obj.IsActive() && !m_stopFlag)
		{
			obj.Init(*m_emittPos,*m_radian);
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
		m_muzzleFlashArray[m_emittTimer].Init(*m_emittPos);
	}
	else
	{
		m_emittTimer = 0;
	}

	if (m_shotTimer.UpdateTimer())
	{
		m_isActiveFlag = false;
	}
}

void TurretFireEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	if (m_shotTimer.IsTimeUp() || !m_isActiveFlag)
	{
		return;
	}

	for (auto& obj : m_fireEffectArray)
	{
		obj.Draw(arg_rasterize, arg_blas);
	}
	if (!m_stopFlag)
	{
		m_muzzleFlashArray[m_emittTimer].Draw(arg_rasterize, arg_blas);
	}
}

void TurretFireEffect::Stop()
{
	m_stopFlag = true;
}
