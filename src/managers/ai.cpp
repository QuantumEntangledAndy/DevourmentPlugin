#include "managers/ai.hpp"
#include "data/actors.hpp"
#include "models/pred.hpp"
#include "models/prey.hpp"

using namespace SKSE;

namespace {
	bool CombatVoreCheck(Pred* pred, Prey* prey) {
		if (!pred) {
			return false;
		}
		if (!prey) {
			return false;
		}
		if (pred->IsInCombat()) {
			return pred->IsValidPrey(prey);
		}
		return false;
	}
}

namespace Devr {
	void Ai::Update() {
		for (auto actor: FindActors()) {
			auto pred = Pred::FromActor(actor);
			if (!pred) {
				continue;
			}

			std::string predName = pre.Name();
			float reach = swallowRange + pred->GetLength();
			bool doBleedoutVore = (GetPluginInfo("SexLabDefeat.esp") == nullptr);

			float cooldownTime = pred->GetCooldown();

			auto currentTarget = pred->GetCombatTarget();
			auto prey = Pref::FromActor(currentTarget);
			if (prey) {
				if (CombatVoreCheck(pred, prey)) {
					// Gets s list of combat spells
					// TODO: Redesign
				}
			}
		}
	}
}
