#include <QCoreApplication>
#include <QScxmlStateMachine>
#include <QDebug>

#include <iostream>

#include "MyStateMachine.h"

#include <thread>
#include <chrono>
#include <mutex>

std::mutex g_i_mutex; 

#include <QThread>

class MyThread: public QThread
{
public:
  MyThread(MyStateMachine *machine)
  {
    m_machine=machine;
  }

  void run()
  {
    int count=1000000;
    while(true) {
      count++;
      //std::cout<<"Thread is running...\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(0));

      // const std::lock_guard<std::mutex> lock(g_i_mutex);

      if (count%2==0)
      {
          // std::cout<<"Submitting start...\n";
          m_machine->submitEvent("start");
      }
      else
      {

          // std::cout<<"Submitting stop...\n";
          m_machine->submitEvent("stop");
      }

    }

    count=10;
    while(count--)
    {

      std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    QCoreApplication::quit();
  }

private:
  MyStateMachine *m_machine;

};

void myFunction(MyStateMachine *machine)
{

  int count=1000000;
  while(true) {
    count++;
    //std::cout<<"Thread is running...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(0));

    // const std::lock_guard<std::mutex> lock(g_i_mutex);

    if (count%2==0)
    {
        std::cout<<"Submitting start...\n";
        machine->submitEvent("start");
     // machine->submitEvent("onTransition");
    }
    else
    {

        std::cout<<"Submitting stop...\n";
        machine->submitEvent("stop");
    }

  }

  count=10;
  while(count--)
  {

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

  }

  QCoreApplication::quit();
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  MyStateMachine stateMachine;
  
  stateMachine.connectToEvent("onTransition", [](const QScxmlEvent &event){
    // std::cout << event.data().isNull() << std::endl;
    // std::cout << event.data().toMap()["state"].toString().toStdString() << std::endl;
    std::cout << "received message onTransition";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "...done!"<< std::endl;
  });

  stateMachine.start();

  if (!stateMachine.isInitialized()) {
    std::cerr << "State machine failed to initialize." << std::endl;
    return -1;
  }

  std::thread t1(myFunction, &stateMachine);
  // MyThread t1(&stateMachine);
  // t1.start();

  int ret=0;
  ret=app.exec();

  t1.join();
  //t1.wait();
  
  return ret;
  
}
