/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Yu Jing
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#include <cassert>
#include <cstdio>

#include <vector>

// Brief Headers List
// Ref:
#include "glm/common.hpp"         // all the GLSL common functions
#include "glm/exponential.hpp"    // all the GLSL exponential functions
#include "glm/geometric.hpp"      // all the GLSL geometry functions
#include "glm/gtc/constants.hpp"  // glm::pi
#include "glm/gtc/matrix_transform.hpp"  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/integer.hpp"            // all the GLSL integer functions
#include "glm/mat2x2.hpp"             // mat2, dmat2
#include "glm/mat2x3.hpp"             // mat2x3, dmat2x3
#include "glm/mat2x4.hpp"             // mat2x4, dmat2x4
#include "glm/mat3x2.hpp"             // mat3x2, dmat3x2
#include "glm/mat3x3.hpp"             // mat3, dmat3
#include "glm/mat3x4.hpp"             // mat3x4, dmat2
#include "glm/mat4x2.hpp"             // mat4x2, dmat4x2
#include "glm/mat4x3.hpp"             // mat4x3, dmat4x3
#include "glm/mat4x4.hpp"             // mat4, dmat4
#include "glm/matrix.hpp"             // all the GLSL matrix functions
#include "glm/packing.hpp"            // all the GLSL packing functions
#include "glm/trigonometric.hpp"      // all the GLSL trigonometric functions
#include "glm/vec2.hpp"               // vec2, bvec2, dvec2, ivec2 and uvec2
#include "glm/vec3.hpp"               // vec3, bvec3, dvec3, ivec3 and uvec3
#include "glm/vec4.hpp"               // vec4, bvec4, dvec4, ivec4 and uvec4
#include "glm/vector_relational.hpp"  // all the GLSL vector relational functions

#include "glog/logging.h"

void Print(glm::mat4 m4) {
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%6.5f ", m4[i][j]);
    }
    printf("]\n");
  }
}

glm::mat4 Camera(float translate, glm::vec2 const &rotate) {
  glm::mat4 projection =
      glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
  glm::mat4 view =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -translate));
  view = glm::rotate(view, rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  return projection * view * model;
}

void ExampleEqual() {
  glm::ivec4 A(1, 2, 3, 4);
  glm::ivec4 B(A);
  // LOG(INFO) << "eq:" << glm::equal(A, B);
  LOG(INFO) << "all(eq):" << glm::all(glm::equal(A, B));
}

void ExampleCamera() {
  std::vector<float> vtrans = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
  std::vector<float> rxs = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
  std::vector<float> rys = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
  for (auto t : vtrans) {
    for (auto rx : rxs) {
      for (auto ry : rys) {
        glm::vec2 rotate(rx, ry);
        glm::mat4 final = Camera(t, rotate);
        printf("translate: %.3f, rotate:[%.3f, %.3f]\n", t, rx, ry);
        Print(final);
        printf("---------------- END --------------\n\n");
      }
    }
  }
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  ExampleEqual();
  ExampleCamera();

  return 0;
}
