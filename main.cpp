#include"ArapiPackHasher.h"

int main () {
    int key = 1;
    //init
    bsl::string file_name = "data.txt";
    int reload_sec = 2;
    uint32_t hash_bucket_num = 100;
    int ret = a::b::ArapiPackHasher::instance()->init(file_name, reload_sec, hash_bucket_num);
    if (ret != 0) {
        std::cout << " init fail " << std::endl;
        return 0;
    }
    while(true) {
        int value = a::b::ArapiPackHasher::instance()->get_partition(key);
        std::cout << " value is " << value << std::endl;
        sleep(5);
    }
}
