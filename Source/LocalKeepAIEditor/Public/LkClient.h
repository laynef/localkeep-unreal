// Copyright (c) 2026 Man Of Tech LLC. MIT licensed — see LICENSE.
#pragma once

#include "CoreMinimal.h"

/**
 * The bridge to the `lk` command-line tool.
 *
 * This plugin never speaks to the Local Keep AI API itself. `lk` already holds
 * the user's session and their chosen model, so a plugin with its own HTTP
 * client would need a second copy of the credentials and would drift from the
 * CLI the first time authentication changed. Shelling out means the editor
 * never handles a token at all.
 *
 * Every call runs OFF THE GAME THREAD. A model answer takes seconds; running
 * one on the editor's main thread freezes Unreal, and a frozen editor during a
 * long generation is indistinguishable from a crash.
 */
class LOCALKEEPAIEDITOR_API FLkClient
{
public:
	/** Absolute path to the `lk` binary, or empty if it cannot be found. */
	static FString FindBinary();

	/** The model to use: the plugin setting, else ~/.sage/config.json, else a default. */
	static FString ResolveModel();

	/**
	 * Run `lk` with the given arguments and return stdout.
	 * BLOCKING — callers must already be on a background thread.
	 * bOutSuccess is false when the binary is missing or the exit code is not 0.
	 */
	static FString RunSync(const TArray<FString>& Args, bool& bOutSuccess, FString& OutError);

	/** `lk ask --prompt <text> --model <m> --raw` — one question, one answer. */
	static void AskAsync(const FString& Prompt, TFunction<void(bool, FString)> OnDone);

	/** `lk run --file <path> --prompt <text> --model <m> --raw` — act on a file. */
	static void RunOnFileAsync(const FString& FilePath, const FString& Instruction,
	                           TFunction<void(bool, FString)> OnDone);

	/** `lk models --all` — the model ids the user can pick from. */
	static void ListModelsAsync(TFunction<void(bool, TArray<FString>)> OnDone);

	/** The install line shown whenever the binary is missing. */
	static FString InstallHint();
};
