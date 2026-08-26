// Copyright (c) 2026 Man Of Tech LLC. MIT licensed — see LICENSE.

#include "LocalKeepAIEditorModule.h"

#include "LkChatPanel.h"
#include "Framework/Docking/TabManager.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"

#define LOCTEXT_NAMESPACE "FLocalKeepAIEditorModule"

const FName FLocalKeepAIEditorModule::LocalKeepAITabName(TEXT("LocalKeepAI"));

void FLocalKeepAIEditorModule::StartupModule()
{
	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			LocalKeepAITabName,
			FOnSpawnTab::CreateRaw(this, &FLocalKeepAIEditorModule::SpawnChatTab))
		.SetDisplayName(LOCTEXT("TabTitle", "Local Keep AI"))
		.SetTooltipText(LOCTEXT("TabTooltip", "Ask your own AI models about this project"))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory());

	// Menus are registered through a callback rather than inline: UToolMenus
	// may not have finished its own startup when a plugin loads, and
	// registering directly against a half-built menu silently does nothing.
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &FLocalKeepAIEditorModule::RegisterMenus));
}

void FLocalKeepAIEditorModule::ShutdownModule()
{
	// Unregister in the reverse order of StartupModule. A tab spawner left
	// behind after the module unloads points at freed memory, and the crash
	// lands on whoever next opens the Window menu — far from the cause.
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LocalKeepAITabName);
}

void FLocalKeepAIEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Tools"));
	if (!ToolsMenu)
	{
		return;
	}
	FToolMenuSection& Section = ToolsMenu->FindOrAddSection(TEXT("LocalKeepAI"));
	Section.AddMenuEntry(
		TEXT("OpenLocalKeepAI"),
		LOCTEXT("MenuEntry", "Local Keep AI"),
		LOCTEXT("MenuEntryTooltip", "Open the Local Keep AI panel"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(
				FLocalKeepAIEditorModule::LocalKeepAITabName);
		})));
}

TSharedRef<SDockTab> FLocalKeepAIEditorModule::SpawnChatTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SLkChatPanel)
		];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLocalKeepAIEditorModule, LocalKeepAIEditor)
