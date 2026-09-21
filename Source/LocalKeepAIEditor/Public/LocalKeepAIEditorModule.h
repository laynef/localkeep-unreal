// Copyright (c) 2026 Man Of Tech LLC.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class SDockTab;
class FSpawnTabArgs;

/**
 * Editor-only module: registers the Local Keep AI tab and the Tools menu entry.
 *
 * Editor type, PostEngineInit loading phase — this module touches UToolMenus
 * and the tab manager, neither of which exists in a packaged game. Shipping it
 * as a Runtime module would make every cooked build carry editor-only symbols
 * and fail to link.
 */
class FLocalKeepAIEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** The tab id, also used by the Window menu entry. */
	static const FName LocalKeepAITabName;

private:
	void RegisterMenus();
	TSharedRef<SDockTab> SpawnChatTab(const FSpawnTabArgs& Args);
};
