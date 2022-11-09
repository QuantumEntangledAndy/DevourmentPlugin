#include "managers/register.hpp"
#include "managers/skevents.hpp"
#include "models/events.hpp"

using namespace std;
using namespace RE;
using namespace SKSE;

namespace Devr {
	void RegisterManagers() {
		EventDispatcher::AddListener(&SkEvents::GetSingleton()); // Register for various skyrim events
	}
}
