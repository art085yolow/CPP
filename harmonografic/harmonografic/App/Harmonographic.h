#pragma once

#include "./WindowInterface/UserInterface.h"
#include "./WindowHandler.h"


class Harmonographic: public UserInterface
{
public:
	Harmonographic();
	~Harmonographic();

	
	// Odziedziczono za pośrednictwem elementu UserInterface
	virtual void onCreate() override;

	virtual void onUpdate() override;

	virtual void onDestroy() override;

	
	// clean up after


private:
	WindowHandler m_main_window;



};

