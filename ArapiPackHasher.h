#include<bsl/containers/hash/bsl_phashmap.h>
#include <bsl/string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

namespace a {
namespace b {
class ArapiPackHasher {
public:
    ~ArapiPackHasher();
    static ArapiPackHasher* instance();
    uint64_t get_partition(uint64_t division_id) const;
    int init(bsl::string dict_file, int reload_sec,
            uint32_t hash_bucket_num);
protected:
     ArapiPackHasher();
private :
    static void* reload_forever(void* arg);
    int do_reload_file();
    int read_hash_file();
private :
    bsl::phashmap<uint64_t, uint64_t> _hash_table[2];
    volatile uint32_t _cur_table_idx;
    time_t _file_last_modify_time;
    int _reload_sec;
    bsl::string _dict_file;
    uint32_t _hash_bucket_num;
    pthread_t _reload_threadid;
    volatile bool _sig_exit;
    static ArapiPackHasher _s_instance;
};

} //end namespace a
} // end namespace b
