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

std::string fontPath(std::string name)
{
	std::string res = basePath() + "/fonts/" + name;
	cout << res << endl;
	return res;
}

std::string texturePath(std::string name)
{
	std::string res = basePath() + "/textures/" + name;
	cout << res << endl;
	return res;
}
