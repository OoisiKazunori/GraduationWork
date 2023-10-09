#pragma once
#include <Raytracing/BlasVector.h>
#include <Render/DrawingByRasterize.h>
#include <memory>
#include <array>
#include "../KazLibrary/Math/KazMath.h"

class Bullet;
class MeshCollision;

class BulletMgr {

private:

	static const int BULLET_COUNT = 64;
	std::array<std::shared_ptr<Bullet>, BULLET_COUNT> m_bullet;
	KazMath::Vec3<float> m_cameraDir;

public:

	BulletMgr(DrawingByRasterize& arg_rasterize);

	void Init();

	void Genrate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir);

	void Update(std::weak_ptr<MeshCollision> arg_meshCollision);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);


};