#include "Echo.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Effect/StopMgr.h"
#include <algorithm>

Echo::Echo()
{

	m_isActive = false;

}

void Echo::Init()
{

	m_isActive = false;
	m_echoMemoryData.m_isActive = false;

}

void Echo::Generate(KazMath::Vec3<float> arg_pos, float arg_maxEchoRadius, COLOR arg_echoColorID, bool arg_isMemory)
{

	//通常のEchoの情報を詰め込んでおく。
	m_echoData.m_pos = arg_pos;
	m_echoData.m_radius = 0.0f;
	m_echoData.m_alpha = ALPHA;
	m_echoData.m_colorID = static_cast<int>(arg_echoColorID);

	m_maxEchoRadius = arg_maxEchoRadius;
	m_isActive = true;

	m_status = STATUS::APPEAR;
	m_easingTimer = 0.0f;

	m_isMemory = arg_isMemory;

}

void Echo::Update()
{

	switch (m_status)
	{
	case Echo::STATUS::APPEAR:
	{

		m_easingTimer = std::clamp(m_easingTimer + 1.0f * StopMgr::Instance()->GetGameSpeed(), 0.0f, APPEAR_EASING_TIMER);

		float easingAmount = EasingMaker(Out, Cubic, m_easingTimer / APPEAR_EASING_TIMER);
		m_echoData.m_radius = m_maxEchoRadius * easingAmount;
		m_echoMemoryData.m_radius = m_echoData.m_radius;

		if (APPEAR_EASING_TIMER <= m_easingTimer) {

			m_easingTimer = 0.0f;
			m_status = STATUS::EXIT;

		}

	}
	break;
	case Echo::STATUS::EXIT:
	{

		//記録用かそうじゃないかで消えるまでのタイマーを変える。
		float exitEasingTimer = EXIT_EASING_TIMER;
		if (m_isMemory) {
			exitEasingTimer = EXIT_EASING_TIMER_MEMORY;
		}

		m_easingTimer = std::clamp(m_easingTimer + 1.0f * StopMgr::Instance()->GetGameSpeed(), 0.0f, exitEasingTimer);

		float easingAmount = EasingMaker(In, Cubic, m_easingTimer / exitEasingTimer);
		m_echoData.m_alpha = (1.0f - easingAmount) * ALPHA;

		if (exitEasingTimer <= m_easingTimer) {

			Init();

		}
	}
	break;
	default:
		break;
	}


}
