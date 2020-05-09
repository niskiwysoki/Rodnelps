// Fill out your copyright notice in the Description page of Project Settings.


#include "TraderUserWidget.h"

UTraderUserWidget::UTraderUserWidget(const FObjectInitializer& init)
	: UUserWidget(init)
{
	m_Trader = nullptr;
}

void UTraderUserWidget::setTrader(ATraderCard* trader)
{
	if (!trader)
		return;

	m_Trader = trader;
	OnTraderSet(trader);
}


