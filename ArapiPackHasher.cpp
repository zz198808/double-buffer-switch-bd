#include"ArapiPackHasher.h"

namespace a {
namespace b {

ArapiPackHasher ArapiPackHasher::_s_instance;

ArapiPackHasher* ArapiPackHasher::instance() {
    return &_s_instance;
}

ArapiPackHasher::ArapiPackHasher() :
    _cur_table_idx(0),
    _dict_file(""),
    _sig_exit(false) {
    _dict_file = "data.txt";    
}

ArapiPackHasher::~ArapiPackHasher(){}


void* ArapiPackHasher::reload_forever(void* arg) {
    std::cout << "reload pack hash dict thread start!" << std::endl;
    ArapiPackHasher* p_hasher = (ArapiPackHasher*)arg;
    if (p_hasher == NULL) {
        std::cout << "p_hasher is null" << std::endl;
        return NULL;
    }
    if (p_hasher->do_reload_file() != 0) {
        std::cout << "hasher reload dict failed, quit!" << std::endl;
        return NULL;
    }
    std::cout << "hasher reload dict success" << std::endl;
    return NULL;
}

int ArapiPackHasher::read_hash_file() {
    //const char*  dict_file= "data.txt";
    //std::string dict_file = "data.txt";
    //std::ifstream input_file(_dict_file.c_str());
    std::ifstream input_file("data.txt");
    int key;
    int value;
    while (input_file >> key >> value) {
        _hash_table[1 - _cur_table_idx].set(key, value);
    }

}

uint64_t ArapiPackHasher::get_partition(uint64_t division_id) const  {
    uint64_t partition_id = 0;
    if (_hash_table[_cur_table_idx].get(division_id, &partition_id) == bsl::HASH_EXIST) {
        return partition_id;
    }
    return 0;
}

int ArapiPackHasher::do_reload_file() {
    do {
        struct stat file_stat;
        if (stat(_dict_file.c_str(), &file_stat) != 0) {
            std::cout << "file not exist" << std::endl;
            return -1;
        }
        if (file_stat.st_mtime == _file_last_modify_time) {
            sleep(_reload_sec);
            continue;
        }
        if (read_hash_file() < 0) {
            sleep(_reload_sec);
            continue;
        }
        _file_last_modify_time = file_stat.st_mtime;
        _cur_table_idx = 1 - _cur_table_idx;
        sleep(_reload_sec);
    } while (!_sig_exit);
    return 0;
}

int ArapiPackHasher::init(bsl::string dict_file,
         int reload_sec, uint32_t hash_bucket_num) {
    _dict_file = dict_file;
    _reload_sec = reload_sec;
    _hash_bucket_num = hash_bucket_num;
    if (_hash_table[0].create(hash_bucket_num) != 0 || 
            _hash_table[1].create(hash_bucket_num) != 0) {
        std::cout << "create hash table fail "<< std::endl;
        return -1;
    }
    if (pthread_create(&_reload_threadid, NULL, reload_forever, this) != 0) {
        std::cout << "create reload_thread for pack hasher failed!" << std::endl;
        return -1;
    }
    return 0;
}




}
}
