#include "TimerQueue.h"
#include <stdio.h>

Timer::Timer(double Duration_Seconds)
{
	next = NULL;
	this->Duration_Seconds = Duration_Seconds;
}

Timer::~Timer()
{
	if (next != NULL)
		delete next;
}

void Timer::Insert(Timer *newTimer)
{
	if (next != NULL)
		next->Insert(newTimer);
	else
		next = newTimer;
}

int Timer::Tick(double TimeStep_Seconds)
{
	Duration_Seconds -= TimeStep_Seconds;

	if (IsExpired())        // If the timer has expired
	{
		if (next != NULL)
			return 1 + next->Tick(TimeStep_Seconds);
		else
			return 1;
	}
	else
	{
		if (next != NULL)
			return next->Tick(TimeStep_Seconds);
		else
			return 0;
	}
}

TimerQueue::TimerQueue()
{
	TimeStep_Seconds = -1.0;
	first = NULL;
}

TimerQueue::~TimerQueue()
{
	if (first != NULL)
		delete first;
}

TimerQueue::TimerQueue(double SampleRate_Hz)
{
	TimeStep_Seconds = 1.0 / SampleRate_Hz;
	first = NULL;
}


int TimerQueue::Step(double TimeStep_Seconds)
{
	int result;
	Timer *cur, *old;

	if (first == NULL)
		return 0;

	result = first->Tick(TimeStep_Seconds);
	if (result > 0)
	{
		// At least one timer has expired.  Clean up the expired timers
		cur = first;
		while (cur != NULL)
		{
			if (cur->IsExpired())
			{
				old = cur;
				cur = cur->GetNext();
				delete old;
			}
			else
				break;
		}
		first = cur;
	}
	return result;
}

void TimerQueue::Add(Timer *newTimer)
{
	if (first == NULL)
		first = newTimer;
	else
		first->Insert(newTimer);
}

