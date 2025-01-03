#include <queue>
#include <condition_variable>
#include <mutex>

template<typename T>
class MultiQueue{
public:
    void submit(const T& to_submit){
        std::lock_guard<std::mutex> lck(mutex_);
        queue_.push(to_submit);
        cv_.notify_one();  // 通知消费者
    }

    bool wait_for_value(T& ret){
        std::unique_lock<std::mutex> uni_lock(mutex_);
        // 先检查队列是否为空，避免阻塞
        if (!queue_.empty()) {
            ret = queue_.front();
            queue_.pop();
            return true;
        }

        // 如果队列为空，则阻塞等待，直到队列有数据
        cv_.wait(uni_lock, [this]() { return !queue_.empty(); });

        // 再次检查队列是否为空
        if (!queue_.empty()) {
            ret = queue_.front();
            queue_.pop();
            return true;
        }
        return false;  // 这里不会发生，防止编译器警告
    }

    bool wait_for_value(T& ret, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> uni_lock(mutex_);
        
        // 先检查队列是否为空，避免阻塞
        if (!queue_.empty()) {
            ret = queue_.front();
            queue_.pop();
            return true;
        }

        // 如果队列为空，则阻塞等待，直到队列有数据或者超时
        if (cv_.wait_for(uni_lock, timeout, [this]() { return !queue_.empty(); })) {
            // 超时没有发生，队列中有数据
            ret = queue_.front();
            queue_.pop();
            return true;
        }
        
        // 如果超时，返回false
        return false;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
};