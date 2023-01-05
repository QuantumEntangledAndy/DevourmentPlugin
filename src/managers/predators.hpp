#progma once
/**
 * Handles all predators and their updates
 */

#include "models/pred.hpp"
#include "models/events.hpp"

namespace Devr {
	class Predators : public EventListener {
		public:
			Predators& GetSingleton();

			virtual void Update() override;

			std::unordered_map<Actor*, std::unique_ptr<Pred> > predData;
			static std::mutex predDataLock;
	};

}
