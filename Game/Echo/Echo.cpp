#include "Echo.h"
#include "../KazLibrary/Easing/easing.h"
#include <algorithm>

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
	m_echoData.m_alpha = ALPHA;
	m_echoData.m_color = arg_echoColor;

	m_maxEchoRadius = arg_maxEchoRadius;
	m_isActive = true;

	m_status = STATUS::APPEAR;
	m_easingTimer = 0.0f;

}

void Echo::Update()
{

	switch (m_status)
	{
	case Echo::STATUS::APPEAR:
	{

		m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, APPEAR_EASING_TIMER);

		float easingAmount = EasingMaker(Out, Cubic, m_easingTimer / APPEAR_EASING_TIMER);
		m_echoData.m_radius = m_maxEchoRadius * easingAmount;

		if (APPEAR_EASING_TIMER <= m_easingTimer) {

			m_easingTimer = 0.0f;
			m_status = STATUS::EXIT;

		}

	}
	break;
	case Echo::STATUS::EXIT:
	{

		m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, EXIT_EASING_TIMER);

		float easingAmount = EasingMaker(In, Cubic, m_easingTimer / EXIT_EASING_TIMER);
		m_echoData.m_alpha = (1.0f - easingAmount) * ALPHA;

		if (EXIT_EASING_TIMER <= m_easingTimer) {

			Init();

		}
	}
	break;
	default:
		break;
	}


}
