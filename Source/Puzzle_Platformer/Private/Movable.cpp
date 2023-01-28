// Fill out your copyright notice in the Description page of Project Settings.

#include "Movable.h"

#include "Components/StaticMeshComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UMovable::UMovable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMovable::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UMovable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//-------------------- getters and setters
void UMovable::setPlatformAtA(bool Val)
{
	bPlatformAtA = Val;
}

bool UMovable::getPlatformAtA()
{
	return bPlatformAtA;
}

void UMovable::setPlayerAtA(bool Val)
{
	bPlayerAtA = Val;
}

bool UMovable::getPlayerAtA()
{
	return bPlayerAtA;
}

void UMovable::setPlayerAtB(bool Val)
{
	bPlayerAtB = Val;
}

bool UMovable::getPlayerAtB()
{
	return bPlayerAtB;
}

bool UMovable::getIsMovingAhead()
{
	return bIsMovingAhead;
}

int UMovable::getDirectionToMove()
{
	return DirectionToMove;
}

void UMovable::SetSpeed(float val)
{
	Speed = val;
}
//-----------------------

void UMovable::CalculateTime(UPrimitiveComponent* ObjectA, UPrimitiveComponent* ObjectB)
{
	MaxTime = UKismetMathLibrary::Abs_Int(UKismetMathLibrary::FTrunc
	(
		(ObjectB->GetRelativeTransform().GetLocation().Z) - (ObjectA->GetRelativeTransform().GetLocation().Z)
	) / Speed);

}

void UMovable::Switcher()
{

	if (bPlatformAtA && bPlayerAtA)
	{
		DirectionToMove = 1;
		bIsMovingAhead = true;

		UE_LOG(LogTemp, Warning, TEXT("Direction = %d, bIncrease = %s"),
			DirectionToMove, (bIsMovingAhead ? TEXT("true") : TEXT("false")));
	}

	else
	{
		if (!bPlatformAtA && bPlayerAtA)
		{
			DirectionToMove = -1;
			bIsMovingAhead = false;

			UE_LOG(LogTemp, Warning, TEXT("Direction = %d, bIncrease = %s"),
				DirectionToMove, (bIsMovingAhead ? TEXT("true") : TEXT("false")));
		}

		else
		{
			if (bPlatformAtA && bPlayerAtB)
			{
				DirectionToMove = 1;
				bIsMovingAhead = true;

				UE_LOG(LogTemp, Warning, TEXT("Direction = %d, bIncrease = %s"),
					DirectionToMove, (bIsMovingAhead ? TEXT("true") : TEXT("false")));
			}

			else
			{
				if (!bPlatformAtA && bPlayerAtB)
				{
					DirectionToMove = -1;
					bIsMovingAhead = false;

					UE_LOG(LogTemp, Warning, TEXT("Direction = %d, bIncrease = %s "),
						DirectionToMove, (bIsMovingAhead ? TEXT("true") : TEXT("false")));
				}
			}
		}
	}
}

void UMovable::PerformAnimation(UStaticMeshComponent* Platform)
{
	//UE_LOG(LogTemp, Warning, TEXT("bNearBtnA = %s, bNearBtnB = %s"), (bNearBtnA ? TEXT("true") : TEXT("false")), (bNearBtnB ? TEXT("true") : TEXT("false")) );

	FVector NewLocation = Platform->GetRelativeTransform().GetLocation();

	//wait for player input

	// move forward
	if (bIsMovingAhead && bPlatformAtA)
	{
		Platform->SetRelativeLocation(FVector(NewLocation.X, NewLocation.Y, (NewLocation.Z + Speed * DirectionToMove * GetWorld()->GetDeltaSeconds())));
		//FInterpTo can be used
	}

	//move backward
	if (!bIsMovingAhead && !bPlatformAtA)
	{
		Platform->SetRelativeLocation(FVector(NewLocation.X, NewLocation.Y, (NewLocation.Z + Speed * DirectionToMove * GetWorld()->GetDeltaSeconds())));
		//FInterpTo can be used
	}
}