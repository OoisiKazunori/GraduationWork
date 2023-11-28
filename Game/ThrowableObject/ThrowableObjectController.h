#pragma once
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Render/BasicDraw.h"
#include <vector>

class MeshCollision;
class ThrowableObject;

class ThrowableObjectController {

private:

	//�e��t���O
	bool m_isHold;
	bool m_isHoldOld;

	//�����邱�Ƃ��o����I�u�W�F�N�g
	std::array<std::shared_ptr<ThrowableObject>, 150> m_throwableObject;

	//�\�����p�̃I�u�W�F�N�g���o���^�C�}�[
	int generatePredictedObjectTimer;
	const int GENERATE_PREDICTED_OBJECT_TIMER = 1;


public:

	ThrowableObjectController(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update(KazMath::Transform3D arg_playerTransform, KazMath::Vec3<float> arg_throwVec, std::list<std::shared_ptr<MeshCollision>> arg_stageColliders);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void InputHold(bool arg_isHold);

};