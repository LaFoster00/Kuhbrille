#include "ViewportRenderingManager.h"
#include "ViewportRenderingBPLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AViewportRenderingManager::AViewportRenderingManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AViewportRenderingManager::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate FindCapturesDelegate;
	FindCapturesDelegate.BindUObject(this, &AViewportRenderingManager::FindCaptures);
	
	if (GEngine)
	{
		UWorld* world = GEngine->GetWorldFromContextObjectChecked(this);
		if (world)
		{
			FTimerHandle FindCapturesTimerHandle;
			world->GetTimerManager().SetTimer(FindCapturesTimerHandle, FindCapturesDelegate, 1.0f, false);
		}
	}
}

void AViewportRenderingManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UViewportRenderingBPLibrary::SetViewportRenderingEnabled(true);
}

// Called every frame
void AViewportRenderingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDisableOnPlay)
	{
		for (auto twoD : Capture2Ds)
		{
			if (twoD->IsValidLowLevel())
				twoD->CaptureScene();
		}

		for (auto cube : CaptureCubes)
		{
			if (cube->IsValidLowLevel())
				cube->CaptureScene();
		}
	}
}

void AViewportRenderingManager::FindCaptures()
{
	if (bDisableOnPlay)
	{
		UViewportRenderingBPLibrary::SetViewportRenderingEnabled(false);
		if (bFindSceneCaptures)
		{
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), Actors);

			for (const auto Actor : Actors)
			{
				{
					TArray<USceneCaptureComponentCube*> cubes;
					Actor->GetComponents(USceneCaptureComponentCube::StaticClass(), cubes);
					for (auto Component : cubes)
					{
						if (!Component->bCaptureEveryFrame)
							continue;
						CaptureCubes.AddUnique(Component);
						Component->bCaptureEveryFrame = false;
					}
				}

				{
					TArray<USceneCaptureComponent2D*> twoDs;
					Actor->GetComponents(USceneCaptureComponent2D::StaticClass(), twoDs);
					for (auto Component : twoDs)
					{
						if (!Component->bCaptureEveryFrame)
							continue;
						Capture2Ds.AddUnique(Component);
						Component->bCaptureEveryFrame = false;
					}
				}
			}
		}
	}
}
