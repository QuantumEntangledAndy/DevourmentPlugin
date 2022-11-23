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
	}
}
