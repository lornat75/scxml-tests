#include <QCoreApplication>
#include <QScxmlStateMachine>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QScxmlEvent>
#include <iostream>

#include "MyStateMachine.h"

#include <thread>
#include <chrono>
#include <mutex>

std::mutex g_i_mutex; 

void myFunction(MyStateMachine *machine, bool *t1, bool *t2)
{

  // we can use count in the while to stop at a certain number of iterations
  int count=1000000;
  while(true) {
    count++;
    //std::cout<<"Thread is running...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // const std::lock_guard<std::mutex> lock(g_i_mutex);

    if (count%2==0)
    {
        std::cout<<"Submitting start...\n";
        *t1=true;
    }
    else
    {
        std::cout<<"Submitting stop...\n";
        *t2=true;
    }

  }

}



int main(int argc, char *argv[])
{
  QTimer timer1;
  QTimer timer2; 

  QCoreApplication app(argc, argv);

  MyStateMachine stateMachine;

  bool fire1=false;
  bool fire2=false;
   QObject::connect(&timer1, &QTimer::timeout, [&](){
          //std::cout<<"submitting event from thread:"<<std::this_thread::get_id()<<std::endl;

          if (fire1)
                {
                   std::cout<<"submitting event from thread:"<<std::this_thread::get_id()<<std::endl;
                   stateMachine.submitEvent("start"); 
                   fire1=false;
                }           
  });
  //t.setSingleShot(true);
  timer1.start(0);

  QObject::connect(&timer2, &QTimer::timeout, [&](){
          //std::cout<<"submitting event from thread:"<<std::this_thread::get_id()<<std::endl;

          if (fire2)
                {
                   std::cout<<"submitting event from thread:"<<std::this_thread::get_id()<<std::endl;
                   stateMachine.submitEvent("stop"); 
                   fire2=false;
                }           
  });
  //t.setSingleShot(true);
  timer2.start(0);

  stateMachine.connectToEvent("onTransition", [](const QScxmlEvent &event){
    // std::cout << event.data().isNull() << std::endl;
    // std::cout << event.data().toMap()["state"].toString().toStdString() << std::endl;
    std::cout << "received message onTransition";
    std::this_thread::sleep_for(std::chrono::milliseconds(0));
    std::cout << "...done!"<< std::endl;
  });

  stateMachine.start();

  std::cout << "State machine going to initialize from thread id " << std::this_thread::get_id() <<std::endl;

  if (!stateMachine.isInitialized()) {
    std::cerr << "State machine failed to initialize." << std::endl;
    return -1;
  }

  std::thread t1(myFunction, &stateMachine, &fire1, &fire2);
 
  int ret=0;
  ret=app.exec();

  //t1.join();
  //t1.wait();
  
  return ret;
  
}
