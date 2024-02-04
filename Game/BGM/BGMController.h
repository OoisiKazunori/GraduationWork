#pragma once
#include "../KazLibrary/Helper/ISinglton.h"
#include "../KazLibrary/Sound/SoundManager.h"
#include <array>

class BGMController : public ISingleton<BGMController> {

public:

	enum class BGM {

		OUTSIDE,	//室外
		INDOOR,		//室内
		EMERGENCY,	//発見時BGM
		MAX,

	};

	struct BGMData {
		SoundData m_soundData;
		bool m_isActive;
		BGMData(char* arg_filePath);
		BGMData() {};
	};

private:

	BGM m_nowBGM;	//現在再生中のBGM
	std::array<BGMData, static_cast<size_t>(BGM::MAX)> m_BGM;	//BGM配列

	bool m_isTermination;

	const float MAX_VOLUME = 0.05f;


public:

	void Setting();
	void Update();

	void ChangeBGM(BGM arg_bgmID);
	//BGM終了 全てのBGMを緩やかに終わらせる。
	void Termination();


};