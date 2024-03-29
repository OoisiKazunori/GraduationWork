#pragma once
#include <Render/BasicDraw.h>
#include "../KazLibrary/Math/KazMath.h"

class MeshCollision;

class Bullet {

private:

	KazMath::Vec3<float> m_collisionStartPos;	//弾を射出した地点。
	KazMath::Vec3<float> m_collisionEndPos;		//当たり判定の結果の終点。当たらなかったらめっちゃ遠くに。
	KazMath::Vec3<float> m_dir;			//弾を射出する方向。
	KazMath::Vec3<float> m_bulletPos;
	bool m_isActive;					//有効化されているかのフラグ
	BasicDraw::BasicLineRender m_line;

	bool m_isEnemyBullet;

	//消えるまでのタイマー
	int m_disappearTimer;
	const int DISAPPEAR_TIMER = 120;

	const float BULLET_SPEED = 20.0f;	//描画にのみ使用する弾の移動速度。当たり判定は射出地点からめっちゃ遠くにレイを飛ばす形式なので、これは使わない。
	const float BULLET_LENGTH = 20.0f;	//弾の大きさ。描画でのみ使用する。

public:

	Bullet(DrawingByRasterize& arg_rasterize);
	void Init();
	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir, bool arg_isEnemyBullet = false);
	void Update(std::list<std::shared_ptr<MeshCollision>> arg_stageColliders);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; };

	bool CheckMeshCollision(std::weak_ptr<MeshCollision> arg_meshCollision, bool arg_isEnemyObject);

};