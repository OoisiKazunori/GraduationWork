#include "DemoScene.h"
#include"../Game/Input/Input.h"

DemoScene::DemoScene(DrawingByRasterize& arg_rasterize) :
	//DrawFuncHelper�ł̃e�N�X�`���ǂݍ���
	m_2DSprite(arg_rasterize, "Resource/Test/texas.png", true),
	m_3DSprite(arg_rasterize, "Resource/Test/texas.png", false),
	//DrawFuncHelper�ł̃��f���ǂݍ���
	m_modelAnimationRender(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf"),
	m_modelRender(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf")
{
	/*
	�e�N�X�`���⃂�f���̓ǂݍ��݂�TextureRender��ModelRender�̃R���X�g���N�^�œǂݍ��܂�܂����A
	�ǂݍ��ݒP�̂̏����͉��̏����ɂȂ�܂��B(���d�ǂݍ��ݖh�~����)
	�ǂݍ��݂̐��ۂ̓R���\�[���ɏo�͂���܂�
	*/
	std::shared_ptr<ModelInfomation> model = ModelLoader::Instance()->Load("Resource/Test/Virus/", "virus_cur.gltf");
	KazBufferHelper::BufferData texture = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/Test/texas.png");

	//�A�j���[�V�����Đ�
	//m_modelAnimationRender.m_model.m_animator->Play("アーマチュアAction", true, false);

	//3D�X�v���C�g��Trasform����
	m_3DSpriteTransform.pos = { 10.0f,0.0f,0.0f };
	m_3DSpriteTransform.scale = { 0.1f,0.1f,1.0f };
	//�A�j���[�V�����Đ��������f���̈ʒu����
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
	�R���g���[���[��L�[�{�[�h�̓��͂��g������������������ۂ�Input�N���X����g���Ă�������
	�V��������̏�������������ۂ�Input�N���X�Ɋ֐���ǉ����Ďg�p���Ă�������(���������͎��R�ł�)
	(����ύX��e�Ղɍs���₷�����邽�߂Ə����̏d���𖳂����ׂɂ������͂��肢���܂��B)
	*/
	if (Input::Instance()->Done())
	{
		m_sceneNum = 0;
	}
}

void DemoScene::Update()
{
	/*
	�J�������g�p����ۂ͉��̊֐����g�p���Aeye, target, up�̒l�����邱�ƂŌv�Z�ł��܂�
	�v�Z���ʂ͕`����ɓn��܂��B
	CameraMgr::Instance()->Camera({}, {}, {});
	*/
	//�f�o�b�N�p�̃J�������[�N(�����Blender�Ɠ���)
	m_camera.Update();
}

void DemoScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	//�`�施�ߔ��s
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
