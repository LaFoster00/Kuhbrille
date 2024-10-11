#include "KuhbrilleManager.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/TextureRenderTargetCube.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"


// Sets default values
AKuhbrilleManager::AKuhbrilleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(
		TEXT("/KuhbrilleCamera/KuhbrilleOverlay.KuhbrilleOverlay"));
	if (MaterialFinder.Succeeded())
	{
		KuhbrilleOverlayMaterial = MaterialFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTextureRenderTargetCube> LowResCubeFinder(
		TEXT("/KuhbrilleCamera/KuhbrilleLowRes.KuhbrilleLowRes"));
	if (LowResCubeFinder.Succeeded())
	{
		RenderTarget = LowResCubeFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> LutFinder(
		TEXT("/KuhbrilleCamera/deuteranope_lut.deuteranope_lut"));
	if (LutFinder.Succeeded())
	{
		RedGreenBlindLut = LutFinder.Object;
	}

	PostProcessSettings.MotionBlurAmount = 0.f;
	PostProcessSettings.bOverride_MotionBlurAmount = true;

	PostProcessSettings.BloomMethod = BM_SOG;
	PostProcessSettings.bOverride_BloomMethod = true;
	PostProcessSettings.BloomIntensity = 8.0f;
	PostProcessSettings.bOverride_BloomIntensity = true;
	PostProcessSettings.BloomThreshold = -0.935f;
	PostProcessSettings.bOverride_BloomThreshold = true;
	PostProcessSettings.BloomSizeScale = 64.0f;
	PostProcessSettings.bOverride_BloomSizeScale = true;
	PostProcessSettings.Bloom1Size = 0.89f;
	PostProcessSettings.bOverride_Bloom1Size = true;
	PostProcessSettings.Bloom2Size = 1.6f;
	PostProcessSettings.bOverride_Bloom2Size = true;
	PostProcessSettings.Bloom3Size = 6.53f;
	PostProcessSettings.bOverride_Bloom3Size = true;

	PostProcessSettings.AutoExposureMethod = AEM_Histogram;
	PostProcessSettings.bOverride_AutoExposureMethod = true;
	PostProcessSettings.AutoExposureBias = 0.0f;
	PostProcessSettings.bOverride_AutoExposureBias = true;
	PostProcessSettings.AutoExposureMinBrightness = -10.0f;
	PostProcessSettings.bOverride_AutoExposureMinBrightness = true;
	PostProcessSettings.AutoExposureMaxBrightness = 20.0f;
	PostProcessSettings.bOverride_AutoExposureMaxBrightness = true;
	PostProcessSettings.AutoExposureSpeedUp = 0.3f;
	PostProcessSettings.bOverride_AutoExposureSpeedUp = true;
	PostProcessSettings.AutoExposureSpeedDown = 0.3f;
	PostProcessSettings.bOverride_AutoExposureSpeedDown = true;

	PostProcessSettings.LocalExposureHighlightContrastScale = 1.0f;
	PostProcessSettings.bOverride_LocalExposureHighlightContrastScale = true;
	PostProcessSettings.LocalExposureShadowContrastScale = 1.0f;
	PostProcessSettings.bOverride_LocalExposureShadowContrastScale = true;
	PostProcessSettings.LocalExposureDetailStrength = 1.5;
	//PostProcessSettings.bOverride_LocalExposureDetailStrength = true;
	PostProcessSettings.LocalExposureBlurredLuminanceBlend = 1.0f;
	//PostProcessSettings.bOverride_LocalExposureBlurredLuminanceBlend = true;
	PostProcessSettings.LocalExposureBlurredLuminanceKernelSizePercent = 100.0f;
	//PostProcessSettings.bOverride_LocalExposureBlurredLuminanceKernelSizePercent = true;

	PostProcessSettings.ColorGradingLUT = RedGreenBlindLut;
	PostProcessSettings.bOverride_ColorGradingLUT = true;

	PostProcessSettings.FilmToe = 0.8f;
	PostProcessSettings.bOverride_FilmToe = true;
}

// Called when the game starts or when spawned
void AKuhbrilleManager::BeginPlay()
{
	Super::BeginPlay();

	if (KuhbrilleOverlayMaterial->IsValidLowLevel())
		PostProcessSettings.AddBlendable(KuhbrilleOverlayMaterial, 1);

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	auto Pawn = PlayerController->GetPawn();
	Camera = Pawn->GetComponentByClass<UCameraComponent>();

	//Camera->bUsePawnControlRotation = false;
	Camera->SetConstraintAspectRatio(false);
	Camera->SetFieldOfView(121.0f);
	Camera->bOverrideAspectRatioAxisConstraint = true;
	Camera->AspectRatioAxisConstraint = AspectRatio_MaintainYFOV;

	CaptureComponentCube = NewObject<USceneCaptureComponentCube>(this, USceneCaptureComponentCube::StaticClass());

	if (CaptureComponentCube)
	{
		auto playerRoot = PlayerController->GetPawn()->GetComponentByClass<UCapsuleComponent>();
		CaptureComponentCube->SetupAttachment(playerRoot);
		CaptureComponentCube->RegisterComponent();
		CaptureComponentCube->SetRelativeLocation(Camera->GetRelativeLocation());
		CaptureComponentCube->bCaptureRotation = true;
		CaptureComponentCube->bCaptureEveryFrame = !ManualCaptureUpdate;
		CaptureComponentCube->bUseRayTracingIfEnabled = true;
		CaptureComponentCube->TextureTarget = RenderTarget;
		CaptureComponentCube->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
		CaptureComponentCube->CaptureSource = SCS_SceneColorSceneDepth;
	}

	PostProcessVolume = GetWorld()->SpawnActor<APostProcessVolume>();
	PostProcessVolume->Settings = PostProcessSettings;
	PostProcessVolume->bUnbound = true;
}

void AKuhbrilleManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ConstrainPawnCameraZAxis)
		ConstrainPawnCameraToZAxis();
}

void AKuhbrilleManager::ConstrainPawnCameraToZAxis()
{
	FRotator CurrentRotation = PlayerController->GetPawn()->GetControlRotation();
	CurrentRotation.Pitch = 0.0f;
	CurrentRotation.Roll = 0.0f;
	Camera->SetWorldRotation(CurrentRotation);
}

void AKuhbrilleManager::EnableKuhbrille()
{
	SetKuhbrilleEnabled(true);
}

void AKuhbrilleManager::DisableKuhbrille()
{
	SetKuhbrilleEnabled(false);
}

void AKuhbrilleManager::SetKuhbrilleEnabled(bool Enabled)
{
	if (KuhbrilleEnabled == Enabled)
		return;
	KuhbrilleEnabled = Enabled;
	PostProcessVolume->bEnabled = Enabled;
}
