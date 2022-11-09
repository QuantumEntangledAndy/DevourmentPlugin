#include "hooks/hooks.hpp"
#include "hooks/main.hpp"
#include "hooks/vm.hpp"
#include "hooks/havok.hpp"

using namespace RE;

namespace Hooks
{
	void Install()
	{
		logger::info("Devrourment applying hooks...");

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.create(256);

		HookMainUpdate::Hook(trampoline);;
		HookVM::Hook();
		HookHavok::Hook(trampoline);

		logger::info("Devrourment finished applying hooks...");
	}
}
