#pragma once
/**
 * Handles predator related functions
 */

#include "moduels/edible.hpp"
#include "models/stomach.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;


namespace Devr {
	class Prey;

	class Pred : public PredPrey {
		public:
			static Pred* FromActor(Actor* actor);

			bool IsValidPred();
			bool IsValidPrey(Prey* prey);
			float GetCooldown();
			bool OnCooldown();
			float SwallowRange();
			void Vore(Prey* prey);


			const Actor* GetActor();

		protected:
			Pred(Actor* actor);
			NiPointer<Actor> actor;
			float lastSucessfulVoreTime = -1e8 .0; // Time of last vore in seconds of game runtime
	};
}
