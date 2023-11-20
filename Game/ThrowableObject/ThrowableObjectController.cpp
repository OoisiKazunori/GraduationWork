#include "ThrowableObjectController.h"

ThrowableObjectController::ThrowableObjectController(DrawingByRasterize& arg_rasterize) :
	m_line(arg_rasterize)
{

	m_isHold = false;
	m_isHoldOld = false;

}

void ThrowableObjectController::Init()
{

	m_isHold = false;
	m_isHoldOld = false;

}

void ThrowableObjectController::Update(KazMath::Vec3<float> arg_playerPos, KazMath::Vec3<float> arg_throwVec)
{

	//“ü—Í‚³‚ê‚Ä‚¢‚½‚ç
	m_dottedLineVector.clear();
	if (m_isHold) {

		m_dottedLineVector.emplace_back(arg_playerPos);
		m_dottedLineVector.emplace_back(arg_playerPos + arg_throwVec * 100.0f);

	}

}

void ThrowableObjectController::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	if (0 < static_cast<int>(m_dottedLineVector.size())) {

		m_line.m_render.Draw(arg_rasterize, arg_blasVec, m_dottedLineVector[0], m_dottedLineVector[1], KazMath::Color(255, 0, 0, 255));

	}
	m_dottedLineVector.clear();

}

void ThrowableObjectController::InputHold(bool arg_isHold)
{

	m_isHoldOld = m_isHold;
	m_isHold = arg_isHold;

}
