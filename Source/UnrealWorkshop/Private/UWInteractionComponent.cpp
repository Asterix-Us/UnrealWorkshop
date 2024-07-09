// Fill out your copyright notice in the Description page of Project Settings.


#include "UWInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "UWGameplayInterface.h"

UUWInteractionComponent::UUWInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUWInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUWInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUWInteractionComponent::Interact()
{
	// Sphere trace parameters
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.f);

	float Radius = 30.f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	TArray<FHitResult> Hits;

	// Sphere trace
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Check for possible hits
	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			// Check if the Actor implements the required interface
			if (HitActor->Implements<UUWGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
			
				IUWGameplayInterface::Execute_Interact(HitActor, MyPawn);
			}
		}

		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0.f, 2.f);
}
