#pragma once
#include "../KazLibrary/Helper/ISinglton.h"

class PlayerStatus : public ISingleton<PlayerStatus> {

public:

	bool m_isFound;

};