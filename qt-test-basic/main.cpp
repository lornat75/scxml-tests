#include <QCoreApplication>
#include <QScxmlStateMachine>
#include <QDebug>

#include <iostream>

#include "MyStateMachine.h"

#include <thread>
#include <chrono>

void myFunction(MyStateMachine *machine)
{

  int count=1000000;
  while(count--) {
   // count++;
    //std::cout<<"Thread is running...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if (count%2==0)
    {
      //  std::cout<<"Submitting start...\n";
        machine->submitEvent("start");
     // machine->submitEvent("onTransition");
    }
    else
    {
       // std::cout<<"Submitting stop...\n";
        machine->submitEvent("stop");
    }

  }

  count=10;
  while(count--)
  {

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

  }

  QCoreApplication::quit();
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  MyStateMachine stateMachine;
  
  stateMachine.connectToEvent("onTransition", [](const QScxmlEvent &event){
    /* std::cout << event.data().isNull() << std::endl;
    std::cout << event.data().toMap()["state"].toString().toStdString() << std::endl; */
    std::cout << "received message onTransition";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "...done!"<< std::endl;
  });


  stateMachine.start();

  if (!stateMachine.isInitialized()) {
    std::cerr << "State machine failed to initialize." << std::endl;
    return -1;
  }

  std::thread t1(myFunction, &stateMachine);

  int ret=app.exec();

  t1.join();

  return ret;
  
}
