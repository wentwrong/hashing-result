#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <thread>
#include <atomic>
#include <vector>

#include <filereader.hpp>
#include <filewriter.hpp>
#include <hasher.hpp>
#include <constants.hpp>

class ThreadPool
{
    FileReader* fr;
    FileWriter* fw;
    unsigned long block_size;
    unsigned long max_offset;
    std::vector<std::thread> thread_pool;
public:
    ThreadPool(FileReader *fr, FileWriter *fw, unsigned long bs);
    static void worker(FileReader* fr, FileWriter* fw,
                       unsigned long max_offset, unsigned long block_size);
    void run();
    void join_all();
};

#endif // THREADPOOL_HPP
