#include "../../stdafx.hpp"
#include "../../IPC.hpp"
#include "../../Shared.hpp"

using namespace std;
using namespace std::literals;

namespace IPC
{
	timed_mutex stopRunning;
	thread messageThread;

	static void MessageThread(Application *app) {
		while (true) {
			// Quit if we were requested to.
			if (stopRunning.try_lock())
				return;

			mqd_t mq = mq_open("/" MQ_NAME, O_RDONLY);
			if (mq == static_cast<mqd_t>(-1)) {
				// Retry in one second.
				if (stopRunning.try_lock_for(1s))
					return;
				continue;
			}

			struct mq_attr attr;
			mq_getattr(mq, &attr);
			vector<char> buf(attr.mq_msgsize);

			while (true) {
				struct timespec ts;
				clock_gettime(CLOCK_REALTIME, &ts);
				ts.tv_sec += 1;
				auto bytesRead = mq_timedreceive(mq, buf.data(), buf.size(), nullptr, &ts);
				if (bytesRead == -1) {
					// Timeout, reopen the message queue.
					break;
				}

				// Parse.
				app->ParseMessage(buf, bytesRead);

				// Quit if we were requested to.
				if (stopRunning.try_lock()) {
					mq_close(mq);
					return;
				}
			}

			mq_close(mq);
		}
	}

	void StartReceivingMessages(Application *app)
	{
		stopRunning.lock();
		messageThread = thread(MessageThread, app);
	}

	void StopReceivingMessages()
	{
		stopRunning.unlock();

		// Try sending a message into the queue
		// to unblock from mq_timedreceive().
		mqd_t mq = mq_open("/" MQ_NAME, O_WRONLY | O_NONBLOCK);
		if (mq != static_cast<mqd_t>(-1)) {
			char buf[] = { 2, static_cast<char>(MessageType::WATER) };
			mq_send(mq, buf, sizeof(buf), 0);
			mq_close(mq);
		}

		messageThread.join();
	}
}
