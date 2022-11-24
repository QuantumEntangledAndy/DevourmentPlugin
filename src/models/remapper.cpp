#include "models/remap.hpp"

using namespace RE;

namespace Devr {
	TESRace* Remapper::Race(TESRace* originalRace) {
		return originalRace; // TODO: Add json remapping
	}

	Remapper& Remapper::GetSingleton() {
		static Remapper instance;
		return instance;
	}
}
