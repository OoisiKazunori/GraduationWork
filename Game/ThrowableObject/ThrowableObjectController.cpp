#include "ThrowableObjectController.h"
#include "../Game/Collision/MeshCollision.h"

ThrowableObjectController::ThrowableObjectController(DrawingByRasterize& arg_rasterize)
{

	m_isHold = false;
	m_isHoldOld = false;

	for (auto& index : m_line) {
		index.Generate(arg_rasterize);
	}

}

void ThrowableObjectController::Init()
{

	m_isHold = false;
	m_isHoldOld = false;

}

void ThrowableObjectController::Update(KazMath::Vec3<float> arg_playerPos, KazMath::Vec3<float> arg_throwVec, std::list<std::shared_ptr<MeshCollision>> arg_stageColliders)
{

	//入力されていたら
	if (m_isHold) {

		//重力
		float gravity = 0.0f;
		const float ADD_GRAVITY = 0.5f;

		//移動速度
		float vel = 5.0f;
		const float SUB_VEL = 0.2f;

		//座標
		KazMath::Vec3<float> position = arg_playerPos;

		for (int counter = 0; counter < 50; ++counter) {

			//座標を保存する。
			KazMath::Vec3<float> prevPos = position;

			//移動させる。
			position += arg_throwVec * vel;
			position += KazMath::Vec3<float>(0.0f, -1.0f, 0.0f) * gravity;

			//移動量を減衰させる。
			gravity += ADD_GRAVITY;
			vel = std::clamp(vel - SUB_VEL, 0.0f, 100.0f);

			//座標を保存。
			m_dottedLineVector.emplace_back(position);

		}

	}

}

void ThrowableObjectController::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//座標が2個以上入っているか？
	if (2 <= static_cast<int>(m_dottedLineVector.size())) {

		//for (int index = 0; index < static_cast<int>(m_dottedLineVector.size()) - 1; ++index) {

		//	m_line[index].m_render.Draw(arg_rasterize, arg_blasVec, m_dottedLineVector[index], m_dottedLineVector[index + 1], KazMath::Color(255, 255, 255, 255));

		//}
		m_line[0].m_render.Draw(arg_rasterize, arg_blasVec, m_dottedLineVector[0], m_dottedLineVector[1], KazMath::Color(255, 255, 255, 255));

	}

	m_dottedLineVector.clear();

}

void ThrowableObjectController::InputHold(bool arg_isHold)
{

	m_isHoldOld = m_isHold;
	m_isHold = arg_isHold;

}
