#include "logger.h"

static auto console = spdlog::stdout_color_mt("console");

void L_INFO(std::string m)
{
	console->info(m);
}
void L_WARN(std::string m)
{
	console->warn(m);
}
void L_ERROR(std::string m)
{
	console->error(m);
}
void L_CRITIC(std::string m)
{
	console->critical(m);
}