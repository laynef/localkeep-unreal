# Local Keep AI — Unreal Editor plugin

Chat with your own AI models inside the Unreal Editor. Explain a Blueprint or
C++ file, generate tests, and run tasks on the models you choose — local or
cloud — through the Local Keep AI CLI.

## Requirements

* **Unreal Engine 5.8** — the version this build targets.
* **A C++ project.** A Blueprint-only project has no build pipeline for an
  editor module; add any C++ class to convert it.
* **Windows, macOS or Linux.**
* **The Local Keep AI CLI**, installed separately and free to start:

      pip install local-keep-ai-cli
      lk login

  The plugin locates `lk` on your PATH and launches it. It never handles your
  credentials: the CLI holds the session, so the editor never sees a token.

## Installing

1. Copy the `LocalKeepAI` folder into your project's `Plugins/` directory.
2. Regenerate project files and rebuild.
3. Enable **Local Keep AI** in Edit → Plugins → Programming, and restart.

## Using it

Open **Tools → Local Keep AI** for the chat panel. Ask about the file you have
open, request tests, or describe a change. Answers come from the model you have
selected in the CLI (`lk use <model>`), so a local GGUF model and a hosted model
behave identically from the editor's point of view.

## If the panel says the CLI was not found

The plugin searches your PATH, `~/.local/bin` and pyenv shims. Confirm with:

    lk --version

If that works in a terminal but not in the editor, launch the editor from that
same terminal so it inherits your PATH.

## Support

* Documentation: https://localkeep.ai/docs/integrations/unreal
* Support: https://localkeep.ai/support
