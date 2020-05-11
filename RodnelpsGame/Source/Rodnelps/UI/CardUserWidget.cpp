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
	
	NotifyCardDataChanged();
}

void UCardUserWidget::NotifyCardDataChanged()
{
	if (!m_Card)
		return;

	OnCardSet(m_Card);
	m_VictoryPointsText->SetText(FText::AsNumber(m_Card->getCardInfo()->VictoryPoints));
}
