#include "Echo.h"

Echo::Echo()
{

	m_isActive = false;

}

void Echo::Init()
{

	m_isActive = false;

}

void Echo::Generate(KazMath::Vec3<float> arg_pos, float arg_maxEchoRadius, KazMath::Vec3<float> arg_echoColor)
{

	m_echoData.m_pos = arg_pos;
	m_echoData.m_radius = 0.0f;
	m_echoData.m_alpha = 1.0f;
	m_echoData.m_color = arg_echoColor;

	m_maxEchoRadius = arg_maxEchoRadius;
	m_isActive = true;

}

void Echo::Update()
{

	//ˆê’UŒÅ’è
	m_echoData.m_radius = m_maxEchoRadius;

}
