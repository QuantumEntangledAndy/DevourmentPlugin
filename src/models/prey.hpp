#pragma once
/**
 * Handles prey related functions
 */

#include "models/devrobj.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	enum class Ternary {
		Yes,
		No,
		Maybe,
	};

	class Prey : public DevrObj {
		public:
			virtual Ternary IsValidPrey();
			virtual bool IsInStomach();

		protected:
			Prey(TESObjectREFR* actor);
			Stomach* inside; // Stomach the prey is inside. Nullptr if not inside
	};
}
