#pragma once
#include "../KazLibrary/Render/BasicDraw.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include <memory>
#include <array>

class Footprint;

class FootprintMgr : public ISingleton<FootprintMgr> {

private:

	static const int FOOTPRINT_COUNT = 128;
	std::array<std::shared_ptr<Footprint>, FOOTPRINT_COUNT> m_footprints;

public:

	void Setting(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update();
	void Generate(KazMath::Transform3D arg_generateTransform);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

};