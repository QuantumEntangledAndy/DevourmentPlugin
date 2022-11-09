#pragma once
// Keeps track of global state like is enabled
//

using namespace std;
using namespace RE;
using namespace SKSE;

namespace Devr {
	class Plugin {
		public:
			static bool Enabled();
			static bool InGame();
			static void SetInGame(bool value);
			static bool OnMainThread();
			static void SetOnMainThread(bool value);


		private:
			[[nodiscard]] static Plugin& GetSingleton();
			std::atomic_bool enabled = std::atomic_bool(true);
			std::atomic_bool ingame = std::atomic_bool(false);
			std::atomic_bool onmainthread = std::atomic_bool(false);
	};
}
