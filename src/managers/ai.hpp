#pragma once
/**
 * Handles combat AI
 */

#include "models/events.hpp"
#include "models/timer.hpp"

namespace Devr {
	struct AiData {
		Prey* lastPrey = nullptr;
		Timer searchCooldown = Timer(6.0);
	};

	class Ai : public EventListener {
    public:
        virtual void Update() override;

        AiData& GetAiData(Pred* pred);
        Prey* GetAiPrey(Pred* pred);

    private:
        std::unordered_map<Pred*, AiData> data;
    };
}
