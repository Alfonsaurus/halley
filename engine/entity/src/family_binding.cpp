﻿#include "family_binding.h"

using namespace Halley;

FamilyBindingBase::FamilyBindingBase(FamilyMaskType readMask, FamilyMaskType writeMask)
	: family(nullptr)
	, readMask(readMask)
	, writeMask(writeMask)
{
}

void FamilyBindingBase::setFamily(Family* f) {
	family = f;
}