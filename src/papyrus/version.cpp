#include "papyrus/version.hpp"

using namespace SKSE;
using namespace Devr;
using namespace RE;
using namespace RE::BSScript;

namespace {
	const int MAJOR = 1;
	const int MINOR = 1;
	const int PATCH = 1;

	constexpr std::string_view PapyrusClass = "DevrVersion";
	BSFixedString GetVersionString(StaticFunctionTag*) {
		return std::format("Devourment Plugin {}.{}.{}", MAJOR, MINOR, PATCH);
	}

	int GetVersionMajor(StaticFunctionTag*) {
		return MAJOR;
	}

	int GetVersionMinor(StaticFunctionTag*) {
		return MINOR;
	}

	int GetVersionPatch(StaticFunctionTag*) {
		return PATCH;
	}
}

namespace Devr {
	bool RegisterVersion(IVirtualMachine* vm) {
		vm->RegisterFunction("Version", PapyrusClass, GetVersionString);
		vm->RegisterFunction("VersionMajor", PapyrusClass, GetVersionMajor);
		vm->RegisterFunction("VersionMinor", PapyrusClass, GetVersionMinor);
		vm->RegisterFunction("VersionPatch", PapyrusClass, GetVersionPatch);

		return true;
	}
}
