// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Containers/Array.h"
#include "Templates/SubclassOf.h"

#include "SpawnableManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLE_PLATFORMER_API USpawnableManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USpawnableManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Class Variable")
		float CubeMass = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		TSubclassOf<class AActor> SpawnActorClass;

	UPROPERTY()
		TArray<AActor*> SpawnActorArray;

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		AActor* SpawnInteractables(UPrimitiveComponent* SpawnCollider);

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void DeleteExtraInteractables();

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void DestroySpecificInteractable(AActor* ActorToDestory);

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		int GetArrayLength();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};