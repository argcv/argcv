#ifndef ARGCV_ML_PERCEPTRON_HH
#define ARGCV_ML_PERCEPTRON_HH

// Perceptron
// In machine learning, the perceptron is an algorithm for supervised
// classification of an input into one of several possible non-binary outputs.

#include <cstdio>
#include <cstdint>  // uint64_t

#include <string>
#include <vector>
#include <utility>  // std::pair, std::make_pair

#include "ml.hh"

namespace argcv {
namespace ml {

class perceptron : public ml<double, bool> {
public:
    perceptron(uint64_t nx = 0, double rate = 0.0001, size_t c = 10000) : nx(nx), rate(rate), c(c) {}

    perceptron(dataset<double, bool> data, uint64_t nx = 0, double rate = 0.0001, size_t c = 10000)
        : data(data), nx(nx), rate(rate), c(c) {}

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
        w.push_back(0);
        size_t sz_data = data.size();
        // printf("data size : %zu \n", sz_data);
        for (size_t cc = 0; cc < c; cc++) {
            bool changed = false;
            for (size_t ix = 0; ix < sz_data; ix++) {
                double p = 0;
                for (int j = 0; j < nx; j++) {
                    p += w[j] * data.x_at(ix, j);
                }
                p += w[nx];
                int yatix = ytoint(data.y_at(ix));
                if (yatix * p <= 0) {
                    changed = true;
                    for (int j = 0; j < nx; j++) {
                        w[j] += rate * yatix * data.x_at(ix, j);
                    }
                    w[nx] += rate * yatix;
                }
            }
            if (!changed) {
                printf("[perceptron] no change and jump out : %zu \n", cc);
                break;
                //}
            } else {
                // dump();
                // no_change_continue_num = 0;
                if (cc % 1000 == 0) {
                    printf("[perceptron] learn : %zu \n", cc);
                }
            }
        }
        return true;
    }

    void dump() {
        printf("w: %zu \n", w.size());
        for (size_t i = 0; i < w.size(); i++) {
            printf("%zu : %f\n", i, w[i]);
        }
    }

    void add(std::vector<double> x, bool y) { data.add(x, y); }

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

    bool predict(std::vector<double> x) {
        double p = 0;
        for (int j = 0; j < nx; j++) {
            p += w[j] * x[j];
            // printf("p : %f w[j]: %f x[j]: %f\n ", p, w[j], x[j]);
        }
        // printf("p : %f w[j]: %f \n ", p, w[nx]);
        p += w[nx];
        // printf("p : %f w[j]: %f \n ", p, w[nx]);
        return p > 0;
    }

private:
    dataset<double, bool> data;
    size_t nx;
    double rate;
    size_t c;
    std::vector<double> w;
    inline int ytoint(bool y) { return y ? 1 : -1; }
};
}
}  // namespace argcv::ml

/*
// sample

using namespace argcv::ml;

bool y(std::vector<double> x) {
    // printf("%f \n",x[0] * 0.2 + x[1] * 0.8 + x[2] * 0.3);
    return (x[0] * 0.1 * x[1] * 0.8 - x[2] * 0.3 - x[3] * 0.1 + x[4] * 0.2 + 0.2 > 0.5);
}

int main(int argc, char* argv[]) {
    perceptron mml;
    for (int i = 0; i < 2600; i++) {
        double x1 = (random_int() % 100) + 0.0001;
        double x2 = (random_int() % 100) + 0.0001;
        double x3 = (random_int() % 100) + 0.0001;
        double x4 = (random_int() % 100) + 0.0001;
        double x5 = (random_int() % 100) + 0.0001;
        std::vector<double> x
            = {(2.0 / x1) - 1, (2.0 / x2) - 1, (2.0 / x3) - 1, (2.0 / x4) - 1, (2.0 / x5) - 1};
        // printf("## %f - %f - %f \n",(2.0 / random_int()) - 1, (2.0 / random_int()) - 1, (2.0 /
        // random_int()) - 1);
        mml.add(x, y(x));
    }

    mml.learn();
    mml.dump();

    aprf f;
    size_t sz = 0;
    bool listed = false;

    for (double i = -1; i < 1; i += 0.1) {
        for (double j = -1; j < 1; j += 0.1) {
            for (double k = -1; k < 1; k += 0.1) {
                for (double m = -1; m < 1; m += 0.1) {
                    for (double n = -1; n < 1; n += 0.1) {
                        std::vector<double> x = {i, j, k, m, n};
                        f.add(mml.predict(x) > 0, y(x) > 0);
                        if (!listed && ((mml.predict(x) > 0) != (y(x) > 0))) {
                            // printf("%f - %f - %f \n", i, j, k);
                            listed = true;
                        }
                        sz++;
                    }
                }
            }
        }
    }

    printf("result :  sz : %zu pre : %f rec : %f ac : %f , f1 : %f \n", sz, f.precision(), f.recall(),
           f.accuracy(), f.f1());

    return 0;
}
*/

#endif  //  ARGCV_ML_PERCEPTRON_HH
