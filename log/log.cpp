#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include "log.h"
#include <pthread.h>

Log::Log () {
    m_count = 0;
    m_is_async = false;
}

Log::~Log () {
    if (m_fp != NULL) {
        fclose(m_fp);
    }
}

// 异步需要设置阻塞队列的长度，同步不需要设置
bool Log::init (const char *file_name, int close_log, int log_buf_size, int split_lines, int max_queue_size) {
    // 如果设置了max_queue_size，则设置为异步
    if (max_queue_size >= 1) {
        m_is_async = true;
        m_log_queue = new block_queue<std::string>(max_queue_size);
        pthread_t tid;
        pthread_create(&tid, NULL, flush_log_thread, NULL);
    }

    m_close_log = close_log;
    m_log_buf_size = log_buf_size;
    m_buf = new char[m_log_buf_size];
    memset(m_buf, '\0', m_log_buf_size);
    m_split_lines = split_lines;

    time_t t = time(NULL);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    const char *p
} 