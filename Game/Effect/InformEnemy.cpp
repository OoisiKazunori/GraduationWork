#include "InformEnemy.h"
#include"../KazLibrary/Imgui/MyImgui.h"

InformEnemy::InformEnemy()
{
}

void InformEnemy::Load(DrawingByRasterize &arg_rasterize)
{
	m_circle.m_tex.Load(arg_rasterize, "Resource/Inform/Ring.png", true);
	m_screenTex.m_tex.Load(arg_rasterize, "Resource/Test/white1x1.png", true);
	m_interBoxTex.m_tex.Load(arg_rasterize, "Resource/Test/white1x1.png", true);

	m_screen.scale = { 50.0f,50.0f };
	m_interBox.scale = { 50.0f,50.0f };
	m_larpSreen.scale = { 50.0f,50.0f };
}

void InformEnemy::Update(const KazMath::Vec3<float> &arg_pos, KazMath::Transform3D &arg_playerPos)
{
	m_playerPos = arg_playerPos.pos;
	m_enemyPos = arg_pos;
	if (100.0f <= arg_pos.Distance(arg_playerPos.pos))
	{
		m_activeFlag = false;
		return;
	}
	else
	{
		m_activeFlag = true;
	}


	m_crossFlag = m_cross <= 0.0f;
	if (m_crossFlag != m_prevCrossFlag)
	{
		m_offset = m_radian;
	}
	m_prevCrossFlag = m_crossFlag;



	//ìGÇÃÇ¢ÇÈï˚å¸Çì‡êœéÊÇ¡ÇƒäpìxÇìæÇÈ
	if (m_crossFlag)
	{
		//float dotNum = acos(pZVec2.Dot(peVec2));
		//m_radian = m_offset + (m_offset - dotNum);
	}


	const KazMath::Vec2<float>centralPos(1280.0f / 2, 720.0f / 2.0f);
	//ìGÇÃà íu
	KazMath::Vec3<float> screenPos = KazMath::ConvertWorldPosToScreenPos(
		arg_pos,
		CameraMgr::Instance()->GetViewMatrix(),
		CameraMgr::Instance()->GetPerspectiveMatProjection()
	);

	const float radius = 300.0f;
	//âÊñ ì‡ÇÃèàóù--------------------------------
	if (screenPos.z < 1.0f)
	{
		m_screen.pos = screenPos.ConvertVec2();
		m_screen.pos.x = std::clamp(m_screen.pos.x, 0.0f, 1280.0f);
		m_screen.pos.y = std::clamp(m_screen.pos.y, 0.0f, 720.0f);

		//âÊñ íÜêS-enemy
		KazMath::Vec2<float>c_ePos(m_screen.pos - centralPos);
		c_ePos.Normalize();

		std::vector<KazMath::Vec2<float>>posArray = GetCrossPoints(centralPos, centralPos + c_ePos * 400.0f, centralPos, radius, &m_radian);
		if (posArray.size() != 0)
		{
			m_interBox.pos = posArray[1];
			m_transform.pos = { 1280.0f / 2.0f + cosf(m_radian) * radius,720.0f / 2.0f + sinf(m_radian) * radius };
		}
		m_transform.rotation = (float)KazMath::RadianToAngle(m_radian) + 90.0f;
	}
	//âÊñ ì‡ÇÃèàóù--------------------------------
	//âÊñ äOÇÃèàóù--------------------------------
	else
	{
		KazMath::Vec3<float>playerEenemyVec(arg_playerPos.pos - arg_pos);
		playerEenemyVec.Normalize();
		KazMath::Vec3<float>playerZVec(arg_playerPos.GetFront());
		playerZVec.y = 0.0f;
		m_playerFrontVec = playerZVec;

		KazMath::Vec2<float>peVec2(playerEenemyVec.x, playerEenemyVec.z);
		KazMath::Vec2<float>pZVec2(playerZVec.x, playerZVec.z);

		m_radian = atan(playerZVec.Dot(playerEenemyVec));
		m_interBox.pos = { centralPos.x + cosf(m_radian) * radius,centralPos.y + sinf(m_radian) * radius };
		m_transform.pos = { 1280.0f / 2.0f + cosf(m_radian) * radius,720.0f / 2.0f + sinf(m_radian) * radius };
		m_transform.rotation = (float)KazMath::RadianToAngle(m_radian) + 90.0f;

		//std::vector<KazMath::Vec2<float>>posArray = GetCrossPoints(centralPos, centralPos + peVec2 * 400.0f, centralPos, radius, &m_radian);
		//if (posArray.size() != 0)
		//{
		//	m_interBox.pos = posArray[0];
		//	m_transform.pos = { 1280.0f / 2.0f + cosf(m_radian) * radius,720.0f / 2.0f + sinf(m_radian) * radius };
		//}
		m_transform.rotation = (float)KazMath::RadianToAngle(m_radian) + 90.0f;

	}
	//âÊñ äOÇÃèàóù--------------------------------
}

void InformEnemy::Draw(DrawingByRasterize &arg_rasterize, Raytracing::BlasVector &arg_blas)
{
	ImGui::Begin("InformEnemy");
	ImGui::Text("cross:%f", m_cross);
	ImGui::Text("angle:%d", KazMath::RadianToAngle(m_radian));
	ImGui::DragFloat("Angle", &m_angle);
	ImGui::End();

	if (!m_activeFlag)
	{
		return;
	}
	m_circle.m_tex.Draw2D(arg_rasterize, m_transform);
	m_screenTex.m_tex.Draw2D(arg_rasterize, m_screen);

	m_interBoxTex.m_tex.Draw2D(arg_rasterize, m_interBox, KazMath::Color(255, 0, 0, 255));

}
