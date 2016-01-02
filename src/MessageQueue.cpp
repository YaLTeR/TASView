#include "stdafx.hpp"
#include "MessageQueue.hpp"

using namespace std;

MessageQueue::MessageQueue(const string& name) : waiting_for_completion(false)
{
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (overlapped.hEvent == NULL) {
		throw runtime_error("error creating an event for overlapped: " + to_string(GetLastError()));
	}

	pipe = CreateFile(("\\\\.\\pipe\\" + name).c_str(), GENERIC_READ | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (pipe == INVALID_HANDLE_VALUE) {
		CloseHandle(overlapped.hEvent);
		throw runtime_error("error opening the pipe: " + to_string(GetLastError()));
	}

	DWORD mode = PIPE_READMODE_MESSAGE;
	if (!SetNamedPipeHandleState(pipe, &mode, NULL, NULL)) {
		CloseHandle(pipe);
		CloseHandle(overlapped.hEvent);
		throw runtime_error("error setting the pipe state: " + to_string(GetLastError()));
	}
}

MessageQueue::~MessageQueue()
{
	CloseHandle(pipe);
	CloseHandle(overlapped.hEvent);
}
