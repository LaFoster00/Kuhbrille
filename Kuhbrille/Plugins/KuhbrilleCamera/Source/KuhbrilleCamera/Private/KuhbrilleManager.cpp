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
	FPostProcessSettings &pp = CameraActor->GetCameraComponent()->PostProcessSettings;
	
	if (KuhbrilleOverlayMaterial->IsValidLowLevel())
	{
		pp.AddBlendable(KuhbrilleOverlayMaterial, 1.0f);
	}

	pp.BloomMethod = BM_SOG;
	pp.BloomIntensity = 8.0f;
	pp.BloomThreshold = -0.935f;
	pp.BloomSizeScale = 64.0f;
	pp.Bloom1Size = 0.89f;
	pp.Bloom2Size = 1.6f;
	pp.Bloom3Size = 6.53f;

	pp.AutoExposureMethod = AEM_Histogram;
	pp.AutoExposureBias = 0.0f;
	pp.AutoExposureMinBrightness = -10.0f;
	pp.AutoExposureMaxBrightness = 20.0f;
	pp.AutoExposureSpeedUp = 0.3f;
	pp.AutoExposureSpeedDown = 0.3f;

	pp.LocalExposureHighlightContrastScale = 1.0f;
	pp.LocalExposureShadowContrastScale = 1.0f;
	pp.LocalExposureDetailStrength = 1.5;
	pp.LocalExposureBlurredLuminanceBlend = 1.0f;
	pp.LocalExposureBlurredLuminanceKernelSizePercent = 100.0f;

	pp.FilmToe = 0.8f;


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
