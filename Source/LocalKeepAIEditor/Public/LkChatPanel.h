// Copyright (c) 2026 Man Of Tech LLC.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SEditableTextBox;
class SMultiLineEditableTextBox;
class STextBlock;

/** The Local Keep AI tab: a transcript, a prompt box, and the busy state. */
class SLkChatPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLkChatPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnSendClicked();
	void OnPromptCommitted(const FText& Text, ETextCommit::Type CommitType);
	void Send(const FString& Prompt);
	void Append(const FString& Speaker, const FString& Body);

	/** True while a request is in flight; disables Send so one user cannot
	 *  queue five model calls by pressing Enter five times. */
	bool bBusy = false;

	FString Transcript;
	TSharedPtr<SMultiLineEditableTextBox> TranscriptBox;
	TSharedPtr<SEditableTextBox> PromptBox;
	TSharedPtr<STextBlock> StatusText;
};
