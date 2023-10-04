#include "Player.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"

Player::Player(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf")
{
	Init();
}

void Player::Init()
{
	
	m_playerAttitude = PlayerAttitude::STAND;

}

void Player::Update(KazMath::Transform3D arg_cameraQuaternion)
{

	//�������O�̍��W�B
	m_prevPos = m_transform.pos;

	//���͏���
	Input(arg_cameraQuaternion);

	//�����������ɉ�]������B
	Rotate();

}

void Player::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}

void Player::Input(KazMath::Transform3D arg_cameraQuaternion)
{

	//�O�����ƉE�����̃x�N�g�����擾�B -�����Ă���̂́Aarg_cameraQuaternion���v���C���[���猩���J�����̕���������B
	KazMath::Vec3<float> frontVec = -arg_cameraQuaternion.GetFront();
	frontVec.y = 0;
	KazMath::Vec3<float> rightVec = -arg_cameraQuaternion.GetRight();
	rightVec.y = 0;

	//�O�㍶�E�Ɉړ�����B
	KazMath::Vec3<float> inputMoveVec = {};
	if (KeyBoradInputManager::Instance()->InputState(DIK_W)) {
		inputMoveVec += frontVec * MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_S)) {
		inputMoveVec -= frontVec * MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_A)) {
		inputMoveVec -= rightVec * MOVE_SPEED;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_D)) {
		inputMoveVec += rightVec * MOVE_SPEED;
	}
	m_transform.pos += inputMoveVec.GetNormal() * MOVE_SPEED;

}

void Player::Rotate()
{

	KazMath::Vec3<float> movedVec = m_transform.pos - m_prevPos;
	if (0 < movedVec.Length()) {

		//�����������B
		movedVec.Normalize();

		//�f�t�H���g�̐��ʃx�N�g������̉�]�ʂ����߂�B
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(movedVec));

		//�N�H�[�^�j�I�������߂�B
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(movedVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//��]��K��
		m_transform.quaternion = DirectX::XMQuaternionSlerp(m_transform.quaternion, rotateQ, 0.15f);

	}

}
