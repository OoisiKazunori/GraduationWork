#include "LoadScene.h"
#include"../KazLibrary/Singlton/LoadingBar.h"

LoadScene::LoadScene() :m_bar(m_rasterize, "Resource/")
{
	//�`���񐶐�
	//�o�[

	//�`���񐶐�
	m_rasterize.GeneratePipeline();
}

void LoadScene::Update()
{
	/*LoadingBar::Instance()->GetNowRate();
	m_blasVector.Update();*/
}

void LoadScene::Draw()
{
	//���[�h�p�̉�ʕ`��
	//m_bar.m_tex.Draw2D(m_rasterize, KazMath::Transform2D());
	//�Q�[�W�`��

	//���X�^���C�U�`��
	m_rasterize.SortAndRender();
	m_rasterize.UISortAndRender();

	//�����_�[�^�[�Q�b�g�̕�����
	//DirectX��GPU����
}
