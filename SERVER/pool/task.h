
#include <functional>

template <typename T, typename... Args>
class Task {
    std::function<decltype(std::declval<T>()(std::declval<Args>()...))()> stored_task;

public:
    Task(const T& t, const Args&... args)
        : stored_task([t, args...]() { return t(args...); }) {}

    inline auto operator()() -> decltype(stored_task()) {
        return stored_task();
    }
};