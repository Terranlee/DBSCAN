#ifndef ASYNC_HELPERS_H_
#define ASYNC_HELPERS_H_

#include <boost/shared_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

namespace maxcompilersim {

typedef boost::mutex Mutex;
typedef boost::shared_ptr<Mutex> MutexPtr;
typedef boost::condition_variable CondVar;
typedef boost::shared_ptr<CondVar> CondVarPtr;

} // namespace maxcompilersim

#endif // ASYNC_HELPERS_H_
