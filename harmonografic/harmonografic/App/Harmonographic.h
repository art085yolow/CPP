#pragma once

#include "UserInterface.h"

class Harmonographic: public UserInterface
{
public:
	Harmonographic();
	~Harmonographic();

	
	// Odziedziczono za pośrednictwem elementu UserInterface
	virtual void onCreate() override;

	virtual void onUpdate() override;

	virtual void onDestroy() override;

private:




};

