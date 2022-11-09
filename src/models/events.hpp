#pragma once

using namespace std;
using namespace RE;
using namespace SKSE;

namespace Devr {
	class EventListener {
		public:
			// Called on Live (non paused) gameplay
			virtual void Update();

			// Called on Papyrus OnUpdate
			virtual void PapyrusUpdate();

			// Called on Havok update (when processing hitjobs)
			virtual void HavokUpdate();

			// Called on game load started (not yet finished)
			// and when new game is selected
			virtual void Reset();

			// Called when game is enabled (while not paused)
			virtual void Enabled();

			// Called when game is disabled (while not paused)
			virtual void Disabled();

			// Called when a game is started after a load/newgame
			virtual void Start();

			// Called when all forms are loaded (during game load before mainmenu)
			virtual void DataReady();

			// Called when an actor is reset
			virtual void ResetActor(Actor* actor);

			// Called when an actor has an item equipped
			virtual void ActorEquip(Actor* actor);

			// Called when an actor has is fully loaded
			virtual void ActorLoaded(Actor* actor);

			// Called when a papyrus hit event is fired
			virtual void HitEvent(const TESHitEvent* evt);
	};

	class EventDispatcher {
		public:
			static void AddListener(EventListener* listener);
			static void DoUpdate();
			static void DoPapyrusUpdate();
			static void DoHavokUpdate();
			static void DoReset();
			static void DoEnabled();
			static void DoDisabled();
			static void DoStart();
			static void DoDataReady();
			static void DoResetActor(Actor* actor);
			static void DoActorEquip(Actor* actor);
			static void DoActorLoaded(Actor* actor);
			static void DoHitEvent(const TESHitEvent* evt);
		private:
			[[nodiscard]] static EventDispatcher& GetSingleton();
			std::vector<EventListener*> listeners;
	};
}
