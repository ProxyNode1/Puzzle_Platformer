// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

#include "Movable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLE_PLATFORMER_API UMovable : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMovable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int32 Speed = 4; //4 is default value

	bool bPlatformAtA = true;

	bool bIsMovingAhead = false;

	int DirectionToMove = 1;

	bool bPlayerAtA = false;
	bool bPlayerAtB = false;


	int32 MaxTime = 0;
	int32 CurrentTime = 0;


	//-------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void SetSpeed(float val);

	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		void setPlatformAtA(bool Val);
	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		bool getPlatformAtA();

	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		void setPlayerAtA(bool Val);
	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		bool getPlayerAtA();

	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		void setPlayerAtB(bool Val);
	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		bool getPlayerAtB();

	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		bool getIsMovingAhead();

	UFUNCTION(BlueprintCallable, Category = "Getters/Setters")
		int getDirectionToMove();
	//---------------------------------------------------


	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void CalculateTime(UPrimitiveComponent* ObjectA, UPrimitiveComponent* ObjectB);

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void Switcher();

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void PerformAnimation(UStaticMeshComponent* Platform);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};