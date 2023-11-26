#pragma once
#include <Render/BasicDraw.h>
#include "../KazLibrary/Math/KazMath.h"

class MeshCollision;

class Bullet {

private:

	KazMath::Vec3<float> m_startPos;	//弾を射出した地点。
	KazMath::Vec3<float> m_dir;			//弾を射出する方向。
	KazMath::Vec3<float> m_endPos;		//当たり判定の結果の終点。当たらなかったらめっちゃ遠くに。
	bool m_isActive;					//有効化されているかのフラグ
	bool m_isCollision;					//当たり判定を行うことができるか？のフラグ。軽量化で一回しか当たり判定を行わないために使用する。
	BasicDraw::BasicLineRender m_line;

	//消えるまでのタイマー
	int m_disappearTimer;
	const int DISAPPEAR_TIMER = 10;

public:

	Bullet(DrawingByRasterize& arg_rasterize);
	void Init();
	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir);
	void Update(std::list<std::shared_ptr<MeshCollision>> arg_stageColliders);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; };

};