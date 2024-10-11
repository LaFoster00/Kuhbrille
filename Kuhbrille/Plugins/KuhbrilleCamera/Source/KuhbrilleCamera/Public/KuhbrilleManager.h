#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/TextureRenderTargetCube.h"
#include "Materials/Material.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/Texture2D.h"

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

private:
	UFUNCTION(CallInEditor, Category="Config")
	void EnableKuhbrille();

	UFUNCTION(CallInEditor, Category="Config")
	void DisableKuhbrille();
	
public:
	UFUNCTION(BlueprintCallable, Category="Kuhbrille")
	void SetKuhbrilleEnabled(bool Enabled);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	bool KuhbrilleEnabled = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool ConstrainPawnCameraZAxis = true;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	APostProcessVolume* PostProcessVolume;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config", meta=(ShowPostProcessCategories))
	FPostProcessSettings PostProcessSettings;
};
