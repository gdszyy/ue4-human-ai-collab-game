// Copyright 2025 Voidzyy. All Rights Reserved.

#include "EchoAlchemist.h"
#include "Modules/ModuleManager.h"

void FEchoAlchemistModule::StartupModule()
{
	// This code will execute after your module is loaded into memory
	UE_LOG(LogTemp, Log, TEXT("EchoAlchemist Module Started"));
}

void FEchoAlchemistModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module
	UE_LOG(LogTemp, Log, TEXT("EchoAlchemist Module Shutdown"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FEchoAlchemistModule, EchoAlchemist, "EchoAlchemist");
