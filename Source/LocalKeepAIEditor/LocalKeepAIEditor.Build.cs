// Copyright (c) 2026 Man Of Tech LLC. MIT licensed — see LICENSE.

using UnrealBuildTool;

public class LocalKeepAIEditor : ModuleRules
{
	public LocalKeepAIEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"InputCore",
			"UnrealEd",       // editor menus and the level-editor extender
			"ToolMenus",      // UToolMenus, the 5.x menu extension path
			"WorkspaceMenuStructure",
			"Projects",       // IPluginManager, for resolving our own content
			"Json",
			"JsonUtilities",
		});

		// NO HTTP MODULE, DELIBERATELY.
		//
		// Every other Local Keep AI editor integration shells out to the `lk`
		// CLI rather than talking to the API itself, and this one matches them.
		// The reason is not convenience: the CLI holds the user's session and
		// their model choice, so a plugin that spoke HTTP directly would need
		// its own copy of the credentials and would drift from `lk` the first
		// time auth changed. Shelling out means the editor never sees a token.
	}
}
