// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<typename OWT, typename DT, typename TRT>
class FRONTIERDIVER_API TItemTmpl
{
public:

	DT ItemDynamicInfo;
	TRT* ItemTableRowInfo;
	OWT* Owner;


	TItemTmpl<OWT, DT, TRT>* GetItemTmpl();
};

template<typename OWT, typename DT, typename TRT>
inline TItemTmpl<OWT, DT, TRT>* TItemTmpl<OWT, DT, TRT>::GetItemTmpl()
{
	return this;
}
