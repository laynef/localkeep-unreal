// Copyright (c) 2026 Man Of Tech LLC. MIT licensed — see LICENSE.

#include "LkClient.h"

#include "Async/Async.h"
#include "Dom/JsonObject.h"
#include "HAL/PlatformProcess.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	/** Home directory, cross-platform. Unreal has no single accessor for this. */
	FString HomeDir()
	{
#if PLATFORM_WINDOWS
		FString Home = FPlatformMisc::GetEnvironmentVariable(TEXT("USERPROFILE"));
#else
		FString Home = FPlatformMisc::GetEnvironmentVariable(TEXT("HOME"));
#endif
		return Home;
	}

	/**
	 * The same candidate list the Unity and Godot plugins use, in the same
	 * order. A user who installed with pipx, pyenv or Homebrew ends up with the
	 * binary in a different place each time, and PATH is not inherited by a GUI
	 * app launched from Finder or the Windows shell — which is exactly why
	 * "works in my terminal, not in the editor" is the commonest report.
	 */
	TArray<FString> CandidatePaths()
	{
		const FString Home = HomeDir();
		TArray<FString> Out;
#if PLATFORM_WINDOWS
		Out.Add(FPaths::Combine(Home, TEXT("AppData"), TEXT("Roaming"), TEXT("Python"), TEXT("Scripts"), TEXT("lk.exe")));
		Out.Add(FPaths::Combine(Home, TEXT(".local"), TEXT("bin"), TEXT("lk.exe")));
		Out.Add(TEXT("lk.exe"));
#else
		Out.Add(FPaths::Combine(Home, TEXT(".local"), TEXT("bin"), TEXT("lk")));
		Out.Add(FPaths::Combine(Home, TEXT(".pyenv"), TEXT("shims"), TEXT("lk")));
		Out.Add(TEXT("/opt/homebrew/bin/lk"));
		Out.Add(TEXT("/usr/local/bin/lk"));
		Out.Add(TEXT("lk"));
#endif
		return Out;
	}
}

FString FLkClient::InstallHint()
{
	return TEXT("Local Keep AI CLI not found.\n"
	            "Install it, then sign in:\n"
	            "    pip install local-keep-ai-cli\n"
	            "    lk login\n"
	            "The plugin does not bundle the CLI — it uses your own session.");
}

FString FLkClient::FindBinary()
{
	for (const FString& Candidate : CandidatePaths())
	{
		// A bare name ("lk") has no path to test; leave resolution to the OS.
		if (!Candidate.Contains(TEXT("/")) && !Candidate.Contains(TEXT("\\")))
		{
			continue;
		}
		if (FPaths::FileExists(Candidate))
		{
			return Candidate;
		}
	}
	// Fall back to the bare name and let the platform search PATH. If that
	// fails too, RunSync reports it as a missing binary rather than a crash.
#if PLATFORM_WINDOWS
	return TEXT("lk.exe");
#else
	return TEXT("lk");
#endif
}

FString FLkClient::ResolveModel()
{
	const FString ConfigPath = FPaths::Combine(HomeDir(), TEXT(".sage"), TEXT("config.json"));
	FString Raw;
	if (FFileHelper::LoadFileToString(Raw, *ConfigPath))
	{
		TSharedPtr<FJsonObject> Root;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Raw);
		if (FJsonSerializer::Deserialize(Reader, Root) && Root.IsValid())
		{
			FString Model;
			if (Root->TryGetStringField(TEXT("default_model"), Model) && !Model.IsEmpty())
			{
				return Model;
			}
		}
	}
	// The same fallback the other engine plugins use, so a user who has never
	// set a default gets the same model in Unreal as in Unity.
	return TEXT("cloud:qwen3-coder");
}

FString FLkClient::RunSync(const TArray<FString>& Args, bool& bOutSuccess, FString& OutError)
{
	bOutSuccess = false;
	OutError.Empty();

	const FString Binary = FindBinary();

	// ARGUMENTS ARE QUOTED INDIVIDUALLY, NOT CONCATENATED RAW. A prompt is
	// arbitrary user text and will contain spaces and quotes; pasting it into
	// one command string is how an editor plugin turns a question into a shell
	// injection. FPlatformProcess takes a single params string, so each
	// argument is wrapped and its embedded quotes escaped here.
	FString Params;
	for (const FString& Arg : Args)
	{
		FString Escaped = Arg;
		Escaped.ReplaceInline(TEXT("\""), TEXT("\\\""));
		Params += FString::Printf(TEXT("\"%s\" "), *Escaped);
	}
	Params.TrimEndInline();

	int32 ReturnCode = -1;
	FString StdOut;
	FString StdErr;

	const bool bLaunched = FPlatformProcess::ExecProcess(
		*Binary, *Params, &ReturnCode, &StdOut, &StdErr);

	if (!bLaunched)
	{
		OutError = InstallHint();
		return FString();
	}
	if (ReturnCode != 0)
	{
		// The CLI's own message is far more useful than "exit code 1", and it
		// is what tells the user to run `lk login` when the session lapsed.
		OutError = StdErr.IsEmpty()
			? FString::Printf(TEXT("lk exited with code %d"), ReturnCode)
			: StdErr.TrimStartAndEnd();
		return StdOut;
	}

	bOutSuccess = true;
	return StdOut;
}

void FLkClient::AskAsync(const FString& Prompt, TFunction<void(bool, FString)> OnDone)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [Prompt, OnDone]()
	{
		const FString Model = ResolveModel();
		TArray<FString> Args;
		Args.Add(TEXT("ask"));
		Args.Add(TEXT("--prompt"));
		Args.Add(Prompt);
		Args.Add(TEXT("--model"));
		Args.Add(Model);
		Args.Add(TEXT("--raw"));

		bool bOk = false;
		FString Error;
		FString Output = RunSync(Args, bOk, Error);

		// Back to the game thread before touching any Slate widget — Slate is
		// not thread-safe and a cross-thread update is a crash, not a glitch.
		AsyncTask(ENamedThreads::GameThread, [bOk, Output, Error, OnDone]()
		{
			OnDone(bOk, bOk ? Output : Error);
		});
	});
}

void FLkClient::RunOnFileAsync(const FString& FilePath, const FString& Instruction,
                               TFunction<void(bool, FString)> OnDone)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [FilePath, Instruction, OnDone]()
	{
		const FString Model = ResolveModel();
		TArray<FString> Args;
		Args.Add(TEXT("run"));
		Args.Add(TEXT("--file"));
		Args.Add(FilePath);
		Args.Add(TEXT("--prompt"));
		Args.Add(Instruction);
		Args.Add(TEXT("--model"));
		Args.Add(Model);
		Args.Add(TEXT("--raw"));

		bool bOk = false;
		FString Error;
		FString Output = RunSync(Args, bOk, Error);

		AsyncTask(ENamedThreads::GameThread, [bOk, Output, Error, OnDone]()
		{
			OnDone(bOk, bOk ? Output : Error);
		});
	});
}

void FLkClient::ListModelsAsync(TFunction<void(bool, TArray<FString>)> OnDone)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [OnDone]()
	{
		TArray<FString> Args;
		Args.Add(TEXT("models"));
		Args.Add(TEXT("--all"));

		bool bOk = false;
		FString Error;
		const FString Output = RunSync(Args, bOk, Error);

		TArray<FString> Models;
		if (bOk)
		{
			TArray<FString> Lines;
			Output.ParseIntoArrayLines(Lines);
			for (const FString& Line : Lines)
			{
				FString Trimmed = Line.TrimStartAndEnd();
				// The listing is a table; the id is the first column. Only the
				// known runtime prefixes are accepted so headings and totals
				// never end up in the picker as if they were models.
				int32 Space = INDEX_NONE;
				Trimmed.FindChar(TEXT(' '), Space);
				const FString First = (Space == INDEX_NONE) ? Trimmed : Trimmed.Left(Space);
				if (First.StartsWith(TEXT("cloud:")) || First.StartsWith(TEXT("openrouter:"))
					|| First.StartsWith(TEXT("ollama:")) || First.StartsWith(TEXT("llama_cpp:")))
				{
					Models.Add(First);
				}
			}
		}

		AsyncTask(ENamedThreads::GameThread, [bOk, Models, OnDone]()
		{
			OnDone(bOk, Models);
		});
	});
}
