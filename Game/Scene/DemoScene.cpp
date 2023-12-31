#include "DemoScene.h"
#include"../Game/Input/Input.h"

DemoScene::DemoScene(DrawingByRasterize& arg_rasterize) :
	//DrawFuncHelperでのテクスチャ読み込み
	m_2DSprite(arg_rasterize, "Resource/Test/texas.png", true),
	m_3DSprite(arg_rasterize, "Resource/Test/texas.png", false),
	//DrawFuncHelperでのモデル読み込み
	m_modelAnimationRender(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf"),
	m_modelRender(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf")
{
	/*
	テクスチャやモデルの読み込みはTextureRenderやModelRenderのコンストラクタで読み込まれますが、
	読み込み単体の処理は下の処理になります。(多重読み込み防止あり)
	読み込みの成否はコンソールに出力されます
	*/
	std::shared_ptr<ModelInfomation> model = ModelLoader::Instance()->Load("Resource/Test/Virus/", "virus_cur.gltf");
	KazBufferHelper::BufferData texture = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/Test/texas.png");

	//アニメーション再生
	//m_modelAnimationRender.m_model.m_animator->Play("繧｢繝ｼ繝槭メ繝･繧｢Action", true, false);

	//3DスプライトのTrasform調整
	m_3DSpriteTransform.pos = { 10.0f,0.0f,0.0f };
	m_3DSpriteTransform.scale = { 0.1f,0.1f,1.0f };
	//アニメーション再生無しモデルの位置調整
	m_modelTransform.pos = { -10.0f,0.0f,0.0f };
	m_sceneNum = SCENE_NONE;
}

DemoScene::~DemoScene()
{
}

void DemoScene::Init()
{
	m_sceneNum = SCENE_NONE;
}

void DemoScene::PreInit()
{
}

void DemoScene::Finalize()
{
}

void DemoScene::Input()
{
	/*
	コントローラーやキーボードの入力を使った処理を実装する際はInputクラスから使ってください
	新しい操作の処理を実装する際はInputクラスに関数を追加して使用してください(書き換えは自由です)
	(操作変更を容易に行いやすくするためと処理の重複を無くす為にもご協力お願いします。)
	*/
	if (Input::Instance()->Done())
	{
		m_sceneNum = 0;
	}

}

void DemoScene::Update(DrawingByRasterize &arg_rasterize)
{
	/*
	カメラを使用する際は下の関数を使用し、eye, target, upの値を入れることで計算できます
	計算結果は描画情報に渡ります。
	CameraMgr::Instance()->Camera({}, {}, {});
	*/
	//デバック用のカメラワーク(操作はBlenderと同じ)
	m_camera.Update();
}

void DemoScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	//描画命令発行
	m_2DSprite.m_tex.Draw2D(arg_rasterize, m_2DSpriteTransform);
	m_3DSprite.m_tex.Draw3D(arg_rasterize, arg_blasVec, m_3DSpriteTransform);
	m_modelAnimationRender.m_model.Draw(arg_rasterize, arg_blasVec, m_modelAnimationTransform);
	m_modelRender.m_model.Draw(arg_rasterize, arg_blasVec, m_modelTransform);
}

int DemoScene::SceneChange()
{
	if (m_sceneNum != SCENE_NONE)
	{
		int tmp = m_sceneNum;
		m_sceneNum = SCENE_NONE;
		return tmp;
	}
	return SCENE_NONE;
}
