#pragma once
#include <Render/BasicDraw.h>
#include "../KazLibrary/Math/KazMath.h"

class MeshCollision;

class EchoBullet {

private:

	KazMath::Vec3<float> m_dir;			//�e���ˏo��������B
	bool m_isActive;					//�L��������Ă��邩�̃t���O
	bool m_isCollision;					//�����蔻����s�����Ƃ��ł��邩�H�̃t���O�B�y�ʉ��ň�񂵂������蔻����s��Ȃ����߂Ɏg�p����B
	BasicDraw::BasicModelRender m_model;
	KazMath::Transform3D m_transform;		//���f���̕`��Ɏg�p����g�����X�t�H�[�����
	const float BULLET_SPEED = 20.0f;

	int m_echoCount;
	const int ECHO_COUNT = 3;
	float m_echoSpan;
	const float ECHO_SPAN = 120.0f;
	int m_disappearTimer;
	const int DESAPPEAR_TIMER = 60;

public:

	EchoBullet(DrawingByRasterize& arg_rasterize);
	void Init();
	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir);
	void Update(std::weak_ptr<MeshCollision> arg_meshCollision);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; };

};