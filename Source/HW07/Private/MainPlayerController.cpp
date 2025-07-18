#include "MainPlayerController.h"
#include "EnhancedInputSubSystems.h"

AMainPlayerController::AMainPlayerController()
	:	InputMappingContext(nullptr),
		MoveAction(nullptr),
		LookAction(nullptr)
{

}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 현재 PlayerController에 연결된 Local Player 객체를 가져온다
	if(ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// LocalPlayer에서 EnhancedInputLocalPlayerSubsystem을 획득
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		   LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// InputMappingContext가 존재하는지 체크 (안전 코드)
			if(InputMappingContext)
			{
				// Subsystem을 통해 우리가 할당한 IMC를 활성화
				// 우선순위(Priority)는 0이 가장 높은 우선순위
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}