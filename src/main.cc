#include <cstddef>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <curses.h>
#include <eupulogger4system.h>
#include "fdthread.h"
#include "fd.h"
#include "config.h"
#include "fdstatistics.h"

void init_system(int worker_cnt);
void stop_system(int sig);
void exit_system();
void tasks_collection(const char *path, const char *suffix,
                      std::vector<std::string> &);
void tasks_distribution(std::vector<std::string> &);
void show_usage(int argc, char *argv[]);
void traversal_dir(const std::string &path, const std::string &suffix,
                   std::vector<std::string> &pathfilenames);
void traversal_lst(const std::string &path, const std::string &suffix,
                   std::vector<std::string> &pathfilenames);
std::string relative2absolut(const std::string &relative_dir);

std::vector<FdThread *> g_workers;

int main(int argc, char *argv[]) {
    show_usage(argc, argv);
    Config *pcfg = Config::get_instance();
    FdStatistics *pfdst = FdStatistics::get_instance();

    std::vector<std::string> tasks;
    tasks.reserve(1000000);

    tasks_collection(pcfg->get_src_path().c_str(),
                     pcfg->get_img_suffix().c_str(), tasks);
    init_system(pcfg->get_thread_cnt());
    tasks_distribution(tasks);

    signal(SIGINT, stop_system);
    for (auto it : g_workers) it->start();
    for (auto it : g_workers) pthread_join(it->get_pid(), NULL);

    exit_system();
    pfdst->output();

    if (pfdst) pfdst->release();
    if (pcfg) pcfg->release();
    CEupuLogger4System::Release();
    exit(EXIT_SUCCESS);
}

void init_system(int worker_cnt) {
    LOG(_INFO_, "init_system()");

    for (int i = 0; i < worker_cnt; ++i) {
        FdThread *pfdthread = new FdThread(i);
        if (pfdthread) {
            g_workers.push_back(pfdthread);
        }
    }
}

void stop_system(int sig) {
    LOG(_INFO_, "stop(%d)", sig);
    for (auto it : g_workers) it->stop();
}

void exit_system() {
    LOG(_INFO_, "exit_system()");
    for (auto it : g_workers) delete it;
}

void tasks_collection(const char *srcpath, const char *suffix,
                      std::vector<std::string> &pathfilenames) {
    std::string pthfn_lst = Config::get_instance()->get_pthfn_lst();
    if (pthfn_lst == "") {
        traversal_dir(srcpath, suffix, pathfilenames);
        LOG(_INFO_, "tasks_collection() traversal_dir()");
    } else {
        traversal_lst(pthfn_lst, suffix, pathfilenames);
        LOG(_INFO_, "tasks_collection() traversal_lst()");
    }
    uint32_t img_cnt = pathfilenames.size();
    FdStatistics::get_instance()->inc_src_cnt(img_cnt);
    LOG(_INFO_, "tasks_collection() get %d image files", img_cnt);
}

void tasks_distribution(std::vector<std::string> &pathfilenames) {
    std::vector<std::vector<std::string>> tasks;
    std::vector<std::string> emptyitem;
    emptyitem.reserve(125000);
    for (size_t i = 0; i < g_workers.size(); ++i) tasks.push_back(emptyitem);

    int idx = 0;
    for (auto it : pathfilenames) {
        tasks[idx % g_workers.size()].push_back(it);
        idx++;
    }

    idx = 0;
    for (auto it : tasks) {
        g_workers[idx]->set_imgfilenames(it);
        idx++;
    }
}

void traversal_dir(const std::string &srcpath, const std::string &suffix,
                   std::vector<std::string> &pathfilenames) {
    DIR *d;
    struct dirent *file;

    if (!(d = opendir(srcpath.c_str()))) {
        LOG(_ERROR_, "error opendir %s", srcpath.c_str());
        return;
    }

    std::string nextpath;
    while ((file = readdir(d)) != NULL) {
        if (strncmp(file->d_name, ".", 1) == 0) continue;

        nextpath = srcpath + "/" + file->d_name;
        struct stat sb;
        lstat(nextpath.c_str(), &sb);

        if (S_ISDIR(sb.st_mode)) {
            traversal_dir(nextpath, suffix, pathfilenames);
        } else {
            if (nextpath.substr(nextpath.length() - suffix.length(),
                                suffix.length()) == suffix)
                pathfilenames.push_back(nextpath);
        }
    }
    closedir(d);
}

void traversal_lst(const std::string &srcpath, const std::string &suffix,
                   std::vector<std::string> &pathfilenames) {
    FILE *fp;
    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    fp = fopen(srcpath.c_str(), "r");
    if (fp == nullptr) return;

    std::string fn;
    while ((read = getline(&line, &len, fp)) != -1) {
        line[read - 1] = '\0';
        fn = std::string(line);
        pathfilenames.push_back(fn);
    }
    free(line);
    fclose(fp);
}

void show_usage(int argc, char *argv[]) {
    bool valid_args = false;
    do {
        Config *pcfg = Config::get_instance();

#ifdef WITH_FILECONF
        if (argc != 2) break;

        std::string config_filename = std::string(argv[1]);
        if (config_filename == "") config_filename = "./conf.json";
        if (!pcfg || !pcfg->init(config_filename.c_str())) {
            LOG(_ERROR_, "main() Config init(%s) failed",
                config_filename.c_str());
            break;
        }
        int param_idx = 2;
#else
        if (argc < 3) break;
        int param_idx = 1;
#endif

        std::string srcpath = pcfg->get_src_path();
        std::string destpath = pcfg->get_dest_path();
        std::string croppath = pcfg->get_crop_path();
        std::string img_suffix = pcfg->get_img_suffix();
        int thread_cnt = pcfg->get_thread_cnt();
        int multi_face = pcfg->get_multi_face();
        int face_crop = pcfg->get_face_crop();
        std::string model_path = pcfg->get_model_path();
        std::string log_path = pcfg->get_log_path();
        int mean_shape = pcfg->get_mean_shape();
        int final_sz = pcfg->get_final_sz();
        std::string pthfn_lst = pcfg->get_pthfn_lst();

        if (argc > param_idx) {
            srcpath = std::string(argv[param_idx]);
            srcpath = relative2absolut(srcpath);
            pcfg->set_src_path(srcpath);
        }
        param_idx++;
        if (argc > param_idx) {
            croppath = std::string(argv[param_idx]);
            pcfg->set_crop_path(croppath);
        }
        param_idx++;
        if (argc > param_idx) {
            destpath = std::string(argv[param_idx]);
            destpath = relative2absolut(destpath);
            pcfg->set_dest_path(destpath);
        }
        param_idx++;
        if (argc > param_idx) {
            img_suffix = std::string(argv[param_idx]);
            pcfg->set_img_suffix(img_suffix);
        }
        param_idx++;
        if (argc > param_idx) {
            if (strtol(argv[param_idx], NULL, 10) == LONG_MIN ||
                strtol(argv[param_idx], NULL, 10) == LONG_MAX) {
                std::cout << "thread number must be a number" << std::endl;
                break;
            }
            thread_cnt = atoi(argv[param_idx]);
            pcfg->set_thread_cnt(thread_cnt);
        }
        param_idx++;
        if (argc > param_idx) {
            if (strtol(argv[param_idx], NULL, 10) == LONG_MIN ||
                strtol(argv[param_idx], NULL, 10) == LONG_MAX) {
                std::cout
                    << "set if deal with multi faces(1 for yes or 0 for no)"
                    << std::endl;
                break;
            }
            multi_face = atoi(argv[param_idx]);
            pcfg->set_multi_face(multi_face);
        }
        param_idx++;
        if (argc > param_idx) {
            if (strtol(argv[param_idx], NULL, 10) == LONG_MIN ||
                strtol(argv[param_idx], NULL, 10) == LONG_MAX) {
                std::cout << "set if write face crop result files(1 for yes or "
                             "0 for no)" << std::endl;
                break;
            }
            face_crop = atoi(argv[param_idx]);
            pcfg->set_face_crop(face_crop);
        }
        param_idx++;
        if (argc > param_idx) {
            model_path = std::string(argv[param_idx]);
            pcfg->set_model_path(model_path);
        }
        param_idx++;
        if (argc > param_idx) {
            log_path = std::string(argv[param_idx]);
            pcfg->set_log_path(log_path);
        }
        param_idx++;
        if (argc > param_idx) {
            mean_shape = atoi(argv[param_idx]);
            pcfg->set_mean_shape(mean_shape);
        }
        param_idx++;
        if (argc > param_idx) {
            final_sz = atoi(argv[param_idx]);
            pcfg->set_final_sz(final_sz);
        }
        param_idx++;
        if (argc > param_idx) {
            pthfn_lst = std::string(argv[param_idx]);
            pcfg->set_pthfn_lst(pthfn_lst);
        }

        LOG(_INFO_,
            "\nsrc: %s\ncrop: %s\ndest: %s\nsuffix: %s\ncnt: %d\nmulti_face: "
            "%d\nface_crop: %d\nmodel: %s\nlog: %s\nmean_shape: "
            "%d\nfinal_size: %d\npthfn_lst: %s\n",
            pcfg->get_src_path().c_str(), pcfg->get_crop_path().c_str(),
            pcfg->get_dest_path().c_str(), pcfg->get_img_suffix().c_str(),
            pcfg->get_thread_cnt(), pcfg->get_multi_face(),
            pcfg->get_face_crop(), pcfg->get_model_path().c_str(),
            pcfg->get_log_path().c_str(), pcfg->get_mean_shape(),
            pcfg->get_final_sz(), pcfg->get_pthfn_lst().c_str());

        valid_args = true;
    } while (0);

    if (!valid_args) {
        printf("usage:\n");
#ifdef WITH_FILECONF
        printf("./mtfd confpath/conf.json\n");
#else
        printf(
            "./mtfd srcpath croppath outputpath suffix thread_cnt "
            "enable_multiface enable_crop modelpath logpath mean_shape "
            "final_size pathfile_list\n");
#endif
        exit(EXIT_SUCCESS);
    }
}

std::string relative2absolut(const std::string &relative_dir) {
    std::vector<std::string> dirs;
    char dir[PATH_MAX] = {0};
    char *ch = getcwd(dir, PATH_MAX);
    if (ch == NULL) return "";

    std::string strdir(dir);

    if (relative_dir.empty()) return strdir;
    if (relative_dir[0] == '/') return relative_dir;

    boost::split(dirs, strdir, boost::is_any_of("/"), boost::token_compress_on);

    std::vector<std::string> relative_dirs;
    boost::split(relative_dirs, relative_dir, boost::is_any_of("/"),
                 boost::token_compress_on);

    for (auto it : relative_dirs) {
        if (it == ".") continue;
        if (it == ".." && dirs.size() > 0) {
            dirs.pop_back();
            continue;
        }
        dirs.push_back(it);
    }

    std::string absolut_dir;
    for (auto it : dirs) {
        if (it != "") {
            absolut_dir += "/";
            absolut_dir += it;
        }
    }
    return absolut_dir;
}
