#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ATileBase.generated.h"

UCLASS()
class HOMEWORK3_API ATileBase : public AActor
{
    GENERATED_BODY()

public:
    ATileBase();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* BaseMesh;

    UPROPERTY(EditAnywhere)
    UInstancedStaticMeshComponent* InstancedMesh;

    virtual void OnConstruction(const FTransform& Transform) override;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};