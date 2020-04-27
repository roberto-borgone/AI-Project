#include "tpool.h"

ThreadPool::ThreadPool()
{
    this->done = false;

    auto numberOfThreads = std::thread::hardware_concurrency();
    if (numberOfThreads == 0) {
      numberOfThreads = 1;  // se non c'è nessun thread ne istanzio almeno uno
    }

    for (unsigned i = 0; i < numberOfThreads; ++i) {
      threads.push_back(std::thread(&ThreadPool::doWork, this));  // The threads will execute the private member `doWork`
    }
}


ThreadPool::~ThreadPool()
{
    done = true; // So threads know it's time to shut down

    workQueueConditionVariable.notify_all(); // Wake up all the threads, so they can finish and be joined
    for (auto& thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
}

void ThreadPool::queueWork(int fd, std::string &request)
{
    std::lock_guard<std::mutex> g(workQueueMutex);
    workQueue.push(std::pair<int, std::string>(fd, request));  // Push the request to the queue
    workQueueConditionVariable.notify_one(); // Notify one thread that there are requests to process

}

void ThreadPool::doWork()
{
        // Loop while the queue is not destructing
        while (!done) {
          std::pair<int, std::string> request;

          // Create a scope, so we don't lock the queue for longer than necessary
          {
            std::unique_lock<std::mutex> g(workQueueMutex);
            workQueueConditionVariable.wait(g, [&]{  // Only wake up if there are elements in the queue or the program is shutting down
              return !workQueue.empty() || done;
            });

            request = workQueue.front();
            workQueue.pop();
          }

          processRequest(request);
        }
}

void ThreadPool::processRequest(const std::pair<int, std::string> item)
{
       //std::this_thread::sleep_for(std::chrono::seconds(5));
       // Send a message to the connection
       //std::string response = "Good talking to you\n";
       //send(item.first, response.c_str(), response.size(), 0);
       // Close the connection
       //close(item.first);
}


