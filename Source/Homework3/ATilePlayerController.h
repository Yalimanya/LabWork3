#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ATilePlayerController.generated.h"

class ATileGameManager;

UCLASS()
class HOMEWORK3_API ATilePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    ATileGameManager* GameManager;
};