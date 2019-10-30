#include "threadpool.hpp"

ThreadPool::ThreadPool(FileReader *fr, FileWriter *fw, unsigned long bs)
    : fr(fr), fw(fw), block_size(bs)
{
    unsigned long total_chunks = fr->get_size() / bs;
    unsigned long last_chunk_size = fr->get_size() % bs;

    if(last_chunk_size)
        ++total_chunks;

    this->max_offset = total_chunks;
}

void ThreadPool::worker(FileReader* fr, FileWriter* fw, unsigned long max_offset, unsigned long block_size)
{
    // Forever loop:
    // 1. Get unique offset
    // 2. Get block by offset
    // 3. Hash block
    // 4. Write hash at offset
    static std::atomic<unsigned long> _offset;

    for(;;) {
        unsigned long offset = _offset++;
        if(offset >= max_offset)
            // no more block offsets for processing?
            // stop worker
            return;

        // Reading block by offset
        std::unique_ptr<char[]> chunk_data(new char[block_size]);
        long bytes_readed = fr->read_at(chunk_data.get(),
                                        offset * block_size,
                                        block_size);

        unsigned char* buf = reinterpret_cast<unsigned char*>(chunk_data.get());
        std::string hash_val = Hasher::get_hash(buf, size_t(bytes_readed));

        // Write hash by offset
        fw->write_at(hash_val, offset * HASH_LENGTH);
    }
}

void ThreadPool::run()
{
    // Get number of threads available (or 0)
    unsigned int n = std::thread::hardware_concurrency();

    if(n == 0)
        n = 1;

    thread_pool.resize(n);

    for(std::thread &t: thread_pool)
        t = std::thread(worker, this->fr, this->fw, this->max_offset, this->block_size);
}

void ThreadPool::join_all()
{
    for(std::thread &t: thread_pool)
        if(t.joinable())
            t.join();
}
