class Timer
{
public:
	Timer(double Duration_Seconds);

	~Timer();

	int Tick(double TimeStep_Seconds);

	void Insert(Timer *newTimer);

	Timer *GetNext(void) {return next;}

	bool IsExpired(void) {return Duration_Seconds <= 0.0;}
private:
	Timer *next;
	double Duration_Seconds;
};

class TimerQueue
{
public:
	TimerQueue();
	TimerQueue(double SampleRate_Hz);

	~TimerQueue();

	int Step(double TimeStep_Seconds);
	int Step(void);

	void Add(Timer *newTimer);
private:
	Timer *first;
	double TimeStep_Seconds;
};

