// Fill out your copyright notice in the Description page of Project Settings.


#include "CardUserWidget.h"
#include "../Card.h"
#include "Components/TextBlock.h"

UCardUserWidget::UCardUserWidget(const FObjectInitializer& init)
	: UUserWidget(init)
{
	m_Card = nullptr;
}

void UCardUserWidget::setCard(ACard* card)
{
	if (!card)
		return;

	m_Card = card;
	OnCardSet(card);
	m_VictoryPointsText->SetText(FText::AsNumber(m_Card->getCardInfo()->VictoryPoints));
}
