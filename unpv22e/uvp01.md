在unpv2的第7章使用了互斥锁和条件变量，是C语法。在muduo库中BlockingQueue,是c++语法,notify也和unp有一点不同。


Solstice
陈硕回复 2010-09-15 23:31:27
回复 mukey：＂push一个任务，就唤醒一个consumer，循环5次，就把5个都唤醒了＂这个可不一定，线程的调度可能以任何顺序发生。push 第一个 task 的时候，会唤醒一个消费者。但是这个消费者线程不一定会马上会执行，它会进入就绪状态，等待被调度。如果操作系统继续执行生产者线程，push 第二个 task，那么按照你的算法，不会有新的消费者被唤醒。这样一来，在 queue_ 再次变空之前，只有一个消费者醒过来处理 tasks，其余 4 个都在睡大觉。Stevens 的书上只有一个消费者，它的代码不适用于多消费者的情形。
举报回复
mukey
mukey回复 2010-09-15 17:09:43
回复 Solstice：我是看的UNP2的produce/consumer，不会错。调用notify的前提条件是，生产不足。判断生产不足的条件是当我们往queue中push后，它的容量为1. 你说的那种情况是不存在的，真存在的话那就是出了大bug，程序已经崩溃了。就拿你那个例子来说：当5个consumer都wait，那么你push一个任务，就唤醒一个consumer，循环5次，就把5个都唤醒了。第6个还会用无用的一次notify, 剩下4个就不会notify了。不可能push10个才开始唤醒的。
mukey
mukey2010-09-15 09:52:52#18楼
赞啊，看了源代码： ThreadPool::run(const Task&amp; task) { MutexLockGuard lock(mutex_); queue_.push_back(task); cond_.notify(); } 看书应该改为: if (queue_.size()==1) cond_.notify();

[https://blog.csdn.net/solstice/article/details/5829421](https://blog.csdn.net/solstice/article/details/5829421)