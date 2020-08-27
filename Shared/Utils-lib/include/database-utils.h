#include "database-structs.h"

#include "build_opts_utils.h"

namespace Database_Structs_Utils{
    std::string UTILS_IMPEXP serialize(DB_Users user);
    DB_Users UTILS_IMPEXP deserialize_db_users(std::string serialization);

    std::string UTILS_IMPEXP serialize(DB_Systems db_system);
    DB_Systems UTILS_IMPEXP deserialize_db_system(std::string serialization);
}
