#ifndef OPCOM_TIMER_H_INCLUDED
#define OPCOM_TIMER_H_INCLUDED

namespace Opcom {

/**
 A timer class.
*/
class Timer {
private:
    /** the time interval */
    unsigned long m_interval;
    /** start time of current cycle */
    unsigned long m_startTime;

public:
    /**
      Create a timer with a specific interval
      @param ms Time in milliseconds
    */
    Timer(unsigned long ms) {
        m_interval = ms;
        m_startTime = millis();
    }

    /**
      Check if the interval has passed.
      @returns true if the interval has passed
    */
    bool isExpired() { return millis() < (m_startTime + m_interval); }

    /**
      Reset the start time to the current time
    */
    void reset() { m_startTime = millis(); }

    /**
      Change the interval of the timer
      @param ms Timer interval in milliseconds
    */
    void setInterval(unsigned long ms) { m_interval = ms; }

    /**
      Check if expired, and reset if so. Using this method, the timer will prevent
      drifting by adding any extra time it took between calls. After a call to
      this method, when it returns true, it also does a reset of the timer.
      @returns True if the timer period has elapsed.
    */
    bool hasPeriodPassed() {
        unsigned long ts = millis();
        unsigned long elapsed = ts - m_startTime;
        if(elapsed >= m_interval) {
            elapsed = elapsed - m_interval;
            m_startTime = ts - elapsed;
            return true;
        }
        return false;
    }
};

}

#endif // OPCOM_TIMER_H_INCLUDED
