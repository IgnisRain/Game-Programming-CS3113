#include "entity.h"
#include "Application.h"

using namespace std;

int main(int argc, char *argv[])
{
	Application app;
	while (!app.UpdateAndRender()) {}
	return 0;
}