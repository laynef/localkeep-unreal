// Copyright (c) 2026 Man Of Tech LLC. MIT licensed — see LICENSE.

#include "LkChatPanel.h"

#include "LkClient.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SLkChatPanel"

void SLkChatPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)

		// Transcript. Read-only rather than disabled: a disabled box greys the
		// text out and an answer you cannot read is not an answer.
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(6.0f)
		[
			SAssignNew(TranscriptBox, SMultiLineEditableTextBox)
			.IsReadOnly(true)
			.AutoWrapText(true)
			.Text(LOCTEXT("Empty",
				"Ask about this project. Answers come from the model set in your "
				"Local Keep AI CLI — local or cloud, your choice.\n"))
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(6.0f, 0.0f)
		[
			SAssignNew(StatusText, STextBlock)
			.Text(FText::GetEmpty())
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(6.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SAssignNew(PromptBox, SEditableTextBox)
				.HintText(LOCTEXT("Hint", "Ask anything about this project"))
				.OnTextCommitted(this, &SLkChatPanel::OnPromptCommitted)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(6.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("Send", "Send"))
				.OnClicked(this, &SLkChatPanel::OnSendClicked)
				.IsEnabled_Lambda([this]() { return !bBusy; })
			]
		]
	];
}

FReply SLkChatPanel::OnSendClicked()
{
	Send(PromptBox->GetText().ToString());
	return FReply::Handled();
}

void SLkChatPanel::OnPromptCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	// Only Enter sends. OnTextCommitted also fires when the box loses focus,
	// so without this check clicking away from a half-typed question would
	// silently spend a model call on it.
	if (CommitType == ETextCommit::OnEnter)
	{
		Send(Text.ToString());
	}
}

void SLkChatPanel::Send(const FString& Prompt)
{
	const FString Trimmed = Prompt.TrimStartAndEnd();
	if (Trimmed.IsEmpty() || bBusy)
	{
		return;
	}

	bBusy = true;
	StatusText->SetText(LOCTEXT("Thinking", "Thinking…"));
	Append(TEXT("You"), Trimmed);
	PromptBox->SetText(FText::GetEmpty());

	// The callback is delivered on the game thread by FLkClient, so touching
	// Slate here is safe. SharedThis keeps the panel alive if the user closes
	// the tab mid-request — without it the answer would land on a freed widget.
	TSharedRef<SLkChatPanel> Self = SharedThis(this);
	FLkClient::AskAsync(Trimmed, [Self](bool bOk, FString Reply)
	{
		Self->bBusy = false;
		Self->StatusText->SetText(FText::GetEmpty());
		Self->Append(bOk ? TEXT("Local Keep AI") : TEXT("Error"), Reply.TrimStartAndEnd());
	});
}

void SLkChatPanel::Append(const FString& Speaker, const FString& Body)
{
	Transcript += FString::Printf(TEXT("%s: %s\n\n"), *Speaker, *Body);
	TranscriptBox->SetText(FText::FromString(Transcript));
	TranscriptBox->ScrollTo(ETextLocation::EndOfDocument);
}

#undef LOCTEXT_NAMESPACE
