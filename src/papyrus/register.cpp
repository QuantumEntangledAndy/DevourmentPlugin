#include "papyrus/register.hpp"
#include "papyrus/version.hpp"


using namespace SKSE;
using namespace RE;
using namespace RE::BSScript;

namespace Devr {
	bool RegisterPapyrus(IVirtualMachine* vm) {
		if (!RegisterVersion(vm)) {
			return false;
		}
		return true;
	}
}
