// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "ExampleMaterialExpression.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TUT_5_CUSTOM_MATERIAL_NODES_API UExampleMaterialExpression : public UMaterialExpression
{
	GENERATED_BODY()
	
public:
	// Input
	UPROPERTY(meta = (RequiredInput = "false"))
    FExpressionInput ExampleAInput;

	// This allows us to set the value without needing to connect a pin
	// The engine automatically picks up the connection as ExampleA without the suffix "Input"
	UPROPERTY(EditAnywhere, meta = (Tooltip = "Example tooltip", OverridingInputProperty = "ExampleAInput"))
	FVector4f ExampleA = FVector4f(0.0f, 0.0f, 0.0f, 0.0f);

public:
	UExampleMaterialExpression();

#if WITH_EDITOR
	//~ Begin UMaterialExpression Interface
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual int32 CompilePreview(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	//~ End UMaterialExpression Interface
#endif 
};
