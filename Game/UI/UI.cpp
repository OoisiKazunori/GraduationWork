#include "UI.h"

UI2DElement::UI2DElement(DrawingByRasterize& arg_rasterize, const char* f_filePath):
m_2DSprite(arg_rasterize, f_filePath, true)
{
	m_nowPos = { WeponUIManager::GetUIBasePos().x, WeponUIManager::GetUIBasePos().y };
	m_nowScale = { 1.0f, 1.0f };
}

void UI2DElement::Init(DrawingByRasterize& arg_rasterize, std::string f_filePath)
{
	
}

void UI2DElement::Update()
{
}

void UI2DElement::Draw(DrawingByRasterize& arg_rasterize)
{
	KazMath::Transform2D l_trans = KazMath::Transform2D(m_nowPos, m_nowScale);

	m_2DSprite.m_tex.Draw2D(arg_rasterize, l_trans);
}

void UI2DElement::EasePosInit(KazMath::Vec2<float> f_easeEnd)
{
	m_easePosTimer = 0.0f;

	m_easePosEnd = f_easeEnd;
	m_easePosStart = m_nowPos;
}

WeponUIManager::WeponUIManager(DrawingByRasterize& arg_rasterize): 
m_hundgun(arg_rasterize, "Resource/UITexture/UI_handGun.png"),
m_knife(arg_rasterize, "Resource/UITexture/UI_Knife.png")
{
	m_nowWepon = e_Knife;
}

void WeponUIManager::Init(DrawingByRasterize& arg_rasterize)
{
	
}

void WeponUIManager::Update()
{
}

void WeponUIManager::Draw(DrawingByRasterize& arg_rasterize)
{
	m_knife.Draw(arg_rasterize);
	m_hundgun.Draw(arg_rasterize);
}
