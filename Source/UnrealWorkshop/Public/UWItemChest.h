// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UWGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "UWItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNREALWORKSHOP_API AUWItemChest : public AActor, public IUWGameplayInterface
{
	GENERATED_BODY()
	
public:	
	AUWItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
