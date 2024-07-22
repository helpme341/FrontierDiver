// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UItemBase;
struct FItemDynamicInfoBase;
struct FItemTableRowInfoBase;



/**
 * 
 */
template<typename DT  = FItemDynamicInfoBase, typename TRT = FItemTableRowInfoBase>
class FRONTIERDIVER_API TItemTmpl
{
public:

	DT* ItemDynamicInfo = NewObject<DT>();
	TRT* ItemTableRowInfo;
	TItemTmpl<DT, TRT>* GetItemTmpl();


	2
};

template<typename DT, typename TRT>
inline TItemTmpl<DT, TRT>* TItemTmpl<DT, TRT>::GetItemTmpl()
{
	return this;
}
