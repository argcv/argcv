#ifndef ARGCV_ML_PERCEPTRON_HH
#define ARGCV_ML_PERCEPTRON_HH

// Machine learning is a subfield of computer science that evolved from
// the study of pattern recognition and computational learning theory in
// Artificial Intelligence. Machine learning explores the construction and
// study of algorithms that can learn from and make predictions on data.
// ref: http://en.wikipedia.org/wiki/Machine_learning

#include <cstdio>
#include <cstdint>  // uint64_t

#include <string>
#include <vector>
#include <utility>  // std::pair, std::make_pair

#include "ml.hh"

namespace argcv {
namespace ml {

class perceptron : public ml<double, int> {
public:
    perceptron(uint64_t nx = 0, double rate = 0.001, size_t sloop = 1000)
        : nx(nx), rate(rate), sloop(sloop) {}

    perceptron(dataset<double, int> data, uint64_t nx = 0, double rate = 0.001, size_t sloop = 1000)
        : data(data), nx(nx), rate(rate), sloop(sloop) {}

    ~perceptron() {}

    bool learn() {
        if (nx == 0) {
            nx = data.x_size();
        }
        if (nx == 0) {
            return false;
        }
        w.clear();
        for (size_t ix = 0; ix < nx; ix++) {
            w.push_back(0);
        }
        for (size_t c = 0; c < sloop; c++) {
            bool changed = false;
            size_t sz_data = data.size();
            for (size_t ix = 0; ix < sz_data; ix++) {
                double p = 0;
                for (int j = 0; j < nx; j++) {
                    p += w[j] * data.x_at(ix, j);
                }
                if (data.y_at(ix) * p <= 0) {
                    changed = true;
                    for (int j = 0; j < nx; j++) {
                        w[j] += rate * data.y_at(ix) * data.x_at(ix, j);
                    }
                }
            }
            if (!changed) {
                // no_change_continue_num ++ ;
                // if(no_change_continue_num > 20)
                //{
                // printf("no any change at %d continued times : %d\n",c,no_change_continue_num);
                printf("no any change at %zu\n", c);
                break;
                //}
            } else {
                // no_change_continue_num = 0;
            }
        }
        return true;
    }

    void add(std::vector<double> x, int y) { data.add(x, y); }

    bool save(const std::string &path) {
        if (w.size() == 0) {
            fprintf(stderr, "[perceptron::save] size of w is zero, store failed\n");
            return false;
        }
        FILE *fp = fopen(path.c_str(), "wb");
        if (fp == NULL) {
            fprintf(stderr, "[perceptron::save] file %s open failed \n", path.c_str());
            return false;
        }
        double w2w[w.size()];
        size_t s2w = w.size();
        for (std::vector<double>::size_type ix = 0; ix != w.size(); ix++) {
            w2w[ix] = w[ix];
        }
        fwrite(&s2w, sizeof(size_t), 1, fp);
        fwrite(w2w, sizeof(double) * s2w, 1, fp);
        fclose(fp);
        return true;
    }
    bool load(const std::string &path) {
        FILE *fp = fopen(path.c_str(), "rb");
        if (fp == NULL) {
            fprintf(stderr, "[perceptron::save] file %s open failed \n", path.c_str());
            return false;
        }
        fread(&nx, sizeof(size_t), 1, fp);
        double w2r[nx];
        fread(&w2r, sizeof(double) * nx, 1, fp);
        fclose(fp);
        w.clear();
        for (size_t i = 0; i < nx; i++) {
            w.push_back(w2r[i]);
        }
        return true;
    }

    int predict(std::vector<double> x) {
        double p = 0;
        for (int j = 0; j < nx; j++) {
            p += w[j] * x[j];
        }
        return p;
    }

private:
    dataset<double, int> data;
    size_t nx;
    double rate;
    size_t sloop;
    std::vector<double> w;
};
}
}  // namespace argcv::ml

#endif  //  ARGCV_ML_PERCEPTRON_HH
