#pragma once

#include "Application.hpp"

void MessageThread(const Application *app, std::timed_mutex &m);
