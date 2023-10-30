#include "BulletMgr.h"
#include "Bullet.h"
#include "EchoBullet.h"

BulletMgr::BulletMgr(DrawingByRasterize& arg_rasterize) {

	for (auto& index : m_bullet) {

		index = std::make_shared<Bullet>(arg_rasterize);

	}
	for (auto& index : m_echoBullet) {

		index = std::make_shared<EchoBullet>(arg_rasterize);

	}

}

void BulletMgr::Init() {

	for (auto& index : m_bullet) {

		index->Init();

	}
	for (auto& index : m_echoBullet) {

		index->Init();

	}

}

void BulletMgr::Genrate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir, bool arg_isEcho) {

	if (arg_isEcho) {

		for (auto& index : m_echoBullet) {

			if (index->GetIsActive()) continue;

			index->Generate(arg_pos, arg_dir);

			break;

		}

	}
	else {

		for (auto& index : m_bullet) {

			if (index->GetIsActive()) continue;

			index->Generate(arg_pos, arg_dir);

			break;

		}

	}

}

void BulletMgr::Update(std::weak_ptr<MeshCollision> arg_meshCollision) {

	for (auto& index : m_bullet) {

		if (!index->GetIsActive()) continue;

		index->Update(arg_meshCollision);

	}

	for (auto& index : m_echoBullet) {

		if (!index->GetIsActive()) continue;

		index->Update(arg_meshCollision);

	}

}

void BulletMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {

	for (auto& index : m_bullet) {

		if (!index->GetIsActive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

	for (auto& index : m_echoBullet) {

		if (!index->GetIsActive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}