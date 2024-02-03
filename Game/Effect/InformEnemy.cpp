#include "InformEnemy.h"
#include"../KazLibrary/Imgui/MyImgui.h"

InformEnemy::InformEnemy()
{
}

void InformEnemy::Load(DrawingByRasterize& arg_rasterize)
{
	m_circle.m_tex.Load(arg_rasterize, "Resource/Inform/Ring.png", true);

}

void InformEnemy::Update(const KazMath::Vec3<float>& arg_enemyPos, KazMath::Transform3D& arg_playerTransform)
{
	
	//�v���C���[����G�܂ł̃x�N�g��
	KazMath::Vec3<float> playerToEnemyVec = KazMath::Vec3<float>(arg_enemyPos - arg_playerTransform.pos).GetNormal();
	//�v���C���[�̐��ʃx�N�g��
	KazMath::Vec3<float> playerForwardVec = arg_playerTransform.GetFront();

	//�e�x�N�g����Y����0�ɂ���XZ���ʂɂ���B
	playerToEnemyVec.y = 0.0f;
	playerToEnemyVec.Normalize();
	playerForwardVec.y = 0.0f;
	playerForwardVec.Normalize();

	//���ʃx�N�g�������XZ���ʂł̊p�x�����߂�B
	m_xzAngle = acosf(playerForwardVec.Dot(playerToEnemyVec));

	//���ʃx�N�g����ŉE�������őΏƓI�ɓ������߁A�����ł͍����������ꍇ��-1.0f�������邱�ƂŐ������p�x�ɏC������B
	if (0.0f < playerForwardVec.Cross(playerToEnemyVec).y) {
		m_xzAngle *= -1.0f;
	}

	//��]��K�p�B
	m_transform.rotation = -DirectX::XMConvertToDegrees(m_xzAngle);

	//�ʒu�𒲐��B
	m_transform.pos = KazMath::Vec2<float>(1280.0f / 2.0f, 720.0f / 2.0f);
	KazMath::Vec2<float> screenPos = KazMath::Vec2<float>(sinf(m_xzAngle), cosf(m_xzAngle));
	screenPos.x *= -1.0f;
	screenPos.y *= -1.0f;
	m_transform.pos += screenPos * 200.0f;

}

void InformEnemy::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{

	ImGui::Begin("InformEnemy");
	ImGui::DragFloat("XZAngle : ", &m_transform.rotation);
	ImGui::End();

	if (!m_activeFlag)
	{
		return;
	}
	m_circle.m_tex.Draw2D(arg_rasterize, m_transform);

}
