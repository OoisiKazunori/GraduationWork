#include "Player.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"

Player::Player(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf")
{
}

void Player::Init()
{
}

void Player::Update(KazMath::Transform3D arg_cameraQuaternion)
{

	//入力処理
	Input(arg_cameraQuaternion);



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
	if (KeyBoradInputManager::Instance()->InputState(DIK_W)) {
		m_transform.pos += frontVec * MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_S)) {
		m_transform.pos -= frontVec * MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_A)) {
		m_transform.pos -= rightVec * MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_D)) {
		m_transform.pos += rightVec * MOVE_SPEED;
	}

}
