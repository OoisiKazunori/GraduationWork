#include "ThrowableObject.h"
#include "../Game/Collision/MeshCollision.h"

ThrowableObject::ThrowableObject(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Bullet/", "EchoBullet.gltf")
{

	Init();

}

void ThrowableObject::Init()
{

	m_isActive = false;

}

void ThrowableObject::Update(std::list<std::shared_ptr<MeshCollision>> f_stageColliders)
{
}

void ThrowableObject::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
}
