// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Gun = CreateDefaultSubobject<USceneComponent>(TEXT("Gun"));
	RootComponent = Gun;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(Gun);

}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, TEXT("MuzzleSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	

	if(GunTrace(Hit, ShotDirection))
	{
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location , ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
	
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if(OwnerController == nullptr) return false;
	// el location hwa el start point, da location el camera 
	// el rotation hwa direction el camera
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location , Rotation);

	ShotDirection = -Rotation.Vector();
	// Roation.Vector() btgbly el axis elly el camera bassa feeh
	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1 , Params);
}

AController *AGun::GetOwnerController() const
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if(OwnerPawn == nullptr) 
		return nullptr;
	return OwnerPawn->GetController();
	
}
