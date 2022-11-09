#pragma once
// Module that holds data that is persistent across saves

#include "models/serde.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;
using namespace Devr;

namespace Devr {
	class Cosave {
		public:
			[[nodiscard]] static Cosave& GetSingleton() noexcept;
			static void OnRevert(SKSE::SerializationInterface*);
			static void OnGameSaved(SKSE::SerializationInterface* serde);
			static void OnGameLoaded(SKSE::SerializationInterface* serde);

			static void RegisterForSerde(std::uint32_t tag, Serde* callback);

			std::unordered_map<std::uint32_t, Serde*> serde_registry;

		private:
			Cosave() = default;

			mutable std::mutex _lock;
	};
}
