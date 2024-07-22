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

	TStrongObjectPtr<DT> ItemDynamicInfo = TStrongObjectPtr(NewObject<DT>());
	TSharedPtr<TRT> ItemTableRowInfo;
	TItemTmpl<DT, TRT>* GetItemTmpl();
};

template<typename DT, typename TRT>
inline TItemTmpl<DT, TRT>* TItemTmpl<DT, TRT>::GetItemTmpl()
{
	return this;
}
