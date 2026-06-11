#include "ATileGameManager.h"
#include "ATilePlayerController.h"
#include "GameFramework/PlayerInput.h"


ATileGameManager::ATileGameManager() : GridSize(100), GridOffset(0, 0, 0.5f), MapExtendsInGrids(25), CurrentTileIndex(0)
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
    GridSelection->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

    if (PlaneMesh.Succeeded())
    {
        GridSelection->SetStaticMesh(PlaneMesh.Object);
    }

    if (GridMaterial.Succeeded())
    {
        GridSelection->SetMaterial(0, GridMaterial.Object);
    }

    GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATileGameManager::BeginPlay()
{
    Super::BeginPlay();

    if (auto PlayerController = Cast<ATilePlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        PlayerController->GameManager = this;
    }
}

void ATileGameManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
    // Return if there are no tile types defined in the Editor
    if (TileTypes.Num() == 0) return;

    // Calculate the snapped grid location
    FVector GridLoc = GridOffset;
    GridLoc.X += FMath::GridSnap(Location.X, GridSize);
    GridLoc.Y += FMath::GridSnap(Location.Y, GridSize);
    GridLoc.Z += Location.Z;

    GridSelection->SetWorldLocation(GridLoc);

    // HOMEWORK 4 - TASK 2: Preview the currently selected tile mesh
    if (TileTypes.IsValidIndex(CurrentTileIndex) && TileTypes[CurrentTileIndex]->BaseMesh)
    {
        GridSelection->SetStaticMesh(TileTypes[CurrentTileIndex]->BaseMesh);
    }

    UPlayerInput* Input = GetWorld()->GetFirstPlayerController()->PlayerInput;

    // LEFT MOUSE BUTTON: Place the tile
    if (Input->WasJustPressed(EKeys::LeftMouseButton))
    {
        // Convert world location to array indices
        int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
        int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

        // Prevent placing tiles outside the map boundaries
        if (GridX < 0 || GridY < 0 || GridX >= MapExtendsInGrids * 2 || GridY >= MapExtendsInGrids * 2)
            return;

        // Check if a tile already exists at this location
        if (Map[GridX][GridY] != nullptr)
            return;

        if (TileTypes.IsValidIndex(CurrentTileIndex))
        {
            ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
            Map[GridX][GridY] = SelectedTile;

            // Add instance to the map using the current rotation and location
            FTransform TileTransform(CurrentTileRotation, GridLoc + GridOffset, FVector(1.0f));
            SelectedTile->InstancedMesh->AddInstance(SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform, true);
        }
    }
    // HOMEWORK 4 - TASK 3: RIGHT MOUSE BUTTON: Rotate tile 90 degrees (Yaw)
    else if (Input->WasJustPressed(EKeys::RightMouseButton))
    {
        CurrentTileRotation.Yaw += 90.0f;
        // Update the preview mesh rotation
        GridSelection->SetWorldRotation(CurrentTileRotation);
    }
    // MOUSE SCROLL DOWN: Cycle to the next tile
    else if (Input->WasJustPressed(EKeys::MouseScrollDown))
    {
        CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();

        // Reset rotation when changing tiles
        CurrentTileRotation = FRotator::ZeroRotator;
        GridSelection->SetWorldRotation(CurrentTileRotation);
    }
    // HOMEWORK 4 - TASK 4: MOUSE SCROLL UP: Cycle to the previous tile
    else if (Input->WasJustPressed(EKeys::MouseScrollUp))
    {
        CurrentTileIndex--;
        // Wrap around to the end of the array if index goes below zero
        if (CurrentTileIndex < 0)
        {
            CurrentTileIndex = TileTypes.Num() - 1;
        }

        // Reset rotation when changing tiles
        CurrentTileRotation = FRotator::ZeroRotator;
        GridSelection->SetWorldRotation(CurrentTileRotation);
    }
}