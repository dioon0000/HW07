#include "MainGameModeBase.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"

AMainGameModeBase::AMainGameModeBase()
{
	DefaultPawnClass = AMainCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
}