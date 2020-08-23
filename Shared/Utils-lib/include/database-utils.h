#include "database-structs.h"

namespace Database_Structs_Utils{
    std::string serialize(DB_Users user);

    DB_Users deserialize_db_users(std::string serialization);
}
