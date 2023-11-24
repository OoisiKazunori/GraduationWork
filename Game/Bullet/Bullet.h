#pragma once
#include <Render/BasicDraw.h>
#include "../KazLibrary/Math/KazMath.h"

class MeshCollision;

class Bullet {

private:

	KazMath::Vec3<float> m_startPos;	//�e���ˏo�����n�_�B
	KazMath::Vec3<float> m_dir;			//�e���ˏo��������B
	KazMath::Vec3<float> m_endPos;		//�����蔻��̌��ʂ̏I�_�B������Ȃ�������߂����ቓ���ɁB
	bool m_isActive;					//�L��������Ă��邩�̃t���O
	bool m_isCollision;					//�����蔻����s�����Ƃ��ł��邩�H�̃t���O�B�y�ʉ��ň�񂵂������蔻����s��Ȃ����߂Ɏg�p����B
	std::array<BasicDraw::BasicLineRender, 1> m_line;

	//������܂ł̃^�C�}�[
	int m_disappearTimer;
	const int DISAPPEAR_TIMER = 30;

public:

	Bullet(DrawingByRasterize& arg_rasterize);
	void Init();
	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir);
	void Update(std::weak_ptr<MeshCollision> arg_meshCollision);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; };

};