#include "Gauge.h"
#include <iostream>
#include <algorithm>

Gauge::Gauge() :m_nowRate(0.0f), m_maxRate(0.0f)
{
}

void Gauge::Init(float arg_maxRate, bool arg_rangeFlag, float arg_nowRate)
{
	m_nowRate = arg_nowRate;
	m_maxRate = arg_maxRate;

	if (arg_rangeFlag)
	{
		m_minRate = -1.0f;
	}
	else
	{
		m_minRate = 0.0f;
	}
}

float Gauge::GetRate()
{
	return std::clamp(m_nowRate / m_maxRate, m_minRate, 1.0f);
}

bool Gauge::IsMax()
{
	return 1.0f <= GetRate();
}

bool Gauge::IsEmpty()
{
	return GetRate() <= 0.0f;
}

void Gauge::Reset()
{
	m_nowRate = 0.0f;
}

void Gauge::Incre(float arg_increNum)
{
	m_nowRate += arg_increNum;
}

void Gauge::Decre(float arg_decreNum)
{
	m_nowRate -= arg_decreNum;
}