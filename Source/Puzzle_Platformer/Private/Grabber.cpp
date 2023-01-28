// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Kismet/KismetSystemLibrary.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();


	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!MyCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Character Not Found!"));
	}
	else
	{
		PlayerCam = MyCharacter->FindComponentByClass<UCameraComponent>();
		if (!PlayerCam)
		{
			UE_LOG(LogTemp, Error, TEXT("Camera Not Found!"));
		}
	}

}


void UGrabber::FindPhysicsHandle() //find "PhysicsHandleComponent" added in BP Class
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Handle Not Found!"));
	}

}


void UGrabber::Grab()
{
	GetFirstObjectHit();

	if (IsActorHit && !IsActorHitUnderPlayer())
	{
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		AActor* Actor = HitResult.GetActor();

		if (ComponentToGrab) //if (ComponentToGrab && ComponentToGrab->ComponentHasTag(TagForMass))
		{
			if (ComponentToGrab->GetMass() <= PickUpLimit)
			{
				if (!PhysicsHandle) return;

				if (Actor)
				{
					DefaultMat = ComponentToGrab->GetMaterial(0);

					if (OnGrabMatParent->IsValidLowLevel())
					{
						FLinearColor Color;
						UMaterialInstanceDynamic* GrabbedMat = UMaterialInstanceDynamic::Create(OnGrabMatParent, this);

						ComponentToGrab->SetMaterial(0, OnGrabMatParent);

						/*
						if (DefaultMat->GetVectorParameterValue(FMaterialParameterInfo("Color"), OUT Color))
						{
							//UE_LOG(LogTemp, Warning, TEXT("Grabbed Mesh Color: %s"), *Color.ToString());

							GrabbedMat->SetVectorParameterValue(FName("Base Color"), Color);
							
							ComponentToGrab->SetMaterial(0, GrabbedMat);

							GrabbedMat = nullptr;
						}
						*/
					}


					if (!(ComponentToGrab->IsSimulatingPhysics()))
					{
						ComponentToGrab->SetSimulatePhysics(1);
					}


					PhysicsHandle->GrabComponentAtLocationWithRotation
					(
						ComponentToGrab,
						NAME_None,
						ComponentToGrab->GetOwner()->GetActorLocation(),
						ComponentToGrab->GetOwner()->GetActorRotation()
					);


					UE_LOG(LogTemp, Warning, TEXT("Component Found: %s"), *(ComponentToGrab->GetName()));

					Grabbed = 1;

					UE_LOG(LogTemp, Warning, TEXT("Grabbed is: %d"), Grabbed);
				}
			}
		}
	}
}


void UGrabber::Drop()
{
	if (Grabbed)
	{
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

		if (ComponentToGrab && DefaultMat)
		{
			ComponentToGrab->SetMaterial(0, DefaultMat);
		}

		PhysicsHandle->ReleaseComponent();
		Grabbed = 0;

		UE_LOG(LogTemp, Warning, TEXT("Grabbed is: %d"), Grabbed);
	}
}


void UGrabber::Throw()
{
	if (IsActorHit)
	{
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		AActor* Actor = HitResult.GetActor();

		if (Actor)
		{
			if (Grabbed)
			{
				Drop();

				if (DefaultMat)
				{
					ComponentToGrab->SetMaterial(0, DefaultMat);
				}

				ComponentToGrab->AddImpulse((PlayerCam->GetForwardVector() * ForceApplied), NAME_None, true);
			}
		}
	}
}


void UGrabber::GetFirstObjectHit()
{
	//Crete Query for Collision
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	IsActorHit = GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetLineTraceStart(), GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	if (HitResult.Actor.IsValid())
		UE_LOG(LogTemp, Warning, TEXT("ActorHit: %s"), *HitResult.GetActor()->GetName());
}


bool UGrabber::IsActorHitUnderPlayer()
{
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult ThisHitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT ThisHitResult, GetLineTraceStart(),
		GetLineTraceStart() + (GetOwner()->GetActorUpVector() * -200),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	/*UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(), GetLineTraceStart(),
		GetLineTraceStart() + (GetOwner()->GetActorUpVector() * -200), 50,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),*/



	if (ThisHitResult.bBlockingHit && ThisHitResult.GetActor() == HitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *ThisHitResult.GetActor()->GetName());

		return true;
	}

	return false;
}


FVector UGrabber::GetLineTraceStart()
{

	FVector PlayerViewPointLocation;

	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	PlayerViewPointLocation = PlayerCam->GetComponentLocation();

	return PlayerViewPointLocation;
}


FVector UGrabber::GetLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	PlayerViewPointLocation = PlayerCam->GetComponentLocation();
	PlayerViewPointRotation = PlayerCam->GetComponentRotation();

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}


void UGrabber::SetGrabbed(bool Value)
{
	Grabbed = Value;
}


bool UGrabber::GetGrabbed()
{
	return Grabbed;
}

int32 UGrabber::GetPickUpLimit()
{
	return PickUpLimit;
}

float UGrabber::GetReach()
{
	return Reach;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}*/
}