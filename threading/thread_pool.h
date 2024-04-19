#ifndef THREADPOOL_H
#define THREADPOOL_H


#include <condition_variable> 
#include <functional> 
#include <iostream> 
#include <mutex> 
#include <queue> 
#include <vector>
#include <thread>  


// Class that represents a simple thread pool 
class ThreadPool { 
public: 
    /**
     * @brief Construct a new Thread Pool object
     * 
     * @param num_threads Number of threads in the pool. Default is the number of available hardware threads 
     */
    ThreadPool(size_t num_threads 
               = std::thread::hardware_concurrency()) 
    { 
  
        // Creating worker threads 
        for (size_t i = 0; i < num_threads; ++i) { 
            threads_.emplace_back([this] { 
                while (true) { 
                    std::function<void()> task; 
                    // The reason for putting the below code 
                    // here is to unlock the queue before 
                    // executing the task so that other 
                    // threads can perform enqueue tasks 
                    { 
                        // Locking the queue so that data 
                        // can be shared safely 
                        std::unique_lock<std::mutex> lock( 
                            queue_mutex_); 
  
                        // Waiting until there is a task to 
                        // execute or the pool is stopped 
                        cv_.wait(lock, [this] { 
                            return !tasks_.empty() || stop_; 
                        }); 
  
                        // exit the thread in case the pool 
                        // is stopped and there are no tasks 
                        if (stop_ && tasks_.empty()) { 
                            return; 
                        } 
  
                        // Get the next task from the queue 
                        task = std::move(tasks_.front()); 
                        tasks_.pop(); 
                    } 
                    // Execute the task
                    task(); 
                } 
            }); 
        } 
    } 
  
    /**
     * @brief Destroy the Thread Pool object
     * 
     */
    ~ThreadPool() 
    { 
        { 
            // Lock the queue to update the stop flag safely 
            std::unique_lock<std::mutex> lock(queue_mutex_); 
            stop_ = true; 
        } 
  
        // Notify all threads 
        cv_.notify_all(); 
  
        // Joining all worker threads to ensure they have 
        // completed their tasks 
        for (auto& thread : threads_) { 
            thread.join(); 
        } 
    } 
  
    /**
     * @brief Main function to add a task to the queue
     * 
     * @param task Task to be added to the queue as type std::function<void()>
     */
    void enqueue(std::function<void()> task) 
    { 
        {
            // Scoped lock to update the number of tasks
            std::lock_guard<std::mutex> lock(task_count_mutex_);
            ++number_of_tasks;  
        }
        
        // Create a new function that executes the task and then calls taskDone
        // This is done to ensure that the taskDone is called after the task is executed
        auto task_with_done = [this, captured_task = std::move(task)]() {
            captured_task();
            this->taskDone();
        };

        { 
            // Scoped lock to add task to the queue
            std::unique_lock<std::mutex> lock(queue_mutex_); 
            tasks_.emplace(std::move(task_with_done)); 
        }
        // Notify The waiting thread that a task is done
        cv_.notify_one(); 
    } 

    /**
     * @brief Let the thread pool know that a task is done if they are waiting for the tasks to finish
     * 
     */
    void taskDone() 
    {
        std::lock_guard<std::mutex> lock(task_count_mutex_);
        --number_of_tasks;  // decrease number of pending tasks 
        // Notify The waiting thread that a task is done
        task_cv_.notify_one();
    }

    /**
     * @brief Function for the main thread to wait until all tasks are finished from the pool
     * 
     */
    void waitUntilFinished()
    {
        std::unique_lock<std::mutex> lock(task_count_mutex_);
        task_cv_.wait( lock, [&] () { return (number_of_tasks <= 0); } );
    }
  
private: 
    // Vector to store worker threads 
    std::vector<std::thread> threads_; 
  
    // Queue of tasks 
    std::queue<std::function<void()> > tasks_; 

    // Number of tasks in queue and being executed
    int number_of_tasks = 0;
  
    // Mutex to synchronize access to shared data 
    std::mutex queue_mutex_; 

    // Mutex to synchronize access to the number of tasks
    std::mutex task_count_mutex_;
  
    // Condition variable to signal changes in the state of 
    // the tasks queue 
    std::condition_variable cv_; 

    // Condition variable to signal changes in state of the number of tasks finished
    std::condition_variable task_cv_;
  
    // Flag to indicate whether the thread pool should stop or not 
    bool stop_ = false; 
}; 


#endif // THREADPOOL_H