// Fill out your copyright notice in the Description page of Project Settings.

#include "Fournoid.h"
#include "SpawnVolume.h"
#include "EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
: SpawnIntervalRangeLo(2.0f), SpawnIntervalRangeHi(6.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialize the spawn component.
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = SpawnBox;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	registerTimer();
}

// Called every frame
void ASpawnVolume::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector Origin = SpawnBox->Bounds.Origin;
	FVector Extent = SpawnBox->Bounds.BoxExtent;
	
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

void ASpawnVolume::SpawnEnemy()
{
	if (EnemyToSpawn) {
		UWorld* const World = GetWorld();
		if (World) {
			FVector SpawnLocation = GetRandomPointInVolume();
			World->SpawnActor<AEnemyCharacter>(EnemyToSpawn, SpawnLocation, FRotator::ZeroRotator);
        	registerTimer();
			UE_LOG(Fournoid, Log, TEXT("Spawned"));
		}
	}
}

void ASpawnVolume::registerTimer()
{
	SpawnInterval = FMath::FRandRange(SpawnIntervalRangeLo, SpawnIntervalRangeHi);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnEnemy, SpawnInterval, false);
}