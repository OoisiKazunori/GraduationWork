#pragma once
#include <Render/BasicDraw.h>
#include "../KazLibrary/Math/KazMath.h"

class MeshCollision;

class Bullet {

private:

	KazMath::Vec3<float> m_collisionStartPos;	//�e���ˏo�����n�_�B
	KazMath::Vec3<float> m_collisionEndPos;		//�����蔻��̌��ʂ̏I�_�B������Ȃ�������߂����ቓ���ɁB
	KazMath::Vec3<float> m_dir;			//�e���ˏo��������B
	KazMath::Vec3<float> m_bulletPos;
	bool m_isActive;					//�L��������Ă��邩�̃t���O
	bool m_isCollision;					//�����蔻����s�����Ƃ��ł��邩�H�̃t���O�B�y�ʉ��ň�񂵂������蔻����s��Ȃ����߂Ɏg�p����B
	BasicDraw::BasicLineRender m_line;

	//������܂ł̃^�C�}�[
	int m_disappearTimer;
	const int DISAPPEAR_TIMER = 120;

	const float BULLET_SPEED = 20.0f;	//�`��ɂ̂ݎg�p����e�̈ړ����x�B�����蔻��͎ˏo�n�_����߂����ቓ���Ƀ��C���΂��`���Ȃ̂ŁA����͎g��Ȃ��B
	const float BULLET_LENGTH = 20.0f;	//�e�̑傫���B�`��ł̂ݎg�p����B

public:

	Bullet(DrawingByRasterize& arg_rasterize);
	void Init();
	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir);
	void Update(std::list<std::shared_ptr<MeshCollision>> arg_stageColliders);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; };

};