#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//使用するモデル
	KazMath::Transform3D m_transform;		//モデルの描画に使用するトランスフォーム情報

	//移動速度
	const float MOVE_SPEED = 0.5f;

public:

	Player(DrawingByRasterize& arg_rasterize);

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

private:

	void Input();

};