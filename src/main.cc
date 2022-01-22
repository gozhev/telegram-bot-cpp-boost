#include <csignal>
#include <cstdlib>

#include "TelegramBot.h"

namespace tb = gozhev::telegram_bot;

static volatile ::std::sig_atomic_t g_quit = 0;

static void SignalHandler(int)
{
	g_quit = 1;
}

int main(int argc, char** argv)
{
	struct ::sigaction sa{};
	sa.sa_handler = SignalHandler;
	::sigemptyset(&sa.sa_mask);
	::sigaction(SIGINT, &sa, nullptr);
	auto err = tb::Error{false};
	auto opt = tb::Options{argc, argv, err};
	if (err) {
		::std::cerr << "failed to initialize the bot options" << ::std::endl;
		return EXIT_FAILURE;
	}
	auto stop_pred = []() noexcept { return g_quit; };
	auto bot = tb::Bot{::std::move(opt), err};
	if (err) {
		::std::cerr << "failed to initialize the bot" << ::std::endl;
		return EXIT_FAILURE;
	}
	bot.Run(stop_pred, err);
	if (err) {
		::std::cerr << "the bot finished with error" << ::std::endl;
		return EXIT_SUCCESS;
	}
	return 0;
}

// vim: set ts=4 sw=4 noet :
