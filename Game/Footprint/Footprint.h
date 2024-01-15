#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class Footprint {

private:

	BasicDraw::BasicModelRender m_model;
	KazMath::Transform3D m_transform;
	int m_exitTimer;
	const int EXIT_TIMER = 150;
	bool m_isActive;

public:

	Footprint(DrawingByRasterize& arg_rasterize);

	void Init();

	void Generate(KazMath::Transform3D arg_generateTransform);

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);


public:

	inline bool IsActive() {
		return m_isActive;
	}

};