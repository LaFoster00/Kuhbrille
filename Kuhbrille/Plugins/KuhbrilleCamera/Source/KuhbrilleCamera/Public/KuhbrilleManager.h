#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"

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

	virtual void Tick(float DeltaSeconds) override;

	void ConstrainPawnCameraToZAxis();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	USceneCaptureComponentCube* CaptureComponentCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	int RenderTargetSizeX = 512;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	UTextureRenderTargetCube* RenderTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	bool ManualCaptureUpdate = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	UMaterial* KuhbrilleOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	UTexture2D* RedGreenBlindLut;
};
