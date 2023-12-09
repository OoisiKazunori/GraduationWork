#include "ThrowableObjectController.h"
#include "../Game/Collision/MeshCollision.h"
#include "../KazLibrary/Imgui/imgui.h"
#include "../Game/ThrowableObject/ThrowableObject.h"

ThrowableObjectController::ThrowableObjectController(DrawingByRasterize& arg_rasterize)
{

	Init();

	for (auto& index : m_throwableObject) {

		index = std::make_shared<ThrowableObject>(arg_rasterize);

	}

}

void ThrowableObjectController::Init()
{

	m_isHold = false;
	m_isHoldOld = false;
	generatePredictedObjectTimer = 0;
	m_throwDelay = 0;

}

void ThrowableObjectController::Update(KazMath::Transform3D arg_playerTransform, KazMath::Vec3<float> arg_throwVec, std::list<std::shared_ptr<MeshCollision>> arg_stageColliders)
{

	//�������W�����炷�B
	arg_playerTransform.pos -= arg_playerTransform.GetUp() * 1.5f;
	arg_playerTransform.pos -= arg_playerTransform.GetRight() * 4.0f;

	//�����邱�Ƃ��ł���܂ł̃^�C�}�[�̒x�����܂��B���Ă��Ȃ�������AHOLD��Ԃ���������B
	if (0 < m_throwDelay) {

		m_isHold = false;
		--m_throwDelay;

	}

	//���͂���Ă�����
	if (!m_isHold && m_isHoldOld) {

		for (auto& index : m_throwableObject) {

			if (index->GetIsActive()) continue;

			index->Generate(arg_playerTransform, arg_throwVec, 5.0f, false);

			m_throwDelay = THROW_DELAY;

			break;

		}

	}
	else if (m_isHold) {

		//���͂��ꑱ�Ă����Ԃ�������B
		++generatePredictedObjectTimer;
		if (GENERATE_PREDICTED_OBJECT_TIMER <= generatePredictedObjectTimer) {

			//��������B
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
