#include "loadPath.hpp"
#include <cstring>
#include <unistd.h>
#include <iostream>

using namespace std;

static char BASE_RESOURCES_PATH[1024] = {'\0'};

static void initBasePath(void)
{
	getcwd(BASE_RESOURCES_PATH, 1024);
	size_t l = strlen(BASE_RESOURCES_PATH);
	if (BASE_RESOURCES_PATH[l-1] == '/')
		l--;
	char *sub = strstr(BASE_RESOURCES_PATH, "/graphics");
	if (! sub || BASE_RESOURCES_PATH+l-sub != 9)
		strcat(BASE_RESOURCES_PATH, "/graphics");
}

static std::string basePath(void)
{
	if (BASE_RESOURCES_PATH[0] == '\0')
		initBasePath();
	return std::string(BASE_RESOURCES_PATH);
}

std::string fontPath(std::string const & name)
{
	return basePath() + "/fonts/" + name;
}

std::string texturePath(std::string const & name)
{
	return basePath() + "/textures/" + name;
}

std::string soundPath(std::string const & name)
{
	return basePath() + "/sounds/" + name;
}
