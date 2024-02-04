#include "BGMController.h"

void BGMController::Setting()
{


	m_BGM[0] = BGMData("Resource/Sound/BGM/EmergencyBGM.wav");
	m_BGM[1] = BGMData("Resource/Sound/BGM/Kankyo_SE_2_.wav");
	m_BGM[2] = BGMData("Resource/Sound/BGM/Kankyo_SE_1_.wav");

	for (auto& index : m_BGM) {
		index.m_soundData.volume = 0.0f;
		SoundManager::Instance()->SoundPlayerWave(index.m_soundData, 100);
		index.m_soundData.source->Stop();
	}

	m_isTermination = false;

}

void BGMController::Update()
{

	const int BGM_COUNT = static_cast<size_t>(BGM::MAX);
	for (int index = 0; index < BGM_COUNT; ++index) {

		//���ݖ炵�Ă���BGM��������
		if (index == static_cast<int>(m_nowBGM) && !m_isTermination) {

			//���Ă��Ȃ�������炷�B
			if (!m_BGM[index].m_isActive) {
				m_BGM[index].m_soundData.source->Start();
			}

			//�{�����[�����f�t�H���g�ɋ߂Â���B
			m_BGM[index].m_soundData.source->GetVolume(&(m_BGM[index].m_soundData.volume));
			m_BGM[index].m_soundData.volume += (MAX_VOLUME - m_BGM[index].m_soundData.volume) / 10.0f;
			m_BGM[index].m_soundData.source->SetVolume(m_BGM[index].m_soundData.volume);

		}
		//���ݖ炵�Ă��Ȃ�BGM��������
		else {

			//�{�����[����0�ɋ߂Â���B
			m_BGM[index].m_soundData.volume += (0.0f - m_BGM[index].m_soundData.volume) / 10.0f;
			m_BGM[index].m_soundData.source->SetVolume(m_BGM[index].m_soundData.volume);

			//�{�����[��������Ȃ�0�ɋ߂Â��Ă��āA�܂����Ă�����B
			if (m_BGM[index].m_soundData.volume <= 0.005f && m_BGM[index].m_isActive) {

				m_BGM[index].m_soundData.source->Stop();
				m_BGM[index].m_isActive = false;

			}


		}

	}

}

void BGMController::ChangeBGM(BGM arg_bgmID)
{

	m_isTermination = false;
	m_nowBGM = arg_bgmID;

}

void BGMController::Termination()
{
	m_isTermination = true;
}

BGMController::BGMData::BGMData(char* arg_filePath)
{
	m_soundData = SoundManager::Instance()->SoundLoadWave(arg_filePath);
	m_isActive = false;
}
