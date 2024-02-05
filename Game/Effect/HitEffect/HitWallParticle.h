#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Math/Timer.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Raytracing/BlasVector.h"
#include"../KazLibrary/Render/BasicDraw.h"

class HitWallParticle
{
public:
	HitWallParticle();

	void Load(DrawingByRasterize& arg_rasterize);
	void Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_vel, float arg_fallSpeed = 0.01f);
	void Finalize();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector);

private:
	KazMath::Vec3<float>m_pos;
	KazMath::Vec3<float>m_vel;
	float m_fallSpeed;
	bool m_activeFlag;
	//パーティクルの軌跡
	class Particle
	{
	public:
		Particle();

		void Load(DrawingByRasterize& arg_rasterize);
		void Init(const KazMath::Vec3<float>& arg_pos);
		void Finalize();
		void Update();
		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector);

		bool IsActive();

	private:
		KazMath::Transform3D m_transform;
		KazMath::Transform3D m_lerpTransform;
		BasicDraw::BasicTextureRender m_render;
		bool m_activeFlag;
		KazMath::Timer m_time;
	};
	std::array<Particle, 15> m_particle;

	int m_emitterTime;
};

