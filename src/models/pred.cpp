#include "models/pred.hpp"
#include "models/prey.hpp"
#include "data/forms.hpp"
#include "models/remapper.hpp"
#include "managers/predators.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace {
	const float BASE_SWALLOW_RANGE = 225.0;

	bool IsInBlacklist(Actor* actor) {
		return false; // TODO: Add actual blacklist logic
	}

	bool IsInWhitelist(Actor* actor) {
		return false; // TODO: Add actual whitelist logic
	}
}

namespace Devr {

	/**
	 * General checks for preds
	 *
	 * More specific checks see personas
	 */
	Ternary Pred::ValidPred() {
		if (IsInBlacklist(actor)) {
			return Ternary::No;
		}
		if (IsInWhitelist(actor)) {
			return Ternary::Yes;
		}
		return Ternary::Maybe;
	}

	/**
	 * Check if this pred can devour this prey
	 *
	 * This is very general checks for more specific acto related
	 * check see personas
	 */
	bool Pred::CanDevrPrey(Prey* prey, std::string_view stomach_name) {
		if (!prey) {
			return false;
		}
		if (prey->IfValidPrey() == Ternary::No) {
			return false;
		}
		auto stomach = GetStomach(stomach_name);
		if (stomach->Room() < prey->GetVolume()) {
			return false;
		}
		return true;
	}

	Stomach* GetStomach(std::string_view name) {
		auto search = stomachs.find(name);
		if (search != stomachs.end()) {
			return (*search);
		} else {
			return nullptr;
		}
	}

	Pred::Pred(TESObjectREFR* inobject) : Prey(inobject) {

	}

}
