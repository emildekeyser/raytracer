#include "tasks/parallel-task-scheduler.h"
#include "easylogging++.h"

#include <vector>
#include <thread>
#include <algorithm>

using namespace tasks;


namespace
{
    /// <summary>
    /// Performs tasks in parallel.
    /// In a retarded way !! ... using a Queue should be better i think. (Or maybe not if too much time is used waiting for mutex ?)
    /// </summary>
    class ParallelTaskScheduler : public tasks::schedulers::_private_::TaskSchedulerImplementation
    {

    public:
        void perform(std::vector<std::shared_ptr<Task>> tasks) const
        {

            size_t nproc = std::thread::hardware_concurrency();;
            LOG(INFO) << "NPROC: " << nproc;

            size_t length = tasks.size() / nproc;
            size_t remain = tasks.size() % nproc;
            size_t begin = 0;
            size_t end = 0;
            std::vector<std::vector<std::shared_ptr<Task>>> parts;

            for (size_t i = 0; i < std::min(nproc, tasks.size()); ++i)
            {
                end += (remain > 0) ? (length + !!(remain--)) : length;
                parts.push_back(std::vector<std::shared_ptr<Task>>(tasks.begin() + begin, tasks.begin() + end));
                begin = end;
            }

            std::vector<std::thread> threads;
            for (auto part : parts)
            {
                threads.push_back(std::thread([part]() {
                    for (auto task : part) {
                        task->perform();
                    }
                }));
            }

            for (auto &thread : threads) {
                thread.join();
            }

            // std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
            //         t.join();
            //         });

        }
    };
}

TaskScheduler tasks::schedulers::parallel()
{
    return TaskScheduler(std::make_shared<ParallelTaskScheduler>());
}
