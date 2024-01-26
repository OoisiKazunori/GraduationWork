#include "FieldAI.h"
#include"../KazLibrary/Debug/DebugKey.h"

FieldAI::FieldAI()
{
	m_waringGauge.Init(100.0f, true);
}

float FieldAI::GetWaringRate()
{
	return m_waringGauge.GetRate();
}

void FieldAI::DebugUpdate()
{
	if (DebugKey::Instance()->DebugKeyTrigger(DIK_J, "WaringRate-Incre", "DIK_J"))
	{
		m_waringGauge.Incre(10.0f);
	}
	if (DebugKey::Instance()->DebugKeyTrigger(DIK_K, "WaringRate-Decre", "DIK_K"))
	{
		m_waringGauge.Decre(10.0f);
	}
}
