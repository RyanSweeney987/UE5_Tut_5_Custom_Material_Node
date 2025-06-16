// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialExpression.h"
#include "ExampleRTMaterialExpression.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TUT_5_CUSTOM_MATERIAL_NODES_API UExampleRTMaterialExpression : public UMaterialExpression
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (RequiredInput = "false"))
	FExpressionInput TextureInput;

	UPROPERTY(EditAnywhere, meta = (Tooltip = "Example tooltip", OverridingInputProperty = "TextureInput"))
	TObjectPtr<UTexture2D> Texture = nullptr;
	
public:
	UExampleRTMaterialExpression();
	
protected:
#if WITH_EDITOR
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual int32 CompilePreview(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual uint32 GetInputType(int32 InputIndex) override;
#endif
	//~ End UMaterialExpression Interface

	/** Returns true if GetReferencedTexture() can ever return a valid pointer. */
	// Required if you're referencing a texture in your material expression
	virtual bool CanReferenceTexture() const override { return true; }
	virtual UObject* GetReferencedTexture() const override
	{
		return Texture;
	}
};
