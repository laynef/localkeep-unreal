# Third Party Software form — answers for the Fab submission

The first TRC returned:

> For Plugins that use Third Party Code, the Third Party Software form is
> accurately filled out. **FAIL** — Any plugins using Third-party software,
> must fill the form completely.

This file holds the answers to paste into that form, and the evidence behind
each one, so the next submission is not guesswork.

## The short answer

**This plugin bundles no third-party code.** Every dependency it compiles
against is a first-party Unreal Engine module, there is no `ThirdParty` folder,
and no external library, DLL or static lib ships in the zip.

Verified from the source rather than from memory:

```
Source/LocalKeepAIEditor/LocalKeepAIEditor.Build.cs
  Public:  Core
  Private: CoreUObject, Engine, Slate, SlateCore, InputCore, UnrealEd,
           ToolMenus, WorkspaceMenuStructure, Projects, Json, JsonUtilities
```

All twelve are Epic modules shipped with the engine. A scan of every `#include`
in `Source/` returns nothing outside the engine (`Async/Async.h` is Epic's own).
There is no `ThirdParty` directory anywhere in the plugin.

## What the form is probably reacting to

The plugin runs the **Local Keep AI CLI** as an external process:

```cpp
// Source/LocalKeepAIEditor/Private/LkClient.cpp
FPlatformProcess::ExecProcess(...)   // invokes `lk`
```

That is worth declaring plainly, but note two things about it:

1. **It is not third-party.** The CLI is this publisher's own software
   (`local-keep-ai-cli` on PyPI, MIT). Declaring your own product as
   third-party software would be inaccurate in the other direction.
2. **It is not bundled.** The user installs it separately; the plugin only
   *locates and launches* it, and degrades to a clear message when it is
   absent. Nothing is redistributed.

## Answers to give

| Field | Answer |
| --- | --- |
| Does the plugin use third-party software? | **No** — no third-party code, libraries, DLLs or static libs are included or compiled in. |
| Third-party libraries included | **None.** |
| `ThirdParty` folder present | **No** — nothing to place in one. |
| External runtime dependencies | **Yes, one, and it is our own**: the Local Keep AI CLI (`local-keep-ai-cli`, MIT, https://pypi.org/project/local-keep-ai-cli/). Installed by the user; not redistributed with the plugin. The plugin locates it on `PATH` and launches it with `FPlatformProcess::ExecProcess`. |
| Does any code depend on UE source? | The editor module does, and it ships in **human-readable source form** inside the plugin folder — see `Source/LocalKeepAIEditor/`. No pre-compiled binaries are included. |
| Licences requiring attribution | **None** beyond the plugin's own `LICENSE`. |

## If the reviewer still wants the form completed

Complete it with the "no third-party software" answers above rather than
leaving it blank. The original Fail is most consistent with an *empty* form,
not with an inaccurate one — the checklist wording is "must fill the form
completely".
