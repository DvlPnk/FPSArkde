// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "FPSArkde/FPSArkdeGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFPSArkdeGameModeBase() {}
// Cross Module References
	FPSARKDE_API UClass* Z_Construct_UClass_AFPSArkdeGameModeBase_NoRegister();
	FPSARKDE_API UClass* Z_Construct_UClass_AFPSArkdeGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_FPSArkde();
// End Cross Module References
	void AFPSArkdeGameModeBase::StaticRegisterNativesAFPSArkdeGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_AFPSArkdeGameModeBase_NoRegister()
	{
		return AFPSArkdeGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AFPSArkdeGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AFPSArkdeGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_FPSArkde,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AFPSArkdeGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "FPSArkdeGameModeBase.h" },
		{ "ModuleRelativePath", "FPSArkdeGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AFPSArkdeGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AFPSArkdeGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AFPSArkdeGameModeBase_Statics::ClassParams = {
		&AFPSArkdeGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AFPSArkdeGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AFPSArkdeGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AFPSArkdeGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AFPSArkdeGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AFPSArkdeGameModeBase, 3227341447);
	template<> FPSARKDE_API UClass* StaticClass<AFPSArkdeGameModeBase>()
	{
		return AFPSArkdeGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AFPSArkdeGameModeBase(Z_Construct_UClass_AFPSArkdeGameModeBase, &AFPSArkdeGameModeBase::StaticClass, TEXT("/Script/FPSArkde"), TEXT("AFPSArkdeGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AFPSArkdeGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
