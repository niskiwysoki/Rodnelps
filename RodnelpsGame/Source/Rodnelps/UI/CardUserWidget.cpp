// Fill out your copyright notice in the Description page of Project Settings.


#include "CardUserWidget.h"
#include "../Card.h"
#include "Components/TextBlock.h"

UCardUserWidget::UCardUserWidget(const FObjectInitializer& Init)
	: UUserWidget(Init)
{
	m_Card = nullptr;
}

void UCardUserWidget::SetCard(ACard* Card)
{
	if (!Card)
		return;

	m_Card = Card;
	OnCardSet(Card);
	m_VictoryPointsText->SetText(FText::AsNumber(m_Card->getCardInfo()->VictoryPoints));
}
