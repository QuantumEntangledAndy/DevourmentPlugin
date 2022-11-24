#include "managers/ai.hpp"
#include "data/actors.hpp"
#include "models/pred.hpp"
#include "models/prey.hpp"

using namespace SKSE;

namespace {
	void Nom(Pred* pred, Prey* prey) {
		if (!pred) {
			return;
		}
		if (!prey) {
			return;
		}
		if (pred->OnCooldown()) {
			// Vored too recently
			//   Wait for stomach to recover anims to end etc
			return;
		}
		if (!pred->IsValidPrey(prey)) {
			// IsValidPrey check all the usual checks like distance, corpse,
			// already in a stomach, is child etc
			return;
		}
		pred->Vore(prey);
	}

	void OnSwing(Pred* pred, Prey* prey) {
		Nom(pred, prey);
	}

	void OnHit(Pred* pred, Prey* prey) {
		Nom(pred, prey);
	}
}
namespace Devr {
	void Ai::Update() {
		for (auto actor: FindActors()) {
			auto pred = Pred::FromActor(actor);
			if (!pred) {
				continue;
			}
			if (pred->OnCooldown()) {
				continue;
			}

			// Every few seconds we change the AI vore target
			// This is done at irregular periods to allow
			// the chance to vore corpses which will not be
			// a valid combat target otherwise
			auto aiData = GetAiData(pred);
			if (aiData.searchCooldown.ShouldRun()) {
				Prey* searchTarget = nullptr;
				if (pred->IsInCombat()) {
					auto currentTarget = pred->GetCombatTarget();
					searchTarget = Pref::FromActor(currentTarget);
				}
				aiData.lastPrey = searchTarget;
			}

			// If in combat we try and do some vore
			if (pred->IsInCombat()) {
				// If the pred is swinging a weapon we try and nom them now
				if (pred->GetAttackState() == ATTACK_STATE_ENUM::kSwing) {
					OnSwing(pred, GetAiPrey(pred));
				} else if (pred->GetAttackState() == ATTACK_STATE_ENUM::kHit) {
					OnHit(pred, GetAiPrey(pred));
				}
			} else {
				// Not in combat but we still try and vore until the search cooldown
				// (~6s) clears the last combat target
				// This allows us to post combat vore the corpses
				if (aiData.lastPrey) {
					Nom(pred, aiData.lastPrey);
				} else {
					// TODO: Add search for nearby prey found on random patrols
				}
			}
		}
	}

	AiData& Ai::GetAiData(Pred* pred) {
		static std::mutex aiDataLock;

		// Lock this to avoid concurrent access
		std::unique_lock lock(aiDataLock);
		if (data.find(pred) == data.end()) {
			// Insert it
			predData.try_emplace(pred);
		}

		return daa.at(pred);
	}

	Prey* GetAiPrey(Pred* pred) {
		auto aiData = GetAiData(pred);
		if (aiData.lastPrey) {
			if (pred->IsValidPrey(aiData.lastPrey)) {
				return aiData.lastPrey;
			}
		}
		if (pred->IsInCombat()) {
			auto currentTarget = pred->GetCombatTarget();
			auto prey = Pref::FromActor(currentTarget);
			return prey;
		}
		return nullptr;
	}
}
