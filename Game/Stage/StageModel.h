#pragma once
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/BasicDraw.h"

/// <summary>
/// �X�e�[�W�̑�g�̃p�[�c
/// </summary>
class StageModel
{
public:
	StageModel(DrawingByRasterize& arg_rasterize,const std::string& arg_fileDir, const std::string& arg_fileName);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

private:
	KazMath::Transform3D m_transform;
	BasicDraw::BasicModelRender m_stageModelRender;
};

