#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SceneCaptureComponent.h>

#include "KuhbrilleManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class KUHBRILLECAMERA_API AKuhbrilleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKuhbrilleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACameraActor* CameraActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneCaptureComponentCube* CaptureComponentCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RenderTargetSizeX = 512;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextureRenderTargetCube* RenderTarget;
};
