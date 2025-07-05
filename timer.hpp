// Author: Bjorn Wehlin

#pragma once

#include <chrono>
#include <iostream>

namespace sf
{
  /*
  
    This file contains two timer classes: Timer and ScopeTimer

    Timer
    ====
    The Timer class has .start() and .stop() methods. Simply call .start() at the
    beginning of the operation to be timed, and .stop() at the end.

    Example:
    
    sf::Timer timer;
    
    timer.start();
    doSomething();
    timer.stop();

    output --> Took 4.231s

    The Timer can be reused multiple times (but .start() will reset its starting time).

    When .stop() is called, a message is displayed to say how much time was spent. To
    configure the message, .start() can be called with an optional argument to denote
    the operation.

    timer.start("doSomething");
    doSomething();
    timer.stop();
    timer.start("doSomethingElse");
    doSomethingElse();
    timer.stop();

    output --> doSomething took 4.231s
               doSomethingElse took 80.954ms

    ScopeTimer
    ====

    A ScopeTimer owns a Timer and calls .start() at construction, and .stop() at
    destruction.

    To time a single function,

    void doSomething() {
      ScopeTimer timer;

      // do stuff
    }

    One can also introduce a scope for timing purposes alone.

    int main() {
      {
        ScopeTimer timer;
        doSomething();
      }

      {
        ScopeTimer timer;
        doSomethingElse();
      }

      return 0;
    }

    If ScopeTimer is constructed with a message, this will be passed
    to the Timer (its function is described above).

  */

  class Timer
  {
  public:
    using clock_type = std::chrono::high_resolution_clock;

    Timer()
      : m_start(clock_type::now())
    { }

    void start(const std::string& message = "")
    {
      m_message = message;
      m_start = clock_type::now();
    }

    void stop()
    {
      auto dur = std::chrono::duration<float>(clock_type::now() - m_start).count();

      if (m_message.empty())
      {
        std::cout << "Took ";
      }
      else
      {
        std::cout << m_message << " took ";
      }

      m_message.clear();
      
      if (dur < 5e-3)
      {
        std::cout << dur * 1e6 << "us";
      }
      else if (dur < 5.)
      {
        std::cout << dur * 1e3 << "ms";
      }
      else
      {
        std::cout << dur << "s";
      }
      std::cout << std::endl;
    }

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

  private:
    std::string m_message;
    typename clock_type::time_point m_start;
  };

  class ScopeTimer
  {
  public:
    ScopeTimer()
    {
      m_timer.start();
    }

    ScopeTimer(const std::string& message)
    {
      m_timer.start(message);
    }

    ~ScopeTimer()
    {
      m_timer.stop();
    }

  private:
    Timer m_timer;
  };
}