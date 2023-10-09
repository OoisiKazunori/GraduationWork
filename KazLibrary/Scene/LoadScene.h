#pragma once
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include "../KazLibrary/Raytracing/RayPipeline.h"
#include "../KazLibrary/Render/BasicDraw.h"

/// <summary>
/// �}���`�X���b�h�����̃��[�h��ʂ̕`��
/// </summary>
class LoadScene
{
public:
	LoadScene();
	void Update();
	void Draw();

private:

	//���X�^���C�U�`��
	DrawingByRasterize m_rasterize;

	BasicDraw::BasicTextureRender m_bar;
};

