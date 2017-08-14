#ifndef MTFD_INCLUDE_CONFIG_H_
#define MTFD_INCLUDE_CONFIG_H_

#include "aocsingleton.h"
#include <string>
#include <list>
#include <vector>

typedef struct {
    uint32_t thread_cnt;
    uint32_t multi_face;
    uint32_t face_crop;
    std::string src_path;
    std::string dest_path;
    std::string crop_path;
    std::string img_suffix;
    std::string model_path;
    std::string log_path;
    uint32_t mean_shape;
    uint32_t final_sz;
    std::string pthfn_lst;
} FdConf;

class Config : public IAocSingleton<Config> {
   public:
    bool init(const std::string &cfg_path);

    const std::string get_src_path() const { return cfg_.src_path; }
    const std::string get_dest_path() const { return cfg_.dest_path; }
    const std::string get_crop_path() const { return cfg_.crop_path; }
    const std::string get_img_suffix() const { return cfg_.img_suffix; }
    const std::string get_model_path() const { return cfg_.model_path; }
    const std::string get_log_path() const { return cfg_.model_path; }
    const uint32_t get_thread_cnt() const { return cfg_.thread_cnt; }
    const uint32_t get_multi_face() const { return cfg_.multi_face; }
    const uint32_t get_face_crop() const { return cfg_.face_crop; }
    const uint32_t get_mean_shape() const { return cfg_.mean_shape; }
    const uint32_t get_final_sz() const { return cfg_.final_sz; }
    const std::string get_pthfn_lst() const { return cfg_.pthfn_lst; }

    void output();

    void set_src_path(const std::string &src_path) { cfg_.src_path = src_path; }
    void set_dest_path(const std::string &dest_path) {
        cfg_.dest_path = dest_path;
    }
    void set_crop_path(const std::string &crop_path) {
        cfg_.crop_path = crop_path;
    }
    void set_img_suffix(const std::string &img_path) {
        cfg_.img_suffix = img_path;
    }
    void set_model_path(const std::string &model_path) {
        cfg_.model_path = model_path;
    }
    void set_log_path(const std::string &log_path) { cfg_.log_path = log_path; }
    void set_thread_cnt(int thread_cnt) { cfg_.thread_cnt = thread_cnt; }
    void set_multi_face(int multi_face) { cfg_.multi_face = multi_face; }
    void set_face_crop(int face_crop) { cfg_.face_crop = face_crop; }
    void set_mean_shape(int mean_shape) { cfg_.mean_shape = mean_shape; }
    void set_final_sz(int final_sz) { cfg_.final_sz = final_sz; }
    void set_pthfn_lst(const std::string &lst) { cfg_.pthfn_lst = lst; }

   private:
    Config();
    virtual ~Config();
    Config(const Config &);
    Config &operator=(const Config &);
    friend IAocSingleton<Config>;

   private:
    FdConf cfg_;
    std::string selfpath_;
};

#endif
