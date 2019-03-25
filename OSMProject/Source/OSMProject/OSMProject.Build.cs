// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class OSMProject : ModuleRules
{
	public OSMProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		bEnableUndefinedIdentifierWarnings = false; //error C4668 to warning, for osmium library.
		
		//EXTERN DEPENDANCIES
		//OSMIUM
		PublicIncludePaths.Add("D:/vcpkg/installed/x86-windows/include");
		PublicIncludePaths.Add("D:/protozero-code/include");
		PublicIncludePaths.Add("D:/UnrealProjects/OSMProject/Dependancies/libosmium-2.15.0/include");
		
		//OUR OSM SOURCE FILES
		PublicIncludePaths.Add("D:/UnrealProjects/OSMProject/SourceOSM/osmium");
		PublicIncludePaths.Add("D:/UnrealProjects/OSMProject/SourceOSM/readosm");
		
		//READOSM
		PublicAdditionalLibraries.Add("D:/vcpkg/installed/x86-windows/lib/readosm.lib");
	}
}
