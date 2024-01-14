#pragma once
#include "../KazLibrary/Render/BasicDraw.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include <memory>
#include <vector>

class Footprint;

class FootprintMgr : public ISingleton<FootprintMgr> {

private:

	std::vector<std::shared_ptr<Footprint>> m_footprints;

public:

	void Init();
	void Update();
	void Generate(KazMath::Transform3D arg_generateTransform, DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

};