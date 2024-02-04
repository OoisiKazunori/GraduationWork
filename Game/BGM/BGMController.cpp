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

		//現在鳴らしているBGMだったら
		if (index == static_cast<int>(m_nowBGM) && !m_isTermination) {

			//鳴っていなかったら鳴らす。
			if (!m_BGM[index].m_isActive) {
				m_BGM[index].m_soundData.source->Start();
			}

			//ボリュームをデフォルトに近づける。
			m_BGM[index].m_soundData.source->GetVolume(&(m_BGM[index].m_soundData.volume));
			m_BGM[index].m_soundData.volume += (MAX_VOLUME - m_BGM[index].m_soundData.volume) / 10.0f;
			m_BGM[index].m_soundData.source->SetVolume(m_BGM[index].m_soundData.volume);

		}
		//現在鳴らしていないBGMだったら
		else {

			//ボリュームを0に近づける。
			m_BGM[index].m_soundData.volume += (0.0f - m_BGM[index].m_soundData.volume) / 10.0f;
			m_BGM[index].m_soundData.source->SetVolume(m_BGM[index].m_soundData.volume);

			//ボリュームが限りなく0に近づいていて、まだ鳴っていたら。
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
