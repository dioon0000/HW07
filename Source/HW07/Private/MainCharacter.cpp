#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetSimulatePhysics(false);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input Component로 캐스팅
	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA를 가져오기 위해 현재 소유 중인 Controller를 AMainPlayerController로 캐스팅
		if(AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
		{
			if(PlayerController->MoveAction)
			{
				// IA_Move 액션 키를 "누르고 있는 동안" Move() 호출
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMainCharacter::Move
				);
			}
			if(PlayerController->LookAction)
			{
				// IA_Look 액션 마우스가 "움직일 때" Look() 호출
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMainCharacter::Look
				);
			}
		}
	}
}

void AMainCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MoveInput = value.Get<FVector2D>();
	const float MoveSpeed = 500.0f;
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	const FVector Offset = FVector(MoveInput.X, MoveInput.Y, 0.f) * MoveSpeed * DeltaTime;

	AddActorLocalOffset(Offset, true);
}

void AMainCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	// Yaw ( 좌우 회전 )
	AddActorLocalRotation(FRotator(0.0f, LookInput.X, 0.0f), true);

	// Pitch ( 상하 회전 )
	if(SpringArmComp)
	{
		// 기존 회전에 새로운 회전 값을 더한다
		// 마우스 Y축 입력은 일반적으로 방향이 반대이므로 -1을 곱해준다
		SpringArmComp->AddLocalRotation(FRotator(LookInput.Y * -1.0f, 0.0f, 0.0f), true);

		// 카메라가 너무 많이 회전해서 뒤집히는 것을 방지하기 위해 Pitch 각도를 제한
		FRotator CurrentRotation = SpringArmComp->GetRelativeRotation();
		CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch, -70.0f, 20.0f);

		CurrentRotation.Roll = 0.0f;

		SpringArmComp->SetRelativeRotation(CurrentRotation);
	}
}