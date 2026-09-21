# Local Keep AI for Unreal Engine

Ask your own AI models about your project without leaving the Unreal Editor —
explain a C++ class, generate tests, or run a task against **1,000+ models**,
local or cloud, with no API key needed for the local ones.

Plugin: `LocalKeepAI` · Minimum engine: **Unreal Engine 5.3**

[Website](https://localkeep.ai) · [Docs](https://localkeep.ai/docs/integrations/unreal)

## Install

The plugin does **not** bundle the CLI. Install that first — it holds your
session and your model choice, and the plugin reads both from it:

```bash
pip install local-keep-ai-cli
lk login
```

Then add the plugin to your project:

1. Copy this directory into your project's `Plugins/` folder, so you have
   `YourProject/Plugins/LocalKeepAI/LocalKeepAI.uplugin`.
2. Right-click your `.uproject` and choose **Generate Visual Studio project
   files** (or run `GenerateProjectFiles.sh` on macOS/Linux).
3. Open the project. Unreal builds the editor module on first launch and will
   offer to rebuild it if it is out of date — accept.
4. Confirm it loaded: **Edit → Plugins → Programming → Local Keep AI**.

A C++ project is required. A Blueprint-only project has no build pipeline for
an editor module; add any C++ class to convert it, then follow the steps above.

## Use

**Tools → Local Keep AI**, or **Window → Local Keep AI**, opens a dockable
panel. Type a question and press Enter.

Answers come from whichever model your CLI is set to. Change it in a terminal:

```bash
lk models          # what this account can reach
lk models --all    # every model, including the ones you have not enabled
```

## How it works

The plugin shells out to `lk`. It never talks to the Local Keep AI API itself
and never handles a token — the CLI already holds your session, so a second
copy of your credentials inside the editor would be one more place for them to
leak and one more thing to break the next time authentication changes.

Every call runs on a background thread and marshals its result back to the game
thread before touching a Slate widget. A model answer takes seconds; running it
on the editor's main thread would freeze Unreal, which is indistinguishable
from a crash to the person watching.

## Troubleshooting

**"Local Keep AI CLI not found"** — the editor cannot see `lk`. A GUI app
launched from Finder or the Windows shell does not inherit the PATH from your
login shell, which is why this can appear even when `lk` works in your
terminal. The plugin already checks `~/.local/bin`, `~/.pyenv/shims`,
`/opt/homebrew/bin`, `/usr/local/bin` and the Windows Python Scripts folder.
If yours is elsewhere, symlink it into one of those.

**The menu entry is missing** — the module did not load. Check
**Edit → Plugins** for Local Keep AI and enable it, then restart the editor.

**Answers mention a different model than you expected** — the plugin follows
the CLI. Run `lk models` to see the active one.

## Licence

MIT — see [LICENSE](LICENSE).
