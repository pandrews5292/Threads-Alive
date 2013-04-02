// ta_mutex.c

void ta_lock_init(talock_t* lock){
    lock = (talock_t*)malloc(sizeof(talock_t));
    ta_sem_init(lock->sem, 1);
}

void ta_lock_destroy(talock_t* lock){
    ta_sem_destroy(lock->sem);
    free(lock);
}

void ta_lock(talock_t* lock){
    ta_sem_wait(lock->sem);
}

void ta_unlock(talock_t* lock){
    ta_sem_signal(lock->sem);
}
