/**
 * @file channel.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 15/09/2014
 **/

#ifndef CHANNEL_INCLUDED
#define CHANNEL_INCLUDED

#include <pthread.h> // pthread_cond_t

#define I_AM_FIRST 0
#define READ_IS_FIRST 1
#define WRITE_IS_FIRST 2

#define CHANNEL_INPUT_TYPE 0
#define CHANNEL_OUTPUT_TYPE 1
#define NET_CHANNEL_INPUT_TYPE 2
#define NET_CHANNEL_OUTPUT_TYPE 3

struct channel_end_generic_t
{
	unsigned char type;
	void* channel_end;
};

/**
 * @brief Unbuffered unidirectional communication mean.
 **/
struct channel_t
{
	/// I_AM_FIRST if you are the first process, READ_IS_FIRST if the read is the first process and WRITE_IS_FIRST if it the write process which is first
	char am_I_first;

	/// Operation mutex
	pthread_mutex_t* mutex;

	/// Indicates if the partner process is done with the data
	pthread_cond_t* data_available;

	/// Indicates if the partner process has finished
	pthread_cond_t* process_finished;

	/// Size of the pointed buffer area
	unsigned int buffer_length;

	/// Pointer on the buffer area
	void* buffer;
};

/*****************
 * CHANNEL INPUT *
 *****************/

/**
 * @brief Channel input end; the end to read data from.
 **/
struct channel_input_end_t
{
	/// Pointer on the shared zone
	struct channel_t* channel;
};

/**
 * @brief Creates a channel input.
 * @return \li A pointer on the create channel input
		   	   \li NULL if it failed.
 **/
struct channel_input_end_t* create_channel_input_end();

/**
 * @brief Handles mutex and condition variable to prepare a read operation.
 * @param channel_input[in] - The channel input the read operation will be done on.
 **/
void prepare_read(struct channel_input_end_t* channel_input);

/**
 * @brief Handles mutex and condition variable to finish a read operation.
 * @param channel_input[in] - The channel input the read operation has been done on.
 **/
void finish_read(struct channel_input_end_t* channel_input);

/**
 * @brief Reads from the given channel and store the data in the given buffer, indicating its length in the given buffer_length.
 * @param channel_input[in] - The channel to read from.
 * @param buffer[out] - A pointer on an UNallocated memory zone. This function wil allocate the corresponding memory zone or set to NULL if it failed.
 * @param buffer_length[out] - The buffer length.
 **/
void read_from(struct channel_input_end_t* channel_input, void** buffer, unsigned int* buffer_length);

/******************
 * CHANNEL OUTPUT *
 ******************/

/**
 * @brief The channel output end; the end to write data to.
 **/
struct channel_output_end_t
{
	/// Pointer on the shared zone
	struct channel_t* channel;
};

/**
 * @brief Creates a channel output.
 * @return \li A pointer on the created channel output.
		   \li NULL if it failed.
 **/
struct channel_output_end_t* create_channel_output_end();

/**
 * @brief Handles mutex and condition variable to prepare a write operation.
 * @param channel_output[out] - The channel output the write operation will be done on.
 **/
void prepare_write(struct channel_output_end_t* channel_output);

/**
 * @brief Handles mutex and condition variable to finish a write operation.
 * @param channel_output[out] - The channel output the write operation has been done on.
 **/
void finish_write(struct channel_output_end_t* channel_output);

/**
 * @brief Writes to the given channel output the given buffer with the given buffer length.
 * @param channel_output[in] - The channel output to write on.
 * @param buffer[in] - The memory zone to send to the connecte channel input.
 * @param buffer_length[in] - The length of the buffer.
 **/
void write_to(struct channel_output_end_t* channel_output, void* buffer, unsigned int buffer_length);

/**
 * @brief Contains information for a write operation.
 * 
 * When a write function is used as a thread, it can only receive one argument; as a matter of fact, this only argument must contain every information needed.
 **/
struct write_destination_t
{
	/// The channel to write data to.
	struct channel_output_end_t* channel_output_end;

	/// The buffer length.
	unsigned int buffer_length;

	/// The buffer containing the data to be written.
	void* buffer;
};

/**
 * @brief Exactly the same job as writeTo but it only takes all arguments as one, it is easier to use as a thread using the format void* (*)(void*).
 * @param writeDestination[in] - The destination to write to.
 * @return NULL
 **/
void* write_to_destination(void* write_destination);

/***********
 * CHANNEL *
 ***********/

/**
 * @brief Contains a channel output end and a channel input end.
 * 
 * This type of channel is used to create a communication from one and one only process to another process and only one.
 **/
struct one_2_one_channel_t
{
	/// The channel output end; the end to write data to.
	struct channel_output_end_t* channel_output_end;

	/// The channel input end; the end to read data from.
	struct channel_input_end_t* channel_input_end;
};

/**
 * @brief Creates a one 2 one channel.
 * @return \li A pointer on the created channel. 
		   \li Returns NULL if failed.
 **/
struct one_2_one_channel_t* create_one_2_one_channel();

/**
 * @brief Connects a channel output with a channel input
 * @param in - The channel input
 * @param out - The channel output
 * @return \li 0 if succeeded
		   \li -1 if input end is null
		   \li -2 if output end is null
           \li -3 is both ends are null
		   \li -4 if the pipe failed.
 **/
int connect_channels(struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end);

#endif
