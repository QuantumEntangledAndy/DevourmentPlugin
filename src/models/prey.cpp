#include "models/prey.hpp"
#include "data/forms.hpp"
#include "models/remapper.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	/**
	 * By default all are valid prey
	 *
	 * This method may be overridden by
	 * the sub classes to change this
	 *
	 * Maybe is used to say this class says they
	 * can be
	 */
	Ternary Prey::IsValidPrey() {
		if (IsInStomach()) {
			return Ternary::No;
		}
		return Ternary::Maybe;
	}

	bool Prey::IsInStomach() {
		return inside != nullptr;
	}

	Prey::Prey(TESObjectREFR* inobject) : DevrObj(inobject) {

	}

}
