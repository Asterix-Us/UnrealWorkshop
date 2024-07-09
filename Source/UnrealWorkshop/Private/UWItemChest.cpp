// Fill out your copyright notice in the Description page of Project Settings.


#include "UWItemChest.h"

// Sets default values
AUWItemChest::AUWItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(BaseMesh);
}

void AUWItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(110.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AUWItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUWItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

