#include "DemoScene.h"
#include"../Game/Input/Input.h"

DemoScene::DemoScene(DrawingByRasterize& arg_rasterize) :
	//DrawFuncHelper�ł̃e�N�X�`���ǂݍ���
	m_2DSprite("Resource/Test/texas.png"),
	m_3DSprite("Resource/Test/texas.png"),
	//DrawFuncHelper�ł̃��f���ǂݍ���
	m_modelAnimationRender("Resource/Test/Virus/", "virus_cur.gltf"),
	m_modelRender("Resource/Test/Virus/", "virus_cur.gltf")
{

	/*
	�e�N�X�`���⃂�f���̓ǂݍ��݂�TextureRender��ModelRender�̃R���X�g���N�^�œǂݍ��܂�܂����A
	�ǂݍ��ݒP�̂̏����͉��̏����ɂȂ�܂��B(���d�ǂݍ��ݖh�~����)
	�ǂݍ��݂̐��ۂ̓R���\�[���ɏo�͂���܂�
	*/
	std::shared_ptr<ModelInfomation> model = ModelLoader::Instance()->Load("Resource/Test/Virus/", "virus_cur.gltf");
	KazBufferHelper::BufferData texture = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/Test/texas.png");

	//�A�j���[�V�����Đ�
	m_modelAnimationRender.m_model.m_animator->Play("アーマチュアAction", true, false);

	//�����Đ�
	//SoundManager::Instance()->SoundPlayerWave(SoundDataManager::GetSoundName(SoundName::BGM_TITLE_SCENE), true);

	//�O���b�h�̕`��̏�񐶐�--------------------------
	for (int z = 0; z < m_gridCallDataX.size(); ++z)
	{
		std::vector<KazMath::Vec3<float>>posArray(2);
		VertexGenerateData bufferData(posArray.data(), sizeof(DirectX::XMFLOAT3), posArray.size(), sizeof(DirectX::XMFLOAT3));
		m_gridCallDataX[z] = DrawFuncData::SetLine(VertexBufferMgr::Instance()->GenerateBuffer(bufferData, false));
	}
	for (int y = 0; y < m_gridCallDataZ.size(); ++y)
	{
		std::vector<KazMath::Vec3<float>>posArray(2);
		VertexGenerateData bufferData(posArray.data(), sizeof(DirectX::XMFLOAT3), posArray.size(), sizeof(DirectX::XMFLOAT3));
		m_gridCallDataZ[y] = DrawFuncData::SetLine(VertexBufferMgr::Instance()->GenerateBuffer(bufferData, false));
	}

	//3D�X�v���C�g��Trasform����
	m_3DSpriteTransform.pos = { 10.0f,0.0f,0.0f };
	m_3DSpriteTransform.scale = { 0.1f,0.1f,1.0f };
	//�A�j���[�V�����Đ��������f���̈ʒu����
	m_modelTransform.pos = { -10.0f,0.0f,0.0f };
}

DemoScene::~DemoScene()
{
}

void DemoScene::Init()
{
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
		//�����Đ�
		//SoundManager::Instance()->SoundPlayerWave(SoundDataManager::GetSoundName(SoundName::SE_PLAYER_DASH), false);
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

	////���̏�����DrawFuncHelper�����ŕ`�悵��
	////�O���b�h�̕`��--------------------------

	//const float height = -5.0f;
	//const float range = 50.0f;
	//const KazMath::Color lineColor(49, 187, 134, 255);

	////���̐�����ׂ�
	//for (int z = 0; z < m_gridCallDataX.size(); ++z)
	//{
	//	float zLine = static_cast<float>(z) * 10.0f - (range);
	//	KazMath::Vec3<float>startPos(-range, height, zLine), endPos(range, height, zLine);
	//	std::vector<KazMath::Vec3<float>>posArray;
	//	posArray.emplace_back(startPos);
	//	posArray.emplace_back(endPos);
	//	//DrawLine�̕`�����ݒ肷��
	//	DrawFunc::DrawLine(m_gridCallDataZ[z], posArray, m_gridCallDataZ[z].m_modelVertDataHandle, lineColor);
	//	//�`�施�ߔ��s
	//	arg_rasterize.ObjectRender(m_gridCallDataZ[z]);
	//}
	////�c�̐�����ׂ�
	//for (int x = 0; x < m_gridCallDataZ.size(); ++x)
	//{
	//	float xLine = static_cast<float>(x) * 10.0f - (range);
	//	KazMath::Vec3<float>startPos(xLine, height, -range), endPos(xLine, height, range);

	//	std::vector<KazMath::Vec3<float>>posArray;
	//	posArray.emplace_back(startPos);
	//	posArray.emplace_back(endPos);
	//	//DrawLine�̕`�����ݒ肷��
	//	DrawFunc::DrawLine(m_gridCallDataX[x], posArray, m_gridCallDataX[x].m_modelVertDataHandle, lineColor);
	//	//�`�施�ߔ��s
	//	arg_rasterize.ObjectRender(m_gridCallDataX[x]);
	//}
}

int DemoScene::SceneChange()
{
	return SCENE_NONE;
}
