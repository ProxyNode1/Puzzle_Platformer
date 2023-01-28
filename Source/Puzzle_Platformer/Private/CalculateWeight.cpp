// Fill out your copyright notice in the Description page of Project Settings.

#include "CalculateWeight.h"

#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"


#define OUT


// Sets default values for this component's properties
UCalculateWeight::UCalculateWeight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCalculateWeight::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCalculateWeight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


float UCalculateWeight::AddMass(UPrimitiveComponent* Collider, FName ComponentsTag)
{
	CalculatedMass = 0.f;
	
	//get overlapping actors, set it to an array
	Collider->GetOverlappingActors(OUT OverlappingActorArray, TSubclassOf<AActor>());

	//go over all the overlapping actors
	for (int32 i = 0; i != OverlappingActorArray.Num(); ++i)
	{
		//get the actor component that has the tag.
		TArray<UActorComponent*>ComponentArray = OverlappingActorArray[i]->GetComponentsByTag(UStaticMeshComponent::StaticClass(), ComponentsTag);

		//UE_LOG(LogTemp, Warning, TEXT("Component Name: %s"), *(ComponentArray[0]->GetName()));

		//cast to static mesh component since the tagged component is a static mesh. 
		//get mass. add to CalculatedMass. 

		for (int32 j = 0; j != ComponentArray.Num(); ++j)
		{
			UStaticMeshComponent* StaticComponent = Cast<UStaticMeshComponent>(ComponentArray[j]);
			if (StaticComponent)
			{
				float Mass = StaticComponent->GetMass();

				//UE_LOG(LogTemp, Warning, TEXT("Mass: %f"), Mass);

				CalculatedMass += Mass;

				//UE_LOG(LogTemp, Warning, TEXT("CalculatedMass: %f"), CalculatedMass);
			}
		}
	}

	return CalculatedMass;
}

float UCalculateWeight::SubtractMass(AActor* ActorEndOverlap, FName ComponentsTag)
{
	int RemovingActorsindex = 0;

	//finding player that left from overlapping array
	if (OverlappingActorArray.Find(OUT ActorEndOverlap, RemovingActorsindex))
	{

		//get the actor component that has the tag.
		TArray<UActorComponent*>ComponentArray = OverlappingActorArray[RemovingActorsindex]->GetComponentsByTag(
			UStaticMeshComponent::StaticClass(), ComponentsTag
		);

		//cast to static mesh component. get mass. add to CalculatedMass. 
		for (int32 i = 0; i != ComponentArray.Num(); ++i)
		{
			UStaticMeshComponent* StaticComponent = Cast<UStaticMeshComponent>(ComponentArray[i]);
			if (StaticComponent)
			{
				CalculatedMass -= StaticComponent->GetMass();
			}
		}
	}


	return CalculatedMass;
}