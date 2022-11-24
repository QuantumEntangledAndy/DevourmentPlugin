#pragma once
/**
 * Handles comat AI
 */

#include "models/events.hpp"

namespace Devr {
	class Ai : public EventListener {
		public:
			virtual void Update() override;

		private:
			float swallowRange = 225.0;
			float cooldownNPC = 1.0;
			float cooldownCreature = 1.0;
	}
}
