#pragma once

using namespace RE;
using namespace SKSE;

namespace {
	using VM = RE::BSScript::Internal::VirtualMachine;
	using ObjectPtr = RE::BSTSmartPointer<RE::BSScript::Object>;
}

namespace Devr {
	inline RE::VMHandle GetHandle(RE::TESForm* a_form)
	{
		auto vm = VM::GetSingleton();
		auto policy = vm->GetObjectHandlePolicy();
		return policy->GetHandleForObject(a_form->GetFormType(), a_form);
	}

	inline ObjectPtr GetObjectPtr(RE::TESForm* a_form, const char* a_class, bool a_create)
	{
		auto vm = VM::GetSingleton();
		auto handle = GetHandle(a_form);

		ObjectPtr object = nullptr;
		bool found = vm->FindBoundObject(handle, a_class, object);
		if (!found && a_create) {
			vm->CreateObject2(a_class, object);
			vm->BindObject(object, handle, false);
		}

		return object;
	}

	template <class ... Args>
	inline void CallFunctionOn(TESForm* a_form, std::string_view formKind, std::string_view function, Args&&... a_args) {
		const auto skyrimVM = RE::SkyrimVM::GetSingleton();
		auto vm = skyrimVM ? skyrimVM->impl : nullptr;
		if (vm) {
			RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
			auto args = RE::MakeFunctionArguments(std::forward<Args>(a_args)...);
			auto objectPtr = GetObjectPtr(a_form, std::string(formKind).c_str(), false);
			vm->DispatchMethodCall(objectPtr, std::string(formKind).c_str(), args, callback);
		}
	}

	template <class ... Args>
	inline void CallFunction(std::string_view scriptName, std::string_view function, Args&&... a_args) {
		const auto skyrimVM = RE::SkyrimVM::GetSingleton();
		auto vm = skyrimVM ? skyrimVM->impl : nullptr;
		if (vm) {
			RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
			auto args = RE::MakeFunctionArguments(std::forward<Args>(a_args)...);
			vm->DispatchStaticCall(std::string(scriptName).c_str(), std::string(function).c_str(), args, callback);
		}
	}
}
