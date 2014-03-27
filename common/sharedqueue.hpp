#ifndef DEFINE_MQ_HEADER
#define DEFINE_MQ_HEADER

#include <queue>
#include <pthread.h>
#include <stdexcept>
#include <cstring>
#include <string>

/* Excellent PThread examples: 
 * https://computing.llnl.gov/tutorials/pthreads/ 
 */

template <typename T>
class SharedQueue {
	private:
		pthread_mutex_t _mutex;
		pthread_cond_t _cond;
		std::queue<T> _queue;
	public:
		SharedQueue() : _queue() {
			pthread_mutex_init(&_mutex, NULL); 
			pthread_cond_init(&_cond, NULL);
		}
		~SharedQueue(){
			pthread_mutex_destroy(&_mutex);
			pthread_cond_destroy(&_cond);
		}
		void push(T const & item){
			int lock = pthread_mutex_lock(&_mutex);
			if (lock != 0){
				throw std::runtime_error(
					std::string("Couldn't acquire lock: ") + strerror(lock)
				);
			} else {
				_queue.push(item);
				pthread_mutex_unlock(&_mutex);
				pthread_cond_signal(&_cond);
			}
		}
		T const & pop(void){
			int lock = pthread_mutex_lock(&_mutex);
			const T * res;
			if (lock != 0){
				throw std::runtime_error(
					std::string("Couldn't acquire lock: ") + strerror(lock)
				);
			} else {
				while (_queue.size() == 0)
					pthread_cond_wait(&_cond, &_mutex);
				T ref = _queue.front();
				_queue.pop();
				pthread_mutex_unlock(&_mutex);
				res = &ref;
			}
			return *res;
		}
		
		
		T const & front(void){
			int lock = pthread_mutex_lock(&_mutex);
			const T * res;
			if (lock != 0){
				throw std::runtime_error(
					std::string("Couldn't acquire lock: ") + strerror(lock)
				);
			} else {
				while (_queue.size() == 0)
					pthread_cond_wait(&_cond, &_mutex);
				T & ref = _queue.front();
				pthread_mutex_unlock(&_mutex);
				res = &ref;
			}
			return *res;
		}
		
		bool available(void){
			bool res = false;
			int lock = pthread_mutex_lock(&_mutex);
			if (lock != 0){
				throw std::runtime_error(
					std::string("Couldn't acquire lock: ") + strerror(lock)
				);
			} else {
				res = _queue.size() > 0;
				pthread_mutex_unlock(&_mutex);
			}
			return res;
		}
		size_t size()
		{
			int lock = pthread_mutex_lock(&_mutex);
			size_t res;
			if (lock != 0){
				throw std::runtime_error(
					std::string("Couldn't acquire lock: ") + strerror(lock)
				);
			} else {
				res = _queue.size();
				pthread_mutex_unlock(&_mutex);
			}
			return res;
		}
};

#endif
