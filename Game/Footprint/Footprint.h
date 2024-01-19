#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class Footprint {

private:

	BasicDraw::BasicModelRender m_model;
	KazMath::Transform3D m_transform;
	int m_exitTimer;
	const int EXIT_TIMER = 240;
	bool m_isActive;
	bool m_inEcho;

	//エコーに映ってから見えなくなるまでのタイマー
	int m_invisibleTimer;
	const int INVISIBLE_TIMER = 90;

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