// 笔记文件

// 1、生产者和消费者模型
#include <pthread.h>
struct msg {
    struct msg *m_next;
};

struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg () {
    struct msg *mp;
    for (;;) {
        pthread_mutex_lock(&qlock);
        while (workq == NULL) {
            pthread_cond_wait(&qready, &qlock);
        }
        mp = workq;
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);
    }
}

void enqueue_msg (struct msg *mp) {
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}

// 2、单例模式：懒汉式、饿汉式

// 线程安全懒汉模式，使用双检测锁模式
class single {
private:
    static single *p;
    static pthread_mutex_t lock;

    single () {
        pthread_mutex_init(&lock, nullptr);
    }
    ~single () {

    }
public:
    static single *getinstance();
};

single *single::p = nullptr;
single * single::getinstance () {
    if (nullptr == p) {
        pthread_mutex_lock(&lock);
        if (nullptr == p) {
            p = new single;
        }
        pthread_mutex_unlock(&lock);
    }
    return p;
}

// 线程安全懒汉模式，使用局部静态变量
// C++11标准及之后，保证静态局部变量线程安全不需要加锁
class single_2 {
private:
    single_2 () {}
    ~single_2 () {}
public:
    static single_2 * getinstance ();
};

single_2 * single_2::getinstance () {
    static single_2 obj;
    return &obj;
}

// 饿汉模式
class single_3 {
private:
    static single_3 *p;
    single_3 () {}
    ~single_3 () {}
public:
    static single_3 * getinstance ();
};

single_3 *single_3::p = new single_3();
single_3 * single_3::getinstance () {
    return p;
}