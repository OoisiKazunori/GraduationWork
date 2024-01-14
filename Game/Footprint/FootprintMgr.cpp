#include "FootprintMgr.h"
#include "Footprint.h"

void FootprintMgr::Setting(DrawingByRasterize& arg_rasterize)
{

	for (auto& index : m_footprints) {

		index = std::make_shared<Footprint>(arg_rasterize);

	}

}

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

void FootprintMgr::Generate(KazMath::Transform3D arg_generateTransform)
{

	for (auto& index : m_footprints) {

		if (index->IsActive()) continue;

		index->Generate(arg_generateTransform);

		return;

	}

}

void FootprintMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	for (auto& index : m_footprints) {

		if (!index->IsActive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}
