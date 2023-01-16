#include <stdlib.h>
#include <pthread.h>

struct job {
    struct job  *j_next;
    struct job  *j_prev;
    pthread_t   j_id;
    /* ... */
};

struct queue {
    struct job          *q_head;
    struct job          *q_tail;
    pthread_rwlock_t    q_lock;
};

int queue_init(struct queue *qp)
{
    int err;

    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0)
        return(err);
    /* ... */
    return 0;
}

void job_insert(struct queue *qp, struct job *jb)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jb->j_next = qp->q_head;
    jb->j_prev = NULL;
    if (qp->q_head != NULL)
        qp->q_head->j_prev = jb;
    else
        qp->q_tail = jb;
    qp->q_head = jb;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_append(struct queue *qp, struct job *jb)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jb->j_next = NULL;
    jb->j_prev = qp->q_tail;
    if (qp->q_tail != NULL)
        qp->q_tail->j_next = jb;
    else
        qp->q_head = jb;
    qp->q_tail = jb;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_remove(struct queue *qp, struct job *jb)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jb == qp->q_head) {
        qp->q_head = jb->j_next;
        if (qp->q_tail == jb)
            qp->q_tail = NULL;
        else
            jb->j_next->j_prev = jb->j_prev;
    } else if (jb == qp->q_tail) {
        qp->q_tail = jb->j_prev;
        qp->q_tail->j_next = NULL;
    } else {
        jb->j_next->j_prev = jb->j_prev;
        jb->j_prev->j_next = jb->j_next;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

struct job *job_find(struct queue *qp, pthread_t id)
{
    struct job *jb;
    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return(NULL);

    for (jb = qp->q_head; jb != NULL; jb = jb->j_next)
    {
        if (pthread_equal(jb->j_id, id) != 0)
            break;
    }

    pthread_rwlock_unlock(&qp->q_lock);
    return(jb);
}