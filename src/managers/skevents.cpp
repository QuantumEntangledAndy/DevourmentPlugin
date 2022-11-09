#include "managers/skevents.hpp"

using namespace SKSE;
using namespace RE;

namespace Devr {
	void SkEvents::DataReady() {
		auto event_sources = ScriptEventSourceHolder::GetSingleton();
		if (event_sources) {
			event_sources->AddEventSink<TESHitEvent>(this);
			event_sources->AddEventSink<TESObjectLoadedEvent>(this);
			event_sources->AddEventSink<TESEquipEvent>(this);
		}
	}
	SkEvents& SkEvents::GetSingleton() noexcept {
		static SkEvents instance;
		return instance;
	}

	BSEventNotifyControl SkEvents::ProcessEvent(const TESHitEvent * evn, BSTEventSource<TESHitEvent>* dispatcher)
	{
		if (evn) {
			EventDispatcher::DoHitEvent(evn);
		}
		return BSEventNotifyControl::kContinue;
	}

	BSEventNotifyControl SkEvents::ProcessEvent(const TESObjectLoadedEvent * evn, BSTEventSource<TESObjectLoadedEvent>* dispatcher)
	{
		if (evn) {
			auto* actor = TESForm::LookupByID<Actor>(evn->formID);
			if (actor) {
				EventDispatcher::DoActorLoaded(actor);
			}
		}
		return BSEventNotifyControl::kContinue;
	}

	BSEventNotifyControl SkEvents::ProcessEvent(const TESResetEvent* evn, BSTEventSource<TESResetEvent>* dispatcher)
	{
		if (evn) {
			auto* actor = TESForm::LookupByID<Actor>(evn->object->formID);
			if (actor) {
				EventDispatcher::DoResetActor(actor);
			}
		}
		return BSEventNotifyControl::kContinue;
	}

	BSEventNotifyControl SkEvents::ProcessEvent(const TESEquipEvent* evn, BSTEventSource<TESEquipEvent>* dispatcher)
	{
		if (evn) {
			auto* actor = TESForm::LookupByID<Actor>(evn->actor->formID);
			if (actor) {
				EventDispatcher::DoActorEquip(actor);
			}
		}
		return BSEventNotifyControl::kContinue;
	}
}
