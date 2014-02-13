#include <iostream>
#include <json.hpp>

using namespace std;

int main(int argc, const char **argv)
{
    JSON::Dict manager;
    manager.set("name", "Titouan");
    manager.set("level", 15);
    manager.set("fame", 17.2);

    manager.set("players_id", JSON::List());
    LIST(manager.get("players_id")).append(13);
    LIST(manager.get("players_id")).append(17);

    cout << "Output direct: " << manager << endl;
    
    string const & repr = manager.dumps();
    cout << "Output string: " << repr << endl;

    return 0;
}
