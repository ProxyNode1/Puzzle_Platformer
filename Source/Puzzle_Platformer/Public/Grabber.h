// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/NameTypes.h"
#include "Templates/SubclassOf.h" 	
#include "Engine/EngineTypes.h"
#include "Math/Vector.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLE_PLATFORMER_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		int32 ForceApplied = 200000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		int32 PickUpLimit = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		float Reach = 160.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		float PlayerHalfHeight = 160.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		float PlayerRadius = 160.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		FName TagForInteraction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		FName TagForMass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		UMaterialInterface* OnGrabMatParent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Variable")
		TSubclassOf<AActor>KeyClass;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FindPhysicsHandle();

	//FHitResult GetFirstObjectHit();
	void GetFirstObjectHit();

	FVector GetLineTraceStart();

	FVector GetLineTraceEnd();

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void Grab();

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void Drop();

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void Throw();

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		void SetGrabbed(bool Value);

	UFUNCTION(BlueprintCallable, Category = "Class Function")
		bool GetGrabbed();

	UFUNCTION()
		int32 GetPickUpLimit();

	UFUNCTION()
		float GetReach();

private:

	bool Grabbed = false;

	UPROPERTY()
		UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY()
		ACharacter* MyCharacter = nullptr;

	UPROPERTY()
		UCameraComponent* PlayerCam = nullptr;

	UPROPERTY()
		UMaterialInterface* DefaultMat = nullptr;

	bool IsActorHit = false;

	FHitResult HitResult;

	UFUNCTION()
		bool IsActorHitUnderPlayer();		
};
