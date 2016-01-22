#pragma once

#include "Application.hpp"

namespace IPC
{
	void StartReceivingMessages(Application *app);
	void StopReceivingMessages();
}
