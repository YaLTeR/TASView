#include "stdafx.hpp"
#include "Application.hpp"

void MessageThread(const Application *app, std::timed_mutex &m) {
	using namespace std::chrono;

	while (true) {
		try {
			boost::interprocess::message_queue mq(
				boost::interprocess::open_only,
				"BunnymodXT-TASView");

			unsigned char buf[256];
			boost::interprocess::message_queue::size_type received;
			unsigned priority;

			// Try reading messages, if there are no messages for >= 1s, reopen the queue.
			// Because if the queue was removed, we need to close it here.
			auto lastMessageTime = high_resolution_clock::now();
			while (!duration_cast<seconds>(high_resolution_clock::now() - lastMessageTime).count()) {
				// Quit if we were requested to.
				if (m.try_lock())
					return;

				if (mq.timed_receive(buf, sizeof(buf), received, priority, boost::posix_time::second_clock::universal_time() + boost::posix_time::millisec(100))) {
					lastMessageTime = high_resolution_clock::now();

					if (received < 2 || buf[0] != received) {
						std::cerr << "Received less than 2 bytes or less bytes than buf[0]!\n";
						continue;
					}

					switch (buf[1]) {
					case 0x00:
					{ // Time data.
						int32_t hours, minutes, seconds, milliseconds;
						std::memcpy(&hours, buf + 2, sizeof(hours));
						std::memcpy(&minutes, buf + 6, sizeof(minutes));
						std::memcpy(&seconds, buf + 10, sizeof(seconds));
						std::memcpy(&milliseconds, buf + 14, sizeof(milliseconds));
						app->GetBlockTime()->SetTime(hours, minutes, seconds, milliseconds);
						//std::cerr << "Received time! " << hours << " " << minutes << " " << seconds << " " << milliseconds << "\n";
					} break;

					case 0x01:
					{ // Clip data.
						float normalz, in[3], out[3];
						std::memcpy(&normalz, buf + 2, sizeof(normalz));
						std::memcpy(in, buf + 6, sizeof(in));
						std::memcpy(out, buf + 18, sizeof(out));
						app->GetBlockClips()->AddClip(normalz, in, out);
						//std::cerr << "Received clip! " << normalz << "\n";
					} break;

					case 0x02:
					{ // Water frame.
						app->GetBlockWater()->WaterFrame();
					} break;
					}
				}
			}

			// Quit if we were requested to.
			if (m.try_lock())
				return;
		} catch (boost::interprocess::interprocess_exception) {
			// Wait 1s before trying again. Quit if we were requested to at any point during this waiting.
			if (m.try_lock_for(seconds(1)))
				return;
		}
	}
}
