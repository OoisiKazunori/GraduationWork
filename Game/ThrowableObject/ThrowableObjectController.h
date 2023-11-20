#pragma once
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Render/BasicDraw.h"
#include <vector>

class ThrowableObjectController {

private:

	//�e��t���O
	bool m_isHold;
	bool m_isHoldOld;

	//�_����`�悷�邽�߂̔z��
	std::vector<KazMath::Vec3<float>> m_dottedLineVector;
	BasicDraw::BasicLineRender m_line;


public:

	ThrowableObjectController(DrawingByRasterize& arg_rasterize);
	void Init();
	void Update(KazMath::Vec3<float> arg_playerPos, KazMath::Vec3<float> arg_throwVec);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void InputHold(bool arg_isHold);

};