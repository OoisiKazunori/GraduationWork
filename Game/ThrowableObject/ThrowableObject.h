#pragma once
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Render/BasicDraw.h"

class MeshCollision;

class ThrowableObject {

private:

	BasicDraw::BasicModelRender m_model;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_throwVector;
	float m_gravity;
	float m_vel;
	bool m_isActive;

	//d—Í‰ÁZ—Ê
	const float ADD_GRAVITY = 0.1f;
	//ˆÚ“®‘¬“x‚ğ‰º‚°‚é—Ê
	const float SUB_VEL = 0.2f;

public:

	ThrowableObject(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update(std::list<std::shared_ptr<MeshCollision>> f_stageColliders);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; }

};