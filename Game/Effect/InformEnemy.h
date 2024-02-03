#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include <cmath>

class InformEnemy
{
public:
	InformEnemy();
	void Load(DrawingByRasterize &arg_rasterize);
	void Update(const KazMath::Vec3<float> &arg_pos, KazMath::Transform3D &arg_playerPos);
	void Draw(DrawingByRasterize &arg_rasterize, Raytracing::BlasVector &arg_blas);

private:
	KazMath::Transform2D m_transform;
	BasicDraw::BasicTextureRender m_circle;

	KazMath::Transform2D m_screen;
	KazMath::Transform2D m_larpSreen;
	BasicDraw::BasicTextureRender m_screenTex;
	bool m_activeFlag;

	float m_cross;
	float m_radian;
	float m_lerpRadian;
	float m_offset;
	float m_angle;
	bool m_crossFlag, m_prevCrossFlag;
	bool m_radianFlag, m_prevRadianFlag;
	float m_angleOffset;
	KazMath::Vec3<float>m_playerPos, m_enemyPos, m_playerFrontVec;
	KazMath::Transform2D m_interBox, m_interBox2;
	BasicDraw::BasicTextureRender m_interBoxTex;


	std::vector<KazMath::Vec2<float>> GetCrossPoints(const KazMath::Vec2<float> &arg_startPos, const KazMath::Vec2<float> &arg_endPos, const KazMath::Vec2<float> &arg_circlePos, float arg_radius, float *arg_radian)
	{
		//ax + by + c = 0 の定数項
		float a = arg_endPos.y - arg_startPos.y;
		float b = arg_startPos.x - arg_endPos.x;
		float c = -a * arg_startPos.x - b * arg_startPos.y;

		//円の中心から直線までの距離
		//mag(a, b) = √a^2+b^2
		float d = abs((a * arg_circlePos.x + b * arg_circlePos.y + c) / std::sqrtf(std::pow(a, 2.0f) + std::pow(b, 2.0f)));

		//直線の垂線とX軸と平行な線がなす角度θ
		float theta = atan2(b, a);

		std::vector<KazMath::Vec2<float>>result;
		if (d > arg_radius)
		{
			return {};
		}
		else if (d == arg_radius)
		{
			//場合わけ
			if (a * arg_circlePos.x + b * arg_circlePos.y + c > 0)theta += DirectX::XM_PI;

			float crossX = arg_radius * cos(theta) + arg_circlePos.x;
			float crossY = arg_radius * sin(theta) + arg_circlePos.y;

			result.emplace_back(crossX, crossY);
			return result;
		}
		else
		{

			//alphaとbetaの角度を求める
			float alpha, beta, phi;
			phi = acos(d / arg_radius);
			alpha = theta - phi;
			beta = theta + phi;

			//場合わけ
			if (a * arg_circlePos.x + b * arg_circlePos.y + c > 0)
			{
				alpha += DirectX::XM_PI;
				beta += DirectX::XM_PI;
			}

			//交点の座標を求める
			result.emplace_back(arg_radius * cos(alpha) + arg_circlePos.x, arg_radius * sin(alpha) + arg_circlePos.y);
			result.emplace_back(arg_radius * cos(beta) + arg_circlePos.x, arg_radius * sin(beta) + arg_circlePos.y);
			*arg_radian = beta;
			return result;
		}
	};
};

