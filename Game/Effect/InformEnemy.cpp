#include "InformEnemy.h"
#include "../KazLibrary/Imgui/MyImgui.h"
#include "../KazLibrary/Easing/easing.h"

InformEnemy::InformEnemy()
{
}

void InformEnemy::Init()
{
	m_alpha = 0;
}

void InformEnemy::Load(DrawingByRasterize& arg_rasterize)
{
	m_circle.m_tex.Load(arg_rasterize, "Resource/Inform/Ring.png", true);

}

void InformEnemy::Update(const KazMath::Vec3<float>& arg_enemyPos, KazMath::Transform3D& arg_playerTransform, bool arg_inSight)
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
	float xzAngle = acosf(playerForwardVec.Dot(playerToEnemyVec));

	//���ʃx�N�g����ŉE�������őΏƓI�ɓ������߁A�����ł͍����������ꍇ��-1.0f�������邱�ƂŐ������p�x�ɏC������B
	if (0.0f < playerForwardVec.Cross(playerToEnemyVec).y) {
		xzAngle *= -1.0f;
	}

	//��]��K�p�B
	m_transform.rotation = -DirectX::XMConvertToDegrees(xzAngle);

	//�ʒu�𒲐��B
	m_transform.pos = KazMath::Vec2<float>(1280.0f / 2.0f, 720.0f / 2.0f);
	KazMath::Vec2<float> screenPos = KazMath::Vec2<float>(sinf(xzAngle), cosf(xzAngle));
	screenPos.x *= -1.0f;
	screenPos.y *= -1.0f;
	m_transform.pos += screenPos * 300.0f;

	//�����ɉ����ăA���t�@��ς���B
	float distanceRate = KazMath::Vec3<float>(arg_enemyPos - arg_playerTransform.pos).Length();
	distanceRate = std::clamp(distanceRate / ALPHA_RANGE, 0.0f, 1.0f);
	float alpha = 1.0f - EasingMaker(In, Cubic, distanceRate);

	//���E���ɓ����Ă���̂ł���ΓK�p����B
	if (arg_inSight) {
		m_alpha += (alpha - m_alpha) / 3.0f;
	}
	else {
		m_alpha += (0.0f - m_alpha) / 3.0f;
	}

}

void InformEnemy::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{

	if (m_alpha <= 0.01f) return;

	m_circle.m_tex.Draw2D(arg_rasterize, m_transform, KazMath::Color(255, 255, 255, static_cast<int>(255 * m_alpha)));

}
