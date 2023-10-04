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

void Player::Update()
{

	//“ü—Íˆ—
	Input();

}

void Player::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}

void Player::Input()
{

	//‘OŒã¶‰E‚ÉˆÚ“®‚·‚éB
	if (KeyBoradInputManager::Instance()->InputState(DIK_W)) {
		m_transform.pos.z += MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_S)) {
		m_transform.pos.z -= MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_A)) {
		m_transform.pos.x -= MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_D)) {
		m_transform.pos.x += MOVE_SPEED;
	}

}
