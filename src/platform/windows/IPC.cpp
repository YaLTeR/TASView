#include "../../stdafx.hpp"
#include "MessageQueue.hpp"
#include "../../IPC.hpp"

using namespace std;
using namespace std::literals;

namespace IPC
{
	thread messageThread;
	HANDLE stopRunning;

	static void MessageThread(Application *app) {
		while (true) {
			// Quit if we were requested to.
			if (WaitForSingleObject(stopRunning, 0) != WAIT_TIMEOUT)
				return;

			try {
				MessageQueue mq("BunnymodXT-TASView");
				vector<char> buf(256);
				DWORD bytesRead;

				while (true) {
					if (mq.timed_read(buf, 1s, bytesRead, stopRunning))
						app->ParseMessage(buf, bytesRead);

					// Quit if we were requested to.
					if (WaitForSingleObject(stopRunning, 0) != WAIT_TIMEOUT)
						return;
				}
			} catch (const exception& ex) {
				// Wait 1s before trying again. Quit if we were requested to at any point during this waiting.
				cerr << "Exception: " << ex.what() << "\n";
				if (WaitForSingleObject(stopRunning, 1000) != WAIT_TIMEOUT)
					return;
			}
		}
	}

	void StartReceivingMessages(Application *app)
	{
		stopRunning = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (!stopRunning)
			throw runtime_error("CreateEvent failed: "s + to_string(GetLastError()) + "\n"s);

		messageThread = thread(MessageThread, app);
	}

	void StopReceivingMessages()
	{
		if (SetEvent(stopRunning)) {
			messageThread.join();
		} else {
			cerr << "SetEvent failed: " << GetLastError() << "\n";
			messageThread.detach();
		}
	}
}
