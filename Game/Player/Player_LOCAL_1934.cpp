#include "Player.h"
#include "Input/KeyBoradInputManager.h"
#include "Input/ControllerInputManager.h"
#include "../Game/Collision/MeshCollision.h"
#include "../Bullet/BulletMgr.h"
#include "../Camera.h"
#include "../KazLibrary/PostEffect/Outline.h"
#include "../Echo/EchoArray.h"
#include "../ThrowableObject/ThrowableObjectController.h"

Player::Player(DrawingByRasterize& arg_rasterize, KazMath::Transform3D f_startPos) :
	m_model(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf")
{
	m_transform = f_startPos;
	Init();
}

void Player::Init()
{

	m_playerAttitude = PlayerAttitude::STAND;
	m_onGround = false;
	m_isADS = false;
	m_gravity = 0.0f;

}

void Player::Update(std::weak_ptr<Camera> arg_camera, std::weak_ptr<MeshCollision> arg_stageMeshCollision, std::weak_ptr<BulletMgr> arg_bulletMgr, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController)
{

	//動かす前の座標。
	m_prevPos = m_transform.pos;

	//入力処理
	Input(arg_camera, arg_bulletMgr);

	//当たり判定
	Collision(arg_stageMeshCollision);

	//重力をかける。
	if (!m_onGround) {
		m_gravity -= GRAVITY;
	}
	m_transform.pos.y += m_gravity;

	//動いた方向に回転させる。
	//Rotate(arg_camera);

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

	//アウトラインを出す中心点代入
	GBufferMgr::Instance()->m_outline->SetOutlineCenterPos(m_transform.pos);

	////エコーを描画
	//if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE)) {

	//	GBufferMgr::Instance()->m_outline->m_echoData.m_center = m_transform.pos;
	//	GBufferMgr::Instance()->m_outline->m_echoData.m_echoRadius = 0.0f;
	//	GBufferMgr::Instance()->m_outline->m_echoData.m_echoAlpha = 0.2f;

	//}
	//else if (KeyBoradInputManager::Instance()->InputState(DIK_SPACE)) {

	//	GBufferMgr::Instance()->m_outline->m_echoData.m_echoRadius += 8.0f;

	//}
	//else {

	//	GBufferMgr::Instance()->m_outline->m_echoData.m_echoAlpha = std::clamp(GBufferMgr::Instance()->m_outline->m_echoData.m_echoAlpha - 0.01f, 0.0f, 1.0f);

	//}

	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE)) {

		EchoArray::Instance()->Generate(m_transform.pos, 100.0f, KazMath::Vec3<float>(0.24f, 0.50f, 0.64f));

	}

	arg_throwableObjectController.lock()->InputHold(KeyBoradInputManager::Instance()->InputState(DIK_E));

}

void Player::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	//m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}

void Player::Input(std::weak_ptr<Camera> arg_camera, std::weak_ptr<BulletMgr> arg_bulletMgr)
{

	m_transform.quaternion = arg_camera.lock()->GetShotQuaternion().quaternion;

	//前方向と右方向のベクトルを取得
	KazMath::Vec3<float> frontVec = m_transform.GetFront();
	frontVec.y = 0;
	KazMath::Vec3<float> rightVec = m_transform.GetRight();
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

	//右クリックされている間はADS状態にする。
	m_isADS = KeyBoradInputManager::Instance()->MouseInputState(MOUSE_INPUT_RIGHT);

	//弾をうつ入力も受け付ける。
	if (KeyBoradInputManager::Instance()->MouseInputTrigger(MOUSE_INPUT_LEFT)) {

		arg_bulletMgr.lock()->Genrate(m_transform.pos, arg_camera.lock()->GetShotQuaternion().GetFront());

	}
	if (KeyBoradInputManager::Instance()->MouseInputTrigger(MOUSE_INPUT_RIGHT)) {

		arg_bulletMgr.lock()->Genrate(m_transform.pos, arg_camera.lock()->GetShotQuaternion().GetFront(), true);

	}

}

void Player::Rotate(std::weak_ptr<Camera> arg_camera)
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

	////ADSしていたらカメラと同じ方向を向く。
	//if (m_isADS) {

	//	m_transform.quaternion = arg_camera.lock()->GetCameraQuaternion().quaternion;

	//}

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
