#include "FieldAI.h"

FieldAI::FieldAI()
{
	m_waringGauge.Init(100.0f, true);
}

float FieldAI::GetWaringRate()
{
	return m_waringGauge.GetRate();
}
