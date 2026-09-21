# Fab TRC — remediation for the current review round

Every FAIL from the latest Technical Review Checklist, with the exact action
that clears it. Items marked **[portal]** are done on the Fab publish form or
by uploading assets — they are not code changes. Items marked **[done]** are
already fixed in this repo.

Facts verified from source for this round (so the numbers below are real, not
remembered):

- C++ classes: **3** — `FLkClient`, `FLocalKeepAIEditorModule`, `SLkChatPanel`.
- Modules: **1** — `LocalKeepAIEditor`, type **Editor**.
- Blueprints: **0**.
- Executables/DLLs in the zip or tree: **none** (`unzip -l` + `find` both clean).
- Third-party libraries bundled/compiled in: **none** (all 12 Build.cs deps are
  Epic engine modules).

---

## 1. Media — "images must accurately display the relevant functionality" **[portal]**

The reviewer wants screenshots of the plugin **working inside the Unreal
Editor**, not marketing renders. Capture, at 1920×1080:

1. The docked **Tools → Local Keep AI** panel open in a real UE 5.8 project,
   with a question typed and a model's answer in the transcript.
2. The panel showing the "Thinking…" status mid-request (proves it runs).
3. The panel next to a C++/Blueprint file, showing a real answer about it.

Replace every listing image that is a logo/render with one of the above. The
same FAIL is on the Unity listing — reuse editor screenshots per engine.

---

## 2. Technical Information — fill every field **[portal]**

Paste this verbatim into the Technical Information box:

```
Features:
  • Dockable editor panel (Tools → Local Keep AI) that chats with your own
    AI models without leaving the Unreal Editor.
  • Answers come from whichever model your Local Keep AI CLI is set to —
    a local GGUF model with no data leaving your machine, or a hosted model.
  • All requests run on a background thread and return on the game thread,
    so the editor never blocks while a model is thinking.
  • The plugin never handles a token: it shells out to the CLI, which holds
    your session.

Code Modules:
  • LocalKeepAIEditor — Editor

Number of Blueprints: 0
Number of C++ Classes: 3
Network Replicated: No
Supported Development Platforms: Windows (Yes), Mac (Yes), Linux (Yes)
Supported Target Build Platforms: N/A — this is an EDITOR-ONLY plugin
  (LoadingPhase PostEngineInit, module Type "Editor"); it is not compiled
  into a packaged game build. Editor platforms: Win64, Mac, Linux.
Documentation Link: https://localkeep.ai/docs/integrations/unreal
Example Project: https://github.com/laynef/localkeep-unreal/releases/latest
  (a C++ UE 5.8 project with the plugin enabled; the plugin folder itself is
  not redistributed inside it)
Important/Additional Notes:
  • Requires a C++ project — a Blueprint-only project has no editor-module
    build pipeline; add any C++ class to convert it.
  • Requires the free Local Keep AI CLI installed separately
    (pip install local-keep-ai-cli; lk login). The plugin locates and
    launches it; it does not bundle or install it.
```

> Example Project: Fab wants a downloadable project that DEPENDS on the plugin
> but does not redistribute it. If the `releases/latest` link above ships the
> plugin inside `Plugins/`, instead publish a separate "example project" zip
> whose `Plugins/` is empty and whose README says to drop the plugin in. Do
> not leave this field blank — an empty field is the FAIL.

---

## 3. Distribution Method / "needs third-party software … not declared" **[portal]**

Stop answering "no third-party software". The reviewer has failed that twice.
The plugin genuinely requires an **external program it does not bundle** (the
`lk` CLI), and Fab's process for that is to DECLARE it, not to argue it away:

- Set the Distribution Method / dependency declaration to indicate the product
  **requires additional third-party software installed by the user**.
- Name it: **Local Keep AI CLI (`local-keep-ai-cli`, MIT,
  https://pypi.org/project/local-keep-ai-cli/)** — installed by the user via
  `pip install local-keep-ai-cli`, located and launched by the plugin, **not
  redistributed**.
- Put the same sentence in the product Description so a buyer sees it before
  purchase (see §6 wording).

Whether the CLI is "our own" software is beside the point to the reviewer: it
is external to the plugin package, so it must be declared as a dependency.

---

## 4. Documentation — "does not include instructions for using it on UE" **[done]**

The reviewer opened the DocsURL (https://localkeep.ai/docs/integrations/unreal)
and found only CLI-install steps. That page is now expanded with the in-editor
workflow — open the panel, type a question, Send, read the transcript, switch
models via the CLI — matched exactly to what `SLkChatPanel` actually does
(a prompt box, a Send button and a transcript; no in-panel model dropdown or
Explain/Test buttons, so none are claimed). `Docs/README.md` in the package
carries the same steps. Re-point the reviewer at the updated URL.

---

## 5. Quality — "we do not allow executables or program installer files" **[portal + explain]**

There is **no executable in the package** — `unzip -l LocalKeepAI-UE-1.0.0.zip`
and a tree `find` both return zero `.exe/.dll/.so/.dylib`. The reviewer flagged
the string literals `"lk.exe"` / `"lk"` in `LkClient.cpp` (lines 44–46, 84).
Those are the **names the plugin searches your PATH for** to launch your
already-installed CLI — the plugin neither ships nor installs a binary
(`InstallHint()` says so in the code). In the reviewer reply, state exactly
that and cite `unzip -l` output as evidence. This FAIL is the same
undeclared-dependency issue as §3 — declaring the CLI dependency (§3) plus the
Third Party form (§7) is what actually clears it.

---

## 6. Description wording to add **[portal]**

Add this near the top of the Fab Description (also satisfies §3 and §5):

```
REQUIRES THE FREE LOCAL KEEP AI CLI (installed separately)
This plugin does not bundle or install any executable. It locates and launches
the Local Keep AI CLI (local-keep-ai-cli, MIT) that you install yourself with
`pip install local-keep-ai-cli`, then `lk login`. If the CLI is not found the
panel shows install instructions instead of failing silently.
```

---

## 7. Third Party Software form — fill it completely **[portal]**

Submit Epic's form
(https://epicgames.formstack.com/forms/third_party_software_declaration_form)
declaring the ONE external dependency rather than leaving it blank:

| Field | Answer |
| --- | --- |
| Third-party **code/libraries bundled or compiled in** | **None.** All Build.cs dependencies are Epic engine modules; there is no `ThirdParty` folder and no DLL/static lib in the package. |
| **External runtime dependency the product requires** | **Yes — one.** Local Keep AI CLI (`local-keep-ai-cli`), MIT, https://pypi.org/project/local-keep-ai-cli/ . User-installed via pip; located on PATH and launched with `FPlatformProcess::ExecProcess`; **not redistributed**. |
| Executables/installers in the package | **None.** |
| Code depends on UE source | Yes — the editor module, shipped in **human-readable source** under `Source/LocalKeepAIEditor/`. No pre-compiled binaries. |
| Licences requiring attribution | None beyond the plugin's own. |

---

## 8. Code Plugins — "ThirdParty code must be in a folder under Source" **[N/A, explain]**

There is no third-party code to place in a `ThirdParty/` folder — see §7. State
that in the reply. This line resolves once the form in §7 is accepted.

---

## 9. Listing copy must match the shipped panel **[portal — important]**

`FAB_LISTING.md` (the current Description draft) claims more than the panel
does, and "All text must be accurate" + "function as intended" are graded
against the running product. Fix before resubmitting:

- It says the plugin can "explain a C++ class, walk through a Blueprint, draft
  tests" and "Generated code is executed in an isolated sandbox". The shipped
  `SLkChatPanel` is **chat only** — a prompt box, Send, and a transcript. You
  either (a) reword the Description to describe chat only (you can still ask it
  to explain code or write a test *in the prompt*), or (b) wire the
  already-written `FLkClient::RunOnFileAsync` / `ListModelsAsync` into the panel
  as real buttons before claiming them. Do not claim buttons that are not there.
- "1,400+ models" is stale. The true figure is ~1,032 — use "1,000+". A number
  a reviewer can check and find wrong is an accuracy FAIL.

---

## What only you can do vs. what is already done

- **Done in repo:** UE-usage web docs (§4); this remediation sheet; verified
  class/module counts and the zero-executable finding.
- **On the Fab portal (you):** upload real in-editor screenshots (§1); paste
  the Technical Information block (§2); set the dependency Distribution Method
  (§3); add the Description wording (§6); submit the Third Party form (§7);
  reply to the reviewer citing §5 and §8.
