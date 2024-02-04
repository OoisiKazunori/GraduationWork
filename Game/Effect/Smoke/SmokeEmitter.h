#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"SmokeParticle.h"

class SmokeEmitter
{
public:
	struct EmittData
	{
		KazMath::Vec3<float> m_emittPos;
		float m_minScale = 0.005f, m_maxScale = 0.01f;
		float m_minActiveTime = 30, m_maxActiveTime = 50;
		KazMath::Vec3<float> m_range;
		int m_smokeTime;
		bool m_loopFlag;
		KazMath::Color m_color;
	};

	SmokeEmitter();

	void Load(DrawingByRasterize& arg_rasterize);

	/// <summary>
	/// �������BUpdate��Draw���Ă΂�Ă���΁AInit����x�ĂԎ��ŉ�������܂��B
	/// </summary>
	/// <param name="arg_pos">�����n�_</param>
	/// <param name="arg_range">�����͈�</param>
	/// <param name="arg_smokeTimer">��������</param>
	void Init(const EmittData& arg_emittData);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);

private:
	std::array<SmokeParticle, 50>m_smokeParticleArray;
	std::array<int, 50>m_smokeEmittTimeArray;
	KazMath::Vec3<float>m_emittPos;
	float m_minScale, m_maxScale;
	KazMath::Vec3<float>m_range;
	int m_emittTimer, m_emittMaxTimer;
	float m_minActiveTime, m_maxActiveTime;
	bool m_loopFlag;

	KazMath::Color m_color;
};

