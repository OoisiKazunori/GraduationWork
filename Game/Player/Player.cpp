#include "Player.h"
#include "Input/KeyBoradInputManager.h"
#include "Input/ControllerInputManager.h"
#include "../Game/Collision/MeshCollision.h"

Player::Player(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf")
{
	Init();
}

void Player::Init()
{

	m_playerAttitude = PlayerAttitude::STAND;
	m_onGround = false;
	m_gravity = 0.0f;

}

void Player::Update(KazMath::Transform3D arg_cameraQuaternion, std::weak_ptr<MeshCollision> arg_stageMeshCollision)
{

	//動かす前の座標。
	m_prevPos = m_transform.pos;

	//入力処理
	Input(arg_cameraQuaternion);

	//当たり判定
	Collision(arg_stageMeshCollision);

	//重力をかける。
	if (!m_onGround) {
		m_gravity -= GRAVITY;
	}
	m_transform.pos.y += m_gravity;

	//動いた方向に回転させる。
	Rotate();

	//現在の姿勢のステータスによってモデルのスケール量をいじる。アニメーションとかモデルを置き変える処理の代替処理。
	switch (m_playerAttitude)
	{
	case Player::PlayerAttitude::STAND:
		m_transform.scale.x += (1.0f - m_transform.scale.x) / 5.0f;
		m_transform.scale.y += (1.0f - m_transform.scale.y) / 5.0f;
		m_transform.scale.z += (1.0f - m_transform.scale.z) / 5.0f;
		break;
	case Player::PlayerAttitude::SQUAT:
		m_transform.scale.x += (1.0f - m_transform.scale.x) / 5.0f;
		m_transform.scale.y += (0.5f - m_transform.scale.y) / 5.0f;
		m_transform.scale.z += (1.0f - m_transform.scale.z) / 5.0f;
		break;
	case Player::PlayerAttitude::CREEPING:
		m_transform.scale.x += (1.0f - m_transform.scale.x) / 5.0f;
		m_transform.scale.y += (0.2f - m_transform.scale.y) / 5.0f;
		m_transform.scale.z += (3.0f - m_transform.scale.z) / 5.0f;
		break;
	default:
		break;
	}

}

void Player::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}

void Player::Input(KazMath::Transform3D arg_cameraQuaternion)
{

	//前方向と右方向のベクトルを取得。 -をつけているのは、arg_cameraQuaternionがプレイヤーから見たカメラの方向だから。
	KazMath::Vec3<float> frontVec = -arg_cameraQuaternion.GetFront();
	frontVec.y = 0;
	KazMath::Vec3<float> rightVec = -arg_cameraQuaternion.GetRight();
	rightVec.y = 0;

	//前後左右に移動する。
	KazMath::Vec3<float> inputMoveVec = {};
	if (KeyBoradInputManager::Instance()->InputState(DIK_W)) {
		inputMoveVec += frontVec;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_S)) {
		inputMoveVec -= frontVec;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_A)) {
		inputMoveVec -= rightVec;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_D)) {
		inputMoveVec += rightVec;
	}
	m_transform.pos += inputMoveVec.GetNormal() * GetMoveSpeed();

	//CTRLが押されたらステータスを切り返る。
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_LCONTROL)) {
		switch (m_playerAttitude)
		{
		case Player::PlayerAttitude::STAND:
			m_playerAttitude = PlayerAttitude::SQUAT;
			break;
		case Player::PlayerAttitude::SQUAT:
			m_playerAttitude = PlayerAttitude::CREEPING;
			break;
		case Player::PlayerAttitude::CREEPING:
			m_playerAttitude = PlayerAttitude::STAND;
			break;
		default:
			break;
		}
	}

}

void Player::Rotate()
{

	KazMath::Vec3<float> movedVec = m_transform.pos - m_prevPos;
	movedVec.y = 0.0f;
	if (0 < movedVec.Length()) {

		//動いた方向。
		movedVec.Normalize();

		//デフォルトの正面ベクトルからの回転量を求める。
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(movedVec));

		//クォータニオンを求める。
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(movedVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//回転を適応
		m_transform.quaternion = DirectX::XMQuaternionSlerp(m_transform.quaternion, rotateQ, 0.15f);

	}

}

void Player::Collision(std::weak_ptr<MeshCollision> arg_meshCollision)
{


	const float RAY_LENGTH = 5.0f;

	//地面と当たり判定を行う。
	m_onGround = false;
	const float GROUND_RAY_OFFSET = 5.0f;
	MeshCollision::CheckHitResult rayResult = arg_meshCollision.lock()->CheckHitRay(m_transform.pos + m_transform.GetUp() * GROUND_RAY_OFFSET, -m_transform.GetUp());
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH + GROUND_RAY_OFFSET) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH + GROUND_RAY_OFFSET - rayResult.m_distance);
		m_onGround = true;

	}

	//当たり判定を計算。
	rayResult = arg_meshCollision.lock()->CheckHitRay(m_transform.pos, m_transform.GetFront());
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}
	//右方向
	rayResult = arg_meshCollision.lock()->CheckHitRay(m_transform.pos, m_transform.GetRight());
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}
	//左方向
	rayResult = arg_meshCollision.lock()->CheckHitRay(m_transform.pos, -m_transform.GetRight());
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}

}

float Player::GetMoveSpeed()
{
	switch (m_playerAttitude)
	{
	case Player::PlayerAttitude::STAND:
		return MOVE_SPEED_STAND;
		break;
	case Player::PlayerAttitude::SQUAT:
		return MOVE_SPEED_SQUAT;
		break;
	case Player::PlayerAttitude::CREEPING:
		return MOVE_SPEED_CREEPING;
		break;
	default:
		break;
	}
	return MOVE_SPEED_STAND;
}
