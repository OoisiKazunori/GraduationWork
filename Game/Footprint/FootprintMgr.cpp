#include "FootprintMgr.h"
#include "Footprint.h"

void FootprintMgr::Init()
{

	for (auto& index : m_footprints) {

		index->Init();

	}

}

void FootprintMgr::Update()
{

	for (auto& index : m_footprints) {

		if (!index->IsActive()) continue;

		index->Update();

	}

}

void FootprintMgr::Generate(KazMath::Transform3D arg_generateTransform, DrawingByRasterize& arg_rasterize)
{

	for (auto& index : m_footprints) {

		if (index->IsActive()) continue;

		index->Generate(arg_generateTransform);
		
		return;

	}

	m_footprints.emplace_back(std::make_shared<Footprint>(arg_rasterize));
	m_footprints.back()->Generate(arg_generateTransform);

}

void FootprintMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	for (auto& index : m_footprints) {

		if (!index->IsActive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}
