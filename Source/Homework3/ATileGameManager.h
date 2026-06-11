#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATileBase.h"
#include "ATileGameManager.generated.h"

#define MAX_MAP_GRID_SIZE 100

UCLASS()
class HOMEWORK3_API ATileGameManager : public AActor
{
    GENERATED_BODY()

public:
    ATileGameManager();

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* GridSelection;

    void OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed);

    UPROPERTY(EditAnywhere)
    int GridSize;

    UPROPERTY(EditAnywhere)
    FVector GridOffset;

    UPROPERTY(EditAnywhere)
    TArray<ATileBase*> TileTypes;

    UPROPERTY(EditAnywhere)
    int MapExtendsInGrids;

    // Moved above the massive Map array to prevent memory offset limits (C4309)
    UPROPERTY(EditAnywhere)
    FRotator CurrentTileRotation;

    int CurrentTileIndex;

    // Massive arrays should always be at the bottom of the class declarations
    ATileBase* Map[MAX_MAP_GRID_SIZE][MAX_MAP_GRID_SIZE];

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};