#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include <cmath>

class InformEnemy
{
public:
	InformEnemy();
	void Load(DrawingByRasterize &arg_rasterize);
	void Update(const KazMath::Vec3<float> & arg_enemyPos, KazMath::Transform3D & arg_playerTransform, bool arg_inSight);
	void Draw(DrawingByRasterize &arg_rasterize, Raytracing::BlasVector &arg_blas);

private:
	KazMath::Transform2D m_transform;
	BasicDraw::BasicTextureRender m_circle;
	float m_alpha;
	const float ALPHA_RANGE = 200.0f;	//•`‰æ‚·‚é‹——£

};

