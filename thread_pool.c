// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <stdatomic.h>
// #include <stdbool.h>
// #include <unistd.h>




// #define BUFFER_SIZE 16 // Must be a power of 2 for bitwise masking
// #define BUFFER_MASK (BUFFER_SIZE - 1)

// // Define a task packet
// typedef struct {
//     void (*function)(void*);
//     void* arg;
// } thread_task_t;

// // Lock-free bounded queue (Ring Buffer)
// typedef struct {
//     thread_task_t buffer[BUFFER_SIZE];
//     _Atomic size_t head;
//     _Atomic size_t tail;
// } lock_free_queue_t;

// // Thread pool structure
// typedef struct {
//     pthread_t* threads;
//     size_t thread_count;
//     lock_free_queue_t queue;
//     _Atomic bool shutdown;
//     _Atomic int active_tasks;
// } thread_pool_t;

// // Initialize the queue
// void queue_init(lock_free_queue_t* queue) {
//     atomic_init(&queue->head, 0);
//     atomic_init(&queue->tail, 0);
// }

// // Push a task onto the queue (Lock-free)
// bool queue_push(lock_free_queue_t* queue, thread_task_t task) {
//     size_t current_tail = atomic_load_explicit(&queue->tail, memory_order_relaxed);
    
//     for (;;) {
//         size_t current_head = atomic_load_explicit(&queue->head, memory_order_acquire);
        
//         // Check if queue is full
//         if ((current_tail - current_head) >= BUFFER_SIZE) {
//             return false; 
//         }
        
//         // Attempt to claim the slot
//         if (atomic_compare_exchange_weak_explicit(&queue->tail, &current_tail, current_tail + 1,
//                                                  memory_order_release, memory_order_relaxed)) {
//             // Safely write to the claimed slot
//             queue->buffer[current_tail & BUFFER_MASK] = task;
//             return true;
//         }
//         // If exchange fails, current_tail is updated automatically; loop continues
//     }
// }

// // Pop a task from the queue (Lock-free)
// bool queue_pop(lock_free_queue_t* queue, thread_task_t* out_task) {
//     size_t current_head = atomic_load_explicit(&queue->head, memory_order_relaxed);
    
//     for (;;) {
//         size_t current_tail = atomic_load_explicit(&queue->tail, memory_order_acquire);
        
//         // Check if queue is empty
//         if (current_head == current_tail) {
//             return false;
//         }
        
//         // Read data ahead of the CAS operation
//         *out_task = queue->buffer[current_head & BUFFER_MASK];
        
//         // Attempt to consume the slot
//         if (atomic_compare_exchange_weak_explicit(&queue->head, &current_head, current_head + 1,
//                                                  memory_order_release, memory_order_relaxed)) {
//             return true;
//         }
//     }
// }

// // Worker thread logic
// void* worker_thread(void* arg) {
//     thread_pool_t* pool = (thread_pool_t*)arg;
    
//     while (!atomic_load_explicit(&pool->shutdown, memory_order_relaxed) || 
//            atomic_load_explicit(&pool->active_tasks, memory_order_relaxed) > 0) {
        
//         thread_task_t task;
//         if (queue_pop(&pool->queue, &task)) {
//             atomic_fetch_add_explicit(&pool->active_tasks, 1, memory_order_relaxed);
            
//             // Execute the work function
//             task.function(task.arg);
            
//             atomic_fetch_sub_explicit(&pool->active_tasks, 1, memory_order_release);
//         } else {
//             usleep(100); // Backoff to prevent 100% CPU spinning when idle
//         }
//     }
//     return NULL;
// }

// // Create the thread pool
// thread_pool_t* pool_create(size_t thread_count) {
//     thread_pool_t* pool = malloc(sizeof(thread_pool_t));
//     if (!pool) return NULL;
    
//     pool->thread_count = thread_count;
//     pool->threads = malloc(sizeof(pthread_t) * thread_count);
//     queue_init(&pool->queue);
//     atomic_init(&pool->shutdown, false);
//     atomic_init(&pool->active_tasks, 0);
    
//     for (size_t i = 0; i < thread_count; i++) {
//         pthread_create(&pool->threads[i], NULL, worker_thread, pool);
//     }
    
//     return pool;
// }

// // Destroy the thread pool gracefully
// void pool_destroy(thread_pool_t* pool) {
//     atomic_store_explicit(&pool->shutdown, true, memory_order_release);
    
//     for (size_t i = 0; i < pool->thread_count; i++) {
//         pthread_join(pool->threads[i], NULL);
//     }
    
//     自由(pool->threads);
//     free(pool);
// }

// // --- Demonstration / Benchmark ---

// void sample_work(void* arg) {
//     long id = (long)arg;
//     printf("Task %ld executing on a worker thread.\n", id);
//     usleep(50000); // Simulate processing work
// }

// int main() {
//     printf("Initializing Lock-Free Thread Pool...\n");
//     thread_pool_t* pool = pool_create(4); // 4 Worker threads
    
//     // Dispatch 10 tasks asynchronously
//     for (long i = 1; i <= 10; i++) {
//         thread_task_t task = { .function = sample_work, .arg = (void*)i };
//         while (!queue_push(&pool->queue, task)) {
//             usleep(10); // Queue full backoff
//         }
//     }
    
//     // Give tasks time to start executing
//     sleep(2);
    
//     printf("Shutting down Thread Pool...\n");
//     pool_destroy(pool);
//     printf("Done!\n");
    
//     return 0;
// }
