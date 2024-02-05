#pragma once
#include "../KazLibrary/Helper/ISinglton.h"
#include "../KazLibrary/Sound/SoundManager.h"
#include <array>

class BGMController : public ISingleton<BGMController> {

public:

	enum class BGM {

		OUTSIDE,	//���O
		INDOOR,		//����
		EMERGENCY,	//������BGM
		MAX,

	};

	struct BGMData {
		SoundData m_soundData;
		bool m_isActive;
		BGMData(char* arg_filePath);
		BGMData() {};
	};

private:

	BGM m_nowBGM;	//���ݍĐ�����BGM
	std::array<BGMData, static_cast<size_t>(BGM::MAX)> m_BGM;	//BGM�z��

	bool m_isTermination;

	const float MAX_VOLUME = 0.05f;


public:

	void Setting();
	void Update();

	void ChangeBGM(BGM arg_bgmID);
	//BGM�I�� �S�Ă�BGM���ɂ₩�ɏI��点��B
	void Termination();


};