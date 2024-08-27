#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SceneCaptureComponent.h>

#include "ViewportRenderingManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class KUHBRILLECAMERA_API AViewportRenderingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViewportRenderingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void FindCaptures();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDisableOnPlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFindSceneCaptures = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USceneCaptureComponent2D*> Capture2Ds;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USceneCaptureComponentCube*> CaptureCubes;
};
