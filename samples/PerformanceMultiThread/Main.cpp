//
// MultiThread - benchmark for multithread use.
//
#include <cstdlib>
#include <thread>

#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Log.h>

const size_t EVENTS_PER_THREAD = 800000;

enum { Console = 1 };

void threadFunction() {
	for (size_t i = 0; i < EVENTS_PER_THREAD; ++i)
		LOG(plog::debug) << "i=" << i << "clock:" << clock();
}

int main(int argc, char *argv[]) {
	plog::init(plog::debug, "Perfomance.txt");

	int thread_count = 0;
	if (argc > 1) {
		thread_count = atoi(argv[1]);
	}
	if (thread_count < 1) {
		thread_count = 4;
	}

	// Initialize the logger for printing info messages.
	static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init<Console>(plog::debug, &consoleAppender);
	PLOGI_(Console) << "Test started with " << thread_count << " threads";
	std::vector<std::thread> threads(thread_count);

	plog::util::Time startTime;
	plog::util::ftime(&startTime);

	for (size_t i = 0; i < threads.size(); ++i)
		threads[i] = std::thread(threadFunction);

	for (std::thread &thread : threads)
		thread.join();

	plog::util::Time finishTime;
	plog::util::ftime(&finishTime);

	time_t timeDiff = (finishTime.millitm - startTime.millitm) +
	                  (finishTime.time - startTime.time) * 1000;

	PLOGI_(Console) << "Test finished: "
	                << static_cast<double>(timeDiff) * 1000 / (static_cast<double>(EVENTS_PER_THREAD) * thread_count)
	                << " microsec per call, "
					<< static_cast<double> (EVENTS_PER_THREAD * thread_count * 1000) / static_cast<double>(timeDiff)
					<< " calls per second";

	return 0;
}
