#include "KuhbrilleManager.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/TextureRenderTargetCube.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKuhbrilleManager::AKuhbrilleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/KuhbrilleCamera/KuhbrilleOverlay.KuhbrilleOverlay"));
	if (MaterialFinder.Succeeded())
	{
		KuhbrilleOverlayMaterial = MaterialFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTextureRenderTargetCube> LowResCubeFinder(TEXT("/KuhbrilleCamera/KuhbrilleLowRes.KuhbrilleLowRes"));
	if (LowResCubeFinder.Succeeded())
	{
		RenderTarget = LowResCubeFinder.Object;
	}
}

// Called when the game starts or when spawned
void AKuhbrilleManager::BeginPlay()
{
	Super::BeginPlay();

	auto location = FVector(0, 0, 1000);
	auto rotation = FRotator(90, 0, 0);
	CameraActor = GetWorld()->SpawnActor<ACameraActor>(location, rotation);

	if (KuhbrilleOverlayMaterial->IsValidLowLevel())
	{
		CameraActor->GetCameraComponent()->PostProcessSettings.AddBlendable(KuhbrilleOverlayMaterial, 1.0f);
	}

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetViewTarget(CameraActor);

	CaptureComponentCube = NewObject<USceneCaptureComponentCube>(this, USceneCaptureComponentCube::StaticClass());

	if (CaptureComponentCube)
	{
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
