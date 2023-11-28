#include "ThrowableObjectController.h"
#include "../Game/Collision/MeshCollision.h"
#include "../KazLibrary/Imgui/imgui.h"
#include "../Game/ThrowableObject/ThrowableObject.h"

ThrowableObjectController::ThrowableObjectController(DrawingByRasterize& arg_rasterize)
{

	m_isHold = false;
	m_isHoldOld = false;
	generatePredictedObjectTimer = 0;

	for (auto& index : m_throwableObject) {

		index = std::make_shared<ThrowableObject>(arg_rasterize);

	}

}

void ThrowableObjectController::Init()
{

	m_isHold = false;
	m_isHoldOld = false;
	generatePredictedObjectTimer = 0;

}

void ThrowableObjectController::Update(KazMath::Transform3D arg_playerTransform, KazMath::Vec3<float> arg_throwVec, std::list<std::shared_ptr<MeshCollision>> arg_stageColliders)
{

	//生成座標をずらす。
	arg_playerTransform.pos -= arg_playerTransform.GetUp() * 1.5f;
	arg_playerTransform.pos -= arg_playerTransform.GetRight() * 4.0f;

	//入力されていたら
	if (!m_isHold && m_isHoldOld) {

		for (auto& index : m_throwableObject) {

			if (index->GetIsActive()) continue;

			index->Generate(arg_playerTransform, arg_throwVec, 5.0f, false);

			break;

		}

	}
	else if (m_isHold) {

		//入力され続ている状態だったら。
		++generatePredictedObjectTimer;
		if (GENERATE_PREDICTED_OBJECT_TIMER <= generatePredictedObjectTimer) {

			//生成する。
			for (auto& index : m_throwableObject) {

				if (index->GetIsActive()) continue;

				index->Generate(arg_playerTransform, arg_throwVec, 5.0f, true);

				break;

			}

			generatePredictedObjectTimer = 0;

		}

	}


	for (auto& index : m_throwableObject) {

		if (!index->GetIsActive()) continue;

		index->Update(arg_stageColliders);

	}

}

void ThrowableObjectController::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	for (auto& index : m_throwableObject) {

		if (!index->GetIsActive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}

void ThrowableObjectController::InputHold(bool arg_isHold)
{

	m_isHoldOld = m_isHold;
	m_isHold = arg_isHold;

}
