// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Kuhbrille : ModuleRules
{
	public Kuhbrille(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
