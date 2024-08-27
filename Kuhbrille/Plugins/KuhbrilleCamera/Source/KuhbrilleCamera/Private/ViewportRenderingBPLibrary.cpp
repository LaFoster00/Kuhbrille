// Copyright Epic Games, Inc. All Rights Reserved.

#include "ViewportRenderingBPLibrary.h"
#include "KuhbrilleCamera.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"

UViewportRenderingBPLibrary::UViewportRenderingBPLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UViewportRenderingBPLibrary::SetViewportRenderingEnabled(bool bEnable)
{
    if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
    {
        ViewportClient->bDisableWorldRendering = !bEnable;
    }
}