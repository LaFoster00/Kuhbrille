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
}

// Called when the game starts or when spawned
void AKuhbrilleManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	auto Pawn = PlayerController->GetPawn();
	Camera = Pawn->GetComponentByClass<UCameraComponent>();

	FPostProcessSettings& pp = Camera->PostProcessSettings;

	if (KuhbrilleOverlayMaterial->IsValidLowLevel())
	{
		pp.AddBlendable(KuhbrilleOverlayMaterial, 1.0f);
	}

	Camera->bUsePawnControlRotation = false;
	Camera->SetConstraintAspectRatio(false);
	Camera->SetFieldOfView(121.0f);
	Camera->bOverrideAspectRatioAxisConstraint = true;
	Camera->AspectRatioAxisConstraint = AspectRatio_MaintainYFOV;

	pp.MotionBlurAmount = 0.f;
	pp.bOverride_MotionBlurAmount = true;

	pp.BloomMethod = BM_SOG;
	pp.bOverride_BloomMethod = true;
	pp.BloomIntensity = 8.0f;
	pp.bOverride_BloomIntensity = true;
	pp.BloomThreshold = -0.935f;
	pp.bOverride_BloomThreshold = true;
	pp.BloomSizeScale = 64.0f;
	pp.bOverride_BloomSizeScale = true;
	pp.Bloom1Size = 0.89f;
	pp.bOverride_Bloom1Size = true;
	pp.Bloom2Size = 1.6f;
	pp.bOverride_Bloom2Size = true;
	pp.Bloom3Size = 6.53f;
	pp.bOverride_Bloom3Size = true;

	pp.AutoExposureMethod = AEM_Histogram;
	pp.bOverride_AutoExposureMethod = true;
	pp.AutoExposureBias = 0.0f;
	pp.bOverride_AutoExposureBias = true;
	pp.AutoExposureMinBrightness = -10.0f;
	pp.bOverride_AutoExposureMinBrightness = true;
	pp.AutoExposureMaxBrightness = 20.0f;
	pp.bOverride_AutoExposureMaxBrightness = true;
	pp.AutoExposureSpeedUp = 0.3f;
	pp.bOverride_AutoExposureSpeedUp = true;
	pp.AutoExposureSpeedDown = 0.3f;
	pp.bOverride_AutoExposureSpeedDown = true;

	pp.LocalExposureHighlightContrastScale = 1.0f;
	pp.bOverride_LocalExposureHighlightContrastScale = true;
	pp.LocalExposureShadowContrastScale = 1.0f;
	pp.bOverride_LocalExposureShadowContrastScale = true;
	pp.LocalExposureDetailStrength = 1.5;
	//pp.bOverride_LocalExposureDetailStrength = true;
	pp.LocalExposureBlurredLuminanceBlend = 1.0f;
	//pp.bOverride_LocalExposureBlurredLuminanceBlend = true;
	pp.LocalExposureBlurredLuminanceKernelSizePercent = 100.0f;
	//pp.bOverride_LocalExposureBlurredLuminanceKernelSizePercent = true;

	pp.bOverride_ColorGradingLUT = true;
	pp.ColorGradingLUT = RedGreenBlindLut;

	pp.FilmToe = 0.8f;
	pp.bOverride_FilmToe = true;

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
}

void AKuhbrilleManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ConstrainPawnCameraToZAxis();
}

void AKuhbrilleManager::ConstrainPawnCameraToZAxis()
{
	FRotator CurrentRotation = PlayerController->GetPawn()->GetControlRotation();
	CurrentRotation.Pitch = 0.0f;
	CurrentRotation.Roll = 0.0f;
	Camera->SetWorldRotation(CurrentRotation);
}
