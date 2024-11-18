#include "utilities.h"

Observer  observer;
Observer* notifier;

void Setup_Notifier()
{
	notifier = &observer;
	Observer_Init(notifier);
}
