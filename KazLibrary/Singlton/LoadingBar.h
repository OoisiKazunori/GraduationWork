#pragma once
#include"Helper/ISinglton.h"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>

/// <summary>
/// ���[�h��ʂ̃Q�[�W�̐��l�Ǘ�
/// </summary>
class LoadingBar :public ISingleton<LoadingBar>
{
public:
	LoadingBar();
	void Init();
	void SetMaxBar(int arg_num);
	void IncreNowNum(int arg_num);
	float GetNowRate();

	bool IsDone();
private:
	int m_loadMaxBar;
	int m_loadNowBar;
	//�r������
	std::mutex m_mtx;
};

