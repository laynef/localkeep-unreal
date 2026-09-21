// Copyright (c) 2026 Man Of Tech LLC.

using UnrealBuildTool;

public class LocalKeepAIEditor : ModuleRules
{
	public LocalKeepAIEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// THESE TWO ARE WHAT KEEP A 5.8 BUILD WARNING-FREE.
		//
		// A module that sets neither is treated as written for an older engine,
		// and UnrealBuildTool emits deprecation warnings about the legacy
		// include order on every compile. Epic's checklist requires the plugin
		// to generate "no errors or consequential warnings", so a build that is
		// otherwise correct still fails review on the noise.
		//
		// Pinned to Latest deliberately: this plugin declares exactly one
		// supported engine version (5.8, see LocalKeepAI.uplugin), so "latest"
		// is unambiguous here. If the listing is ever widened to several engine
		// versions, pin these to a specific BuildSettingsVersion/
		// EngineIncludeOrderVersion instead — Latest shifts meaning between
		// engines and would then mean something different per build.
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

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
