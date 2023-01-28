// Fill out your copyright notice in the Description page of Project Settings.

#include "Tracer.h"

#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"	




// Sets default values for this component's properties
UTracer::UTracer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UTracer::BeginPlay()
{
	Super::BeginPlay();

	Grabber = GetOwner()->FindComponentByClass<UGrabber>();

	if (!Grabber)
		UE_LOG(LogTemp, Warning, TEXT("Grabber Not Found"))

		PlayerCamera = GetOwner()->FindComponentByClass<UCameraComponent>();

	if (!PlayerCamera)
		UE_LOG(LogTemp, Warning, TEXT("Camera Not Found"))
}


void UTracer::TraceForInteractives()
{
	if (Grabber->GetGrabbed()) // In case the player is holding an object
		return;
	
	SetTraceStart();
	SetTraceEnd();

	FCollisionQueryParams QueryParams(FName(TEXT("MyTraceTag")), false, GetOwner());

	//GetWorld()->LineTraceSingleByChannel(OUT TracerHitResult, TraceStart, TraceEnd, TraceChannel, QueryParams);

	GetWorld()->LineTraceSingleByObjectType(OUT TracerHitResult, TraceStart, TraceEnd,
	FCollisionObjectQueryParams(ObjectType), QueryParams); //Trace Objects by provided objects in ObjectTypes
	
	/*if(TracerHitResult.bBlockingHit)
		UE_LOG(LogTemp, Warning, TEXT("HitComponent: %s"), *(TracerHitResult.GetComponent()->GetName()))
	*/

	if (DrawDBLine)
	{
		if (TracerHitResult.bBlockingHit) DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.f);

		if (!TracerHitResult.bBlockingHit) DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5.f);
	}

	//return TraceHitResult
}


bool UTracer::ShouldAllowInteraction()
{
	if (Grabber->GetGrabbed()) // In case the player is holding an object
		return false;

	//UE_LOG(LogTemp, Warning, TEXT("Object Type: %d"), TracerHitResult.GetComponent()->GetCollisionObjectType())

	if (TracerHitResult.GetComponent()->GetCollisionObjectType() == ECC_PhysicsBody) // In case the traced object is a physics object
	{
		//Check if the PhysicsBody's Mass is Over PickUpLimit
		if (TracerHitResult.GetComponent()->GetMass() > Grabber->GetPickUpLimit())
			return false;


		FCollisionQueryParams QueryParams(FName(TEXT("")), false, GetOwner());

		FHitResult HitResultBelowPlayer;

		// Check if the PhysicsBody Is Under The Player
		GetWorld()->LineTraceSingleByObjectType(OUT HitResultBelowPlayer, TraceStart,
			TraceStart + (GetOwner()->GetActorUpVector() * (-200)),
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), QueryParams);

		if (HitResultBelowPlayer.GetActor() == TracerHitResult.GetActor() && HitResultBelowPlayer.bBlockingHit)
			return false;
	}

	//For when the interactive objects like button shouldn't be allowed to interact with
	if (TracerHitResult.GetComponent()->GetCollisionObjectType() == ObjectType[0]) //is an interactive
	{	}

	return true;
}

void UTracer::SetObjectType(TArray<TEnumAsByte<EObjectTypeQuery>> Value)
{
	ObjectType = Value;

	//UE_LOG(LogTemp, Warning, TEXT("TraceChannel: %d"), TraceChannel);
}

void UTracer::SetTraceStart()
{
	if (!PlayerCamera) return;

	TraceStart = PlayerCamera->GetComponentLocation();
}


void UTracer::SetTraceEnd()
{
	if (!Grabber || !PlayerCamera) return;

	TraceEnd = TraceStart + PlayerCamera->GetForwardVector() * Grabber->GetReach();
}

// Called every frame
void UTracer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}