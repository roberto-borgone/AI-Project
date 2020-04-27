#ifndef TPOOL_H
#define TPOOL_H
#include <QObject>
#include <QDebug>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool : public QObject
{
    Q_OBJECT
public:

    ThreadPool();
    virtual ~ThreadPool();
    ThreadPool(const ThreadPool &tp); //costruttore di copia
    void queueWork(int fd, std::string& request); // This function will be called by the server,
                                                  //every time there is a request that needs to be processed by the thread pool
private:

    void doWork();
    void processRequest(const std::pair<int, std::string> item);

    std::condition_variable_any workQueueConditionVariable;
    std::mutex workQueueMutex; // Mutex to protect workQueue
    std::vector<std::thread> threads; // We store the threads in a vector, so we can later stop them gracefully
    std::queue<std::pair<int, std::string>> workQueue; // Queue of requests waiting to be processed
    bool done; // This will be set to true when the thread pool is shutting down. This tells the threads to stop looping and finish

};

#endif // TPOOL_H
