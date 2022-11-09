#include "data/plugin.hpp"

using namespace std;
using namespace RE;
using namespace SKSE;

namespace Devr {
	bool Plugin::Enabled() {
		return Plugin::GetSingleton().enabled.load();
	}

	bool Plugin::InGame() {
		return Plugin::GetSingleton().ingame.load();
	}
	void Plugin::SetInGame(bool value) {
		Plugin::GetSingleton().ingame.store(value);
	}

	bool Plugin::OnMainThread() {
		return Plugin::GetSingleton().onmainthread.load();
	}
	void Plugin::SetOnMainThread(bool value) {
		Plugin::GetSingleton().onmainthread.store(value);
	}

	Plugin& Plugin::GetSingleton() {
		static Plugin instance;
		return instance;
	}
}
