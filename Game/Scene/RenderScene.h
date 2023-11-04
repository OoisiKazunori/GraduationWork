#pragma once
#include"Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../KazLibrary/Render/BasicDraw.h"

class RenderScene :public SceneBase
{
public:
	RenderScene(DrawingByRasterize& arg_rasterize);
	~RenderScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:
	//�J����--------------------------
	DebugCamera m_camera;

	//���f���̕`��--------------------------
	BasicDraw::BasicModelRender m_sponzaModelRender;//�X�|���U�`��
	KazMath::Transform3D m_sponzaModelTransform;

	std::array<std::array<BasicDraw::BasicModelRender, 4>, 10>m_modelDrawArray;
	BasicDraw::BasicModelRender m_modelInstanceRender;


	//G-Buffer�̕`��
	enum GBufferTexEnum
	{
		GBUFFER_ALBEDO,
		GBUFFER_NORMAL,
		GBUFFER_FINAL,
		GBUFFER_MAX
	};
	int m_gBufferType;
	KazMath::Transform2D m_renderTransform;
	std::array<BasicDraw::BasicTextureRender, GBUFFER_MAX>m_gBufferRender;//GBuffer�ɏ������܂ꂽ�e�N�X�`���̕`��

	int m_sceneNum;
};

