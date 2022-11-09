#pragma once
#include "hooks/hooks.hpp"

using namespace RE;
using namespace SKSE;

namespace Hooks
{

	class HookHavok
	{
		public:
			static void Hook(Trampoline& trampoline);

		private:
			static void ProcessHavokHitJobs(void* a1);
			static inline REL::Relocation<decltype(ProcessHavokHitJobs)> _ProcessHavokHitJobs;
	};
}
