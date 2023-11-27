#pragma once
#include <Raytracing/BlasVector.h>
#include <Render/DrawingByRasterize.h>
#include <memory>
#include <array>
#include "../KazLibrary/Math/KazMath.h"

class Bullet;
class EchoBullet;
class MeshCollision;

class BulletMgr {

private:

	static const int BULLET_COUNT = 64;
	std::array<std::shared_ptr<Bullet>, BULLET_COUNT> m_bullet;
	std::array<std::shared_ptr<EchoBullet>, BULLET_COUNT> m_echoBullet;
	KazMath::Vec3<float> m_cameraDir;

public:

	BulletMgr(DrawingByRasterize& arg_rasterize);

	void Init();

	void Genrate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir, bool arg_isEcho = false);

	void Update(std::list<std::shared_ptr<MeshCollision>> arg_stageColliders);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);


};