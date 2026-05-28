// Copyright 2026 Ajinkya Borade. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogPawnExtensionCompBase.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUE_API URogPawnExtensionCompBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template <class T> T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template parameter is not derived from APawn");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template <class T> T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template parameter is not derived from AController");
		const APawn* OwningPawn = GetOwningPawn();
		if (!OwningPawn) return nullptr;
		return OwningPawn->GetController<T>();
	}
};
