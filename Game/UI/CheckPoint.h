#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class CheckPoint
{
public:
	CheckPoint(DrawingByRasterize& arg_rasterize);

	void Init(const KazMath::Vec3<float>& arg_pointWorldPos);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);

	void CalucurateDistance(const KazMath::Vec3<float>& arg_playerPos);
private:
	KazMath::Vec3<float>m_screenPos, m_worldPos;
	BasicDraw::BasicTextureRender m_render;

	//c‚è‹——£‚Ì”’l‰»
	float m_distance;
	std::vector<int>m_digit;
	std::array<BasicDraw::BasicTextureRender, 10>m_distanceRender;
	std::array<KazBufferHelper::BufferData, 10>m_digitTexture;
	BasicDraw::BasicTextureRender m_meterRender;

};

