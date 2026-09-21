# Fab listing — Local Keep AI for Unreal Engine

Copy-paste source for the Fab publish form. Every claim below is checked
against what the plugin actually ships (`LocalKeepAI.uplugin`, `README.md`,
`Source/LocalKeepAIEditor/`) — Fab reviewers test the product against the
listing, and an unsupported claim is a rejection.

---

## Title *  (80 max)

```
Local Keep AI — Your Own AI Models in the Unreal Editor
```
54 characters. Leads with the brand, states the benefit, contains the words a
buyer searches ("AI", "Unreal Editor").

---

## Description *

```
Ask your own AI models about your project without leaving the Unreal Editor.

Open Tools → Local Keep AI and a dockable chat panel appears — a prompt box, a
Send button and a transcript. Type a question about your project — ask it to
explain a C++ class, sketch an approach, or draft a test — and the model's
answer appears in the panel, next to the work.

WHICH MODEL ANSWERS IS YOUR CHOICE
Local Keep AI reaches 1,000+ models. Run one locally on your own machine with
no API key and no data leaving your computer, or use a hosted model when you
want more capacity. Switch at any time from your terminal — the plugin follows
whatever your CLI is set to, so changing models does not mean changing tools.

BUILT FOR AN EDITOR, NOT BOLTED ONTO ONE
Every request runs on a background thread and returns to the game thread
before touching the UI, so the editor never freezes while a model is thinking.
A frozen Unreal is indistinguishable from a crashed one, and that was not an
acceptable trade.

YOUR CREDENTIALS STAY IN ONE PLACE
The plugin never talks to the Local Keep AI API directly and never stores a
token. It shells out to the CLI, which already holds your session — one place
for your credentials instead of two.

WHAT THE PANEL SHOWS
The panel renders whatever your model returns through the CLI — including any
source citations the model provides. It is a chat surface: you type, the model
answers in the transcript. It does not run or compile code itself.

REQUIREMENTS
• Unreal Engine 5.8 (the version this build targets and the one the listing
  declares — see the note below on why this is a single version, not a range)
• A C++ project (a Blueprint-only project has no build pipeline for an editor
  module — add any C++ class to convert it)
• Windows, macOS or Linux
• The Local Keep AI CLI, installed separately and free to start:
      pip install local-keep-ai-cli
      lk login
  The plugin does not bundle the CLI. Local models are free to run; hosted
  models are part of a Local Keep AI plan.

INSTALL
1. Copy the plugin into YourProject/Plugins/LocalKeepAI/
2. Regenerate project files and open the project — Unreal builds the editor
   module on first launch
3. Confirm under Edit → Plugins → Programming → Local Keep AI

Source included.
Docs: https://localkeep.ai/docs/integrations/unreal
Support: https://localkeep.ai/support
```

---

## Product type *
`Tools & Plugins` (already selected)

## Category *
`Code Plugins` — the module is an Editor module (`"Type": "Editor"`) and the
.uplugin declares `"Category": "Programming"`. If Fab's picker does not offer
Code Plugins under Tools & Plugins, use **Editor Tools / Productivity**.

---

## License type *
**Standard License (Free)** — the plugin source is MIT and costs nothing. Do
not pick Creative Commons: CC BY 4.0 would let anyone redistribute the plugin
under their own name with attribution only, which is not what MIT-with-a-
service-behind-it wants.

Note for the description's honesty: the plugin is free, and hosted models are
paid. That is stated in REQUIREMENTS above rather than buried — a reviewer who
finds a paywall you did not disclose treats it as a misrepresented listing.

---

## Tags *  (25 max — these 14 are the ones people actually search)

```
ai, assistant, chatgpt, llm, code, coding, editor, tools, utility,
productivity, blueprint, cpp, automation, workflow
```

---

## Thumbnail image *
`fab_thumbnail_1920x1080.png` (in this directory) — 1920×1080, 105 KB, PNG.
Meets the minimum resolution and is well under the 3 MB cap.

## Media gallery (at least 1 required)
Strongest to weakest, and the first two are worth capturing before you submit:

1. **Screenshot: the panel answering a real question** — Tools → Local Keep AI
   open beside a C++ file, with an actual answer visible. This is the single
   most convincing image and reviewers look for proof the product runs.
2. **Screenshot: Edit → Plugins → Programming → Local Keep AI**, enabled.
   Shows a clean install.
3. `fab_thumbnail_1920x1080.png` again as a gallery slide.
4. Optional: a short screen recording of asking a question and getting an
   answer. Video converts better than stills for editor tools.

---

## The questionnaire — recommended answers, with reasoning

**Epic Developer Community forum post** → **Yes, create a forum post.**
It is a free support channel attached to the listing, and it is where install
questions (the PATH one in particular) get answered publicly once instead of
privately many times.

**Mature content** → **No.**

**Disallow use by Generative AI Programs (NoAI meta tags)** → your call.
Checking it adds NoAI tags so the listing page is marked against AI scraping.
It does not affect what the plugin does. Given the product is itself an AI
tool, leaving it unchecked is defensible and checking it costs nothing.

**Was this product created with generative AI tools?** → **Yes, it was partly
or fully created with generative AI.**
This is a disclosure question, not a quality judgement, and the plugin's C++
was written with AI assistance. Answering "No" would be inaccurate, and
inaccurate disclosures are the kind of thing that costs a seller account
rather than a listing. Fab does not penalise the disclosure itself.

**Promotional content** → **Do not check it.**
Fab's examples are product placement, logos and slogans for a brand the
product is not itself. This plugin *is* the Local Keep AI client — the brand
presence is the product's identity, not an advertisement embedded in it, the
same way an official Slack or Perforce plugin carries its own name. The
requirement it does have (an external CLI and account) is disclosed in the
description, which is what a reviewer is actually checking for.

---

## Before you hit Submit

- [ ] Upload the plugin as a **zip whose root is the plugin folder** —
      `LocalKeepAI/LocalKeepAI.uplugin`, not the files loose at the top level.
      `Config/FilterPlugin.ini` already controls what ships.
- [ ] Tick **exactly 5.8** as the Supported Engine Version, and the platform
      list Win64 / Mac / Linux.

      WHY ONE VERSION, NOT A RANGE. The first TRC came back with

          "Project(s) provided match the Supported Engine Versions listed. FAIL
           Upon initial submission and approval, every product must contain a
           Project Version that has the latest version of Unreal Engine as a
           Supported Engine Version. (5.8)"

      The listing claimed "5.3 or newer" while one zip was uploaded. For a CODE
      plugin every ticked version needs its own build, because the module is
      compiled against that engine's API — ticking a range you have not built
      is what failed, not the range itself. Tick only versions you upload a
      build for, and 5.8 must be among them.
- [ ] `Version` in `LocalKeepAI.uplugin` is `1` / `"VersionName": "1.0.0"` —
      bump both together on any resubmission.
- [ ] Capture screenshot #1 above. A listing for an editor tool with no
      screenshot of the tool running is the most common reason these bounce.
