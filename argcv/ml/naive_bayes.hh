#ifndef ARGCV_ML_NAIVE_BAYES_HH
#define ARGCV_ML_NAIVE_BAYES_HH

// Perceptron
// In machine learning, the perceptron is an algorithm for supervised
// classification of an input into one of several possible non-binary outputs.

#include <cstdio>
#include <cstdint>  // uint64_t

#include <string>
#include <vector>
#include <map>
#include <utility>  // std::pair, std::make_pair

#include "ml.hh"

namespace argcv {
namespace ml {

class naive_bayes : public ml<std::string, std::string> {
public:
    naive_bayes() {}

    naive_bayes(dataset<std::string, std::string> data) : data(data) {}

    ~naive_bayes() {}

    void add(std::string x[], int len, std::string y) { add(std::vector<std::string>(x, x + len), y); }

    void add(std::vector<std::string> x, std::string y) { data.add(x, y); }

    void add(std::pair<std::vector<std::string>, std::string> ds_item) { add(ds_item.first, ds_item.second); }

    bool learn() {
        wx.clear();
        wy.clear();
        dx_key2id.clear();
        dy_key2id.clear();
        size_t sz_data = data.size();
        size_t sz_x = data.x_size();
        for (size_t ix = 0; ix < sz_data; ix++) {
            wy[get_or_append(dy_key2id, wy, data.y_at(ix))]++;
        }
        for (size_t ixx = 0; ixx < sz_x; ixx++) {
            dx_key2id.push_back(std::map<std::string, int>());
        }
        for (size_t ix = 0; ix < dy_key2id.size(); ix++) {
            std::vector<std::vector<double>> wxx;
            for (size_t ixx = 0; ixx < sz_x; ixx++) {
                wxx.push_back(std::vector<double>());  // for each w
            }
            wx.push_back(wxx);
        }
        printf("#");
        fflush(NULL);
        for (size_t ix = 0; ix < sz_data; ix++) {
            int yid = find_by_key(dy_key2id, data.y_at(ix));
            printf("#[%d]\n", yid);
            fflush(NULL);
            for (size_t jx = 0; jx < sz_x; jx++) {
                printf("${%d|%d}", ix, jx);
                fflush(NULL);
                wx[yid][ix][get_or_append(dx_key2id[ix], wx[yid][ix], data.x_at(ix, jx))]++;
            }
            printf("#[%d]\n", yid);
            fflush(NULL);
        }
        printf("&&#");
        fflush(NULL);
        size_t sz_cd_y = dy_key2id.size();
        for (std::vector<double>::size_type ix = 0; ix != sz_cd_y; ix++) {
            wy[ix] = (wy[ix] + 1) / (sz_data + sz_cd_y);
        }
        for (size_t ix = 0; ix < sz_x; ix++) {
            size_t sz_cd_x = dx_key2id[ix].size();
        }
        for (std::map<std::string, int>::const_iterator it = dy_key2id.begin(); it != dy_key2id.end(); it++) {
            printf("%s - %f \n", it->first.c_str(), wy[it->second]);
        }
        return false;
    }

    bool save(const std::string &path) {
        FILE *fp = fopen(path.c_str(), "wb");
        if (fp == NULL) {
            fprintf(stderr, "[perceptron::save] file %s open failed \n", path.c_str());
            return false;
        }

        fclose(fp);
        return false;
    }

    bool load(const std::string &path) {
        FILE *fp = fopen(path.c_str(), "rb");
        if (fp == NULL) {
            fprintf(stderr, "[perceptron::save] file %s open failed \n", path.c_str());
            return false;
        }
        fclose(fp);
        return false;
    }

    std::string predict(std::vector<std::string> x) { return ""; }

private:
    dataset<std::string, std::string> data;
    std::vector<std::vector<std::vector<double>>> wx;
    std::vector<double> wy;
    std::vector<std::map<std::string, int>> dx_key2id;
    std::map<std::string, int> dy_key2id;

    // unsafe.
    int find_by_key(const std::map<std::string, int> &key2id, const std::string &key) {
        return key2id.find(key)->second;
    }
    int get_or_append(std::map<std::string, int> &key2id, std::vector<double> &w, const std::string &key) {
        std::map<std::string, int>::iterator it = key2id.find(key);
        if (it == key2id.end()) {
            int off = key2id.size();
            while (w.size() <= off) w.push_back(0);
            key2id.insert(std::make_pair(key, off));
            return off;
        } else {
            int off = it->second;
            while (w.size() <= off) w.push_back(0);
            return off;
        }
    }
};
}
}  // namespace argcv::ml

#endif  //  ARGCV_ML_NAIVE_BAYES_HH
