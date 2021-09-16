/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019-2021 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "QueueSystem.hpp"
#ifdef _3DS
	#include <3ds.h>
	static Thread QueueThread = nullptr;
#endif

std::deque<QueueEntry> QueueSystem::Queue;


void QueueSystem::Add(const size_t EntryIndex, const size_t DLIdx, const nlohmann::json *Script) {
	Queue.emplace_back(EntryIndex, DLIdx, Script);

	#ifdef _3DS
		if (Queue.size() == 1) {
			if (QueueThread) {
				threadJoin(QueueThread, U64_MAX);
				threadFree(QueueThread);
				QueueThread = nullptr;
			}

			int32_t Prio = 0;

			svcGetThreadPriority(&Prio, CUR_THREAD_HANDLE);
			QueueThread = threadCreate((ThreadFunc)QueueSystem::Handler, NULL, 64 * 1024, Prio - 1, -2, false);
			aptSetHomeAllowed(false);
		}
	#endif
};


void QueueSystem::Draw() {
	if (Queue.size() > 0) Queue[0].Draw();
};


void QueueSystem::Handler() {
	while(Queue.size()) {
		Queue[0].Handler();
		Queue.pop_front();
	}

	#ifdef _3DS
		aptSetHomeAllowed(true);
	#endif
};


void QueueSystem::Remove(const size_t Idx) {
	if (Idx < Queue.size()) {
		if (Idx == 0) Queue[0].Cancel();
		else Queue.erase(Queue.begin() + Idx);
	}
};
