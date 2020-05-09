// Fill out your copyright notice in the Description page of Project Settings.


#include "TokenUserWidget.h"

UTokenUserWidget::UTokenUserWidget(const FObjectInitializer& init)
	: UUserWidget(init)
{
	m_Token = nullptr;
}

void UTokenUserWidget::setToken(AToken* token)
{
	m_Token = token;
}
