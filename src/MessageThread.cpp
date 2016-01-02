#include "stdafx.hpp"
#include "MessageThread.hpp"
#include "Application.hpp"
#include "MessageQueue.hpp"

using namespace std;

enum class MessageType : unsigned char {
	TIME = 0x00,
	CLIP = 0x01,
	WATER = 0x02,
	FRAMETIME_REMAINDER = 0x03
};

void MessageThread(const Application *app, HANDLE stopRunning) {
	while (true) {
		// Quit if we were requested to.
		if (WaitForSingleObject(stopRunning, 0) != WAIT_TIMEOUT)
			return;

		try {
			MessageQueue mq("BunnymodXT-TASView");
			vector<unsigned char> buf(256);
			DWORD bytesRead;

			while (true) {
				if (mq.timed_read(buf, 1s, bytesRead, stopRunning)) {
					if (bytesRead < 2 || buf[0] != bytesRead) {
						std::cerr << "Received less than 2 bytes or less bytes than buf[0]!\n";
						continue;
					}

					switch (buf[1]) {
					case MessageType::TIME:
					{ // Time data.
						int32_t hours, minutes, seconds, milliseconds;
						std::memcpy(&hours, buf.data() + 2, sizeof(hours));
						std::memcpy(&minutes, buf.data() + 6, sizeof(minutes));
						std::memcpy(&seconds, buf.data() + 10, sizeof(seconds));
						std::memcpy(&milliseconds, buf.data() + 14, sizeof(milliseconds));
						app->GetBlockTime()->SetTime(hours, minutes, seconds, milliseconds);
						//std::cerr << "Received time! " << hours << " " << minutes << " " << seconds << " " << milliseconds << "\n";
					} break;

					case MessageType::CLIP:
					{ // Clip data.
						float normalz, in[3], out[3];
						std::memcpy(&normalz, buf.data() + 2, sizeof(normalz));
						std::memcpy(in, buf.data() + 6, sizeof(in));
						std::memcpy(out, buf.data() + 18, sizeof(out));
						app->GetBlockClips()->AddClip(normalz, in, out);
						//std::cerr << "Received clip! " << normalz << "\n";
					} break;

					case MessageType::WATER:
					{ // Water frame.
						app->GetBlockWater()->WaterFrame();
					} break;

					case MessageType::FRAMETIME_REMAINDER:
					{
						double remainder;
						std::memcpy(&remainder, buf.data() + 2, 8);
						app->GetBlockRemainder()->SetRemainder(remainder);
					} break;
					}
				}

				// Quit if we were requested to.
				if (WaitForSingleObject(stopRunning, 0) != WAIT_TIMEOUT)
					return;
			}
		} catch (const std::exception& ex) {
			// Wait 1s before trying again. Quit if we were requested to at any point during this waiting.
			std::cerr << "Exception: " << ex.what() << "\n";
			if (WaitForSingleObject(stopRunning, 1000) != WAIT_TIMEOUT)
				return;
		}
	}
}
