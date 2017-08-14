#include "config.h"
#include <fstream>
#include <eupulogger4system.h>
#include <cstdio>
#include <cstdlib>
#ifdef WITH_FILECONF
#include <json/json.h>
#endif

const static char* THREAD_CNT = "thread_cnt";
const static char* MULTI_FACE = "multi_face";
const static char* FACE_CROP = "face_crop";
const static char* SRC_PATH = "src_path";
const static char* DEST_PATH = "dest_path";
const static char* CROP_PATH = "crop_path";
const static char* IMG_SUFFIX = "img_suffix";
const static char* MODEL_PATH = "model_path";
const static char* LOG_PATH = "log_path";
const static char* MEAN_SHAPE = "mean_shape";
const static char* FINAL_SZ = "final_sz";
const static char* PTHFN_LST = "pthfn_lst";

Config::Config() {}

Config::~Config() {}

bool Config::init(const std::string& cfg_path) {
    bool ret = false;
    do {
#ifdef WITH_FILECONF
        std::ifstream f;
        f.open(cfg_path.c_str());
        if (!f.is_open()) {
            LOG(_ERROR_, "Config::init() open config file %s failed",
                cfg_path.c_str());
            break;
        }

        Json::Reader r;
        Json::Value v;

        if (!r.parse(f, v, false)) {
            LOG(_ERROR_, "Config::init() parse config %s failed",
                cfg_path.c_str());
            break;
        }
        selfpath_ = cfg_path;

        cfg_.thread_cnt = v[THREAD_CNT].asInt();
        cfg_.multi_face = v[MULTI_FACE].asInt();
        cfg_.face_crop = v[FACE_CROP].asInt();
        cfg_.src_path = v[SRC_PATH].asString();
        cfg_.dest_path = v[DEST_PATH].asString();
        cfg_.crop_path = v[CROP_PATH].asString();
        cfg_.img_suffix = v[IMG_SUFFIX].asString();
        cfg_.model_path = v[MODEL_PATH].asString();
        cfg_.log_path = v[LOG_PATH].asString();
        cfg_.mean_shape = v[MEAN_SHAPE].asInt();
        cfg_.final_sz = v[FINAL_SZ].asInt();
        cfg_.pthfn_lst = v[PTHFN_LST].asString();
#else
        cfg_.thread_cnt = 2;
        cfg_.multi_face = 0;
        cfg_.face_crop = 0;
        cfg_.src_path = "";
        cfg_.dest_path = "";
        cfg_.crop_path = "";
        cfg_.img_suffix = "png";
        cfg_.model_path = "";
        cfg_.log_path = "";
        cfg_.mean_shape = 256;
        cfg_.final_sz = 256;
        cfg_.pthfn_lst = "";
#endif
        ret = true;
    } while (0);
    return ret;
}

void Config::output() {
    printf("%s %d\n", THREAD_CNT, cfg_.thread_cnt);
    printf("%s %s\n", MULTI_FACE, cfg_.multi_face == 1 ? "true" : "false");
    printf("%s %s\n", FACE_CROP, cfg_.face_crop == 1 ? "true" : "false");
    printf("%s %s\n", SRC_PATH, cfg_.src_path.c_str());
    printf("%s %s\n", DEST_PATH, cfg_.dest_path.c_str());
    printf("%s %s\n", CROP_PATH, cfg_.crop_path.c_str());
    printf("%s %s\n", IMG_SUFFIX, cfg_.img_suffix.c_str());
    printf("%s %s\n", MODEL_PATH, cfg_.model_path.c_str());
    printf("%s %s\n", LOG_PATH, cfg_.log_path.c_str());
    printf("%s %d\n", MEAN_SHAPE, cfg_.mean_shape);
    printf("%s %d\n", FINAL_SZ, cfg_.final_sz);
    printf("%s %s\n", PTHFN_LST, cfg_.pthfn_lst.c_str());
}
