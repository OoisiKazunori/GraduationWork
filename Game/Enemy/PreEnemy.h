#pragma once
#include <array>
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Render/BasicDraw.h"
#include "../Game/UI/Reaction.h"

class MeshCollision;

class PreEnemy {

private:

	BasicDraw::SilhouetteModelRender m_modelRender;
	KazMath::Transform3D m_modelTransform;
	bool m_inEcho;
	Reaction m_reactionRender;

public:

	PreEnemy(DrawingByRasterize& arg_rasterize);

	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision);

	void SetPos(KazMath::Vec3<float> arg_pos) {
		m_modelTransform.pos = arg_pos;
	};

};