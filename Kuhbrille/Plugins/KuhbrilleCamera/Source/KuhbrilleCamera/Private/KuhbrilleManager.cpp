#include "KuhbrilleManager.h"

#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/TextureRenderTargetCube.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKuhbrilleManager::AKuhbrilleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKuhbrilleManager::BeginPlay()
{
	Super::BeginPlay();

	auto location = FVector(0, 0, 1000);
	auto rotation = FRotator(90, 0, 0);
	CameraActor = GetWorld()->SpawnActor<ACameraActor>(location, rotation);

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetViewTarget(CameraActor);

	CaptureComponentCube = NewObject<USceneCaptureComponentCube>(this, USceneCaptureComponentCube::StaticClass());

	if (CaptureComponentCube)
	{
		RenderTarget = NewObject<UTextureRenderTargetCube>(this);
		RenderTarget->bHDR = true;
		RenderTarget->InitAutoFormat(RenderTargetSizeX);  // Set the resolution for the render target (e.g., 512x512)

		auto playerRoot = PlayerController->GetPawn()->GetComponentByClass<UCapsuleComponent>();
		CaptureComponentCube->SetupAttachment(playerRoot);
		CaptureComponentCube->RegisterComponent();
		CaptureComponentCube->bCaptureRotation = true;
		CaptureComponentCube->bCaptureEveryFrame = true;
		CaptureComponentCube->bUseRayTracingIfEnabled = true;
		CaptureComponentCube->TextureTarget = RenderTarget;
		CaptureComponentCube->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
		CaptureComponentCube->CaptureSource = SCS_SceneColorSceneDepth;
	}
}
