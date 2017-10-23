#include <string_view>
#include <functional>
using namespace std;

class ITaskNotify {
public:
    virtual ~ITaskNotify() = default;
    virtual void onTask() { delete this; }

private:
    friend class TaskQueue;
    ITaskNotify * m_taskNext{nullptr};
};

class RunOnceTask : public ITaskNotify {
public:
    RunOnceTask(string_view name, function<void()> && fn);

private:
    void onTask() override;

    function<void()> m_fn;
};

/*
I'd like to report a false positive, and the wiki sent me here to get an account.
This constructor: RunOnceTask(string_view name, function<void()> && fn);
Results in: Class 'RunOnceTask' has a constructor with 1 argument that is not explicit.
A complete cpp containing this can be found at: https://godbolt.org/g/HGtn5R
Thanks.
*/
