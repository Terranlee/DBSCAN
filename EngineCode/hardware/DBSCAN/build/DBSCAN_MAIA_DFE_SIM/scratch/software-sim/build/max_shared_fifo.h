#ifndef MAX_SHARED_FIFO_H
#define MAX_SHARED_FIFO_H

#include <stddef.h>



#ifdef __cplusplus
extern "C"
{
#endif

typedef struct max_shared_fifo *max_shared_fifo_t;



/**
 * \brief Create a shared FIFO.
 *
 * This FIFO should be deallocated via max_shared_fifo_destroy().
 *
 * \param name The name of the shared FIFO.
 * \param num_bytes The size of the FIFO, in bytes.
 * \param fifo Pointer to FIFO that is set if the function is successful.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_create(const char *name, size_t num_bytes, max_shared_fifo_t *fifo);

/**
 * \brief Destroy a shared FIFO.
 *
 * The FIFO must have been created via max_shared_fifo_create().
 *
 * \param fifo The FIFO to destroy.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_destroy(max_shared_fifo_t fifo);

/**
 * \brief Open an existing shared FIFO.
 *
 * The FIFO should be deallocated via max_shared_fifo_close().
 *
 * \param name The name of the existing FIFO.
 * \param fifo Pointer to FIFO that is set if the function is successful.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_open(const char *name, max_shared_fifo_t *fifo);

/**
 * \brief Close a shared FIFO.
 *
 * The FIFO must have been opened via max_shared_fifo_open().
 *
 * \param fifo The FIFO to close.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_close(max_shared_fifo_t fifo);

/**
 * \brief Write data into a shared FIFO.
 *
 * Note that this function will block until the specified amount of data has
 * been written to the FIFO.
 *
 * It is safe to cancel a thread calling this function (via pthread_cancel())
 * if the thread uses deferred cancellation.
 *
 * \param fifo The FIFO to write data into.
 * \param data Pointer to array of data to write into the FIFO.
 * \param num_bytes Number of bytes of data to write to the FIFO.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_write(max_shared_fifo_t fifo, const void *data, size_t num_bytes);

/**
 * \brief Read data from a shared FIFO.
 *
 * Note that this function will block until the specified amount of data has
 * been read from the FIFO.
 *
 * It is safe to cancel a thread calling this function (via pthread_cancel())
 * if the thread uses deferred cancellation.
 *
 * \param fifo The FIFO to read data from.
 * \param data Pointer to array that will hold the data read from the FIFO.
 * \param num_bytes Number of bytes of data to read from the FIFO.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_read(max_shared_fifo_t fifo, void *data, size_t num_bytes);

/**
 * \brief Get the number of bytes of data available in a shared FIFO.
 *
 * It is safe to cancel a thread calling this function (via pthread_cancel())
 * if the thread uses deferred cancellation.
 *
 * \param fifo The FIFO to query.
 * \param num_bytes Pointer to integer that will contain the number of bytes
 * of data available if the function returns successfully.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_get_data_available(max_shared_fifo_t fifo, size_t *num_bytes);

/**
 * \brief Get the number of bytes of space available in a shared FIFO.
 *
 * It is safe to cancel a thread calling this function (via pthread_cancel())
 * if the thread uses deferred cancellation.
 *
 * \param fifo The FIFO to query.
 * \param num_bytes Pointer to integer that will contain the number of bytes
 * of space available if the function returns successfully.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_get_space_available(max_shared_fifo_t fifo, size_t *num_bytes);

/**
 * \brief Abort any current or further transfers using a shared FIFO.
 *
 * Once a FIFO has been aborted, any current or future data transfers or
 * capacity queries will result in the \c ECONNABORTED error code.
 *
 * Once aborted, a FIFO can only be reused after calling
 * max_shared_fifo_reset().
 *
 * It is safe to cancel a thread calling this function (via pthread_cancel())
 * if the thread uses deferred cancellation.
 *
 * \param fifo The FIFO to abort.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_abort(max_shared_fifo_t fifo);

/**
 * \brief Reset a shared FIFO to the empty state.
 *
 * It is safe to cancel a thread calling this function (via pthread_cancel())
 * if the thread uses deferred cancellation.
 *
 * \param fifo The FIFO to reset.
 * \return 0 on success or an \c errno style error code on failure.
 */
int max_shared_fifo_reset(max_shared_fifo_t fifo);

#ifdef __cplusplus
}
#endif

#endif // !defined(MAX_SHARED_FIFO_H)
