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
	
	//プレイヤーから敵までのベクトル
	KazMath::Vec3<float> playerToEnemyVec = KazMath::Vec3<float>(arg_enemyPos - arg_playerTransform.pos).GetNormal();
	//プレイヤーの正面ベクトル
	KazMath::Vec3<float> playerForwardVec = arg_playerTransform.GetFront();

	//各ベクトルのY軸を0にしてXZ平面にする。
	playerToEnemyVec.y = 0.0f;
	playerToEnemyVec.Normalize();
	playerForwardVec.y = 0.0f;
	playerForwardVec.Normalize();

	//正面ベクトルを基準にXZ平面での角度を求める。
	m_xzAngle = acosf(playerForwardVec.Dot(playerToEnemyVec));

	//正面ベクトル基準で右か左かで対照的に動くため、ここでは左側だった場合に-1.0fを書けることで正しい角度に修正する。
	if (0.0f < playerForwardVec.Cross(playerToEnemyVec).y) {
		m_xzAngle *= -1.0f;
	}

	//回転を適用。
	m_transform.rotation = -DirectX::XMConvertToDegrees(m_xzAngle);

	//位置を調整。
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
