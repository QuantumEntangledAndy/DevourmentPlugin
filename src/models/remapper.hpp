#progma once
/**
 * Remaps certain data based on a configurable json
 */

using namespace RE;

namespace Devr {
	class Remapper {
		public:
			static TESRace* Race(TESRace* originalRace);
		protected:
			Remapper& GetSingleton();
	};
}
