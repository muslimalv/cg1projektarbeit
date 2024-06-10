#include "baseFuncs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#define PI 3.14159265358979323846

#define ASSERT_EQUAL(expected, actual, size, testName) do { \
    if (memcmp((expected), (actual), size) == 0) { \
        printf("%s: OK\n", testName); \
    } else { \
        printf("%s: Nicht OK\n", testName); \
        printf("Expected:\n"); \
        print_matrix(expected); \
        printf("Actual:\n"); \
        print_matrix(actual); \
    } \
} while (0)

void print_matrix(GLfloat* matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.2f ", matrix[i * 4 + j]);
        }
        printf("\n");
    }
}

void test_identity() {
    GLfloat result[16];
    GLfloat expected[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    identity(result);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_identity");
}

void test_translate() {
    GLfloat in[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    GLfloat v[3] = {1.0f, 2.0f, 3.0f};
    GLfloat result[16];
    GLfloat expected[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 2.0f, 3.0f, 1.0f
    };
    translate(result, in, v);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_translate");
}

void test_scale() {
    GLfloat in[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    GLfloat v[3] = {2.0f, 3.0f, 4.0f};
    GLfloat result[16];
    GLfloat expected[16] = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 3.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 4.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    scale(result, in, v);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_scale");
}

void test_rotatez() {
    GLfloat in[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    GLfloat angle = PI / 2;
    GLfloat result[16];
    GLfloat expected[16] = {
        cos(angle), -sin(angle), 0.0f, 0.0f,
        sin(angle), cos(angle), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    rotatez(result, in, angle);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_rotatez");
}

void test_rotatex() {
    GLfloat in[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    GLfloat angle = PI / 2;
    GLfloat result[16];
    GLfloat expected[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(angle), sin(angle), 0.0f,
        0.0f, -sin(angle), cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    rotatex(result, in, angle);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_rotatex");
}

void test_rotatey() {
    GLfloat in[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    GLfloat angle = PI / 2;
    GLfloat result[16];
    GLfloat expected[16] = {
        cos(angle), 0.0f, -sin(angle), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sin(angle), 0.0f, cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    rotatey(result, in, angle);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_rotatey");
}


void test_normalize() {
    GLfloat in[3] = {3.0f, 4.0f, 0.0f};
    GLfloat expected[3] = {0.6f, 0.8f, 0.0f};
    GLfloat result[3];
    normalize(in, result);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_normalize");
}

void test_cross() {
    GLfloat a[3] = {1.0f, 0.0f, 0.0f};
    GLfloat b[3] = {0.0f, 1.0f, 0.0f};
    GLfloat expected[3] = {0.0f, 0.0f, 1.0f};
    GLfloat result[3];
    cross(a, b, result);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_cross");
}


void test_lookAt() {
    GLfloat eye[3] = {0.0f, 0.0f, 5.0f};
    GLfloat center[3] = {0.0f, 0.0f, 0.0f};
    GLfloat up[3] = {0.0f, 1.0f, 0.0f};
    GLfloat result[16];
    GLfloat expected[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -0.0f, -0.0f, -5.0f, 1.0f
    };
    lookAt(result, eye, center, up);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_lookAt");
}

void test_perspective() {
    GLfloat result[16];
    GLfloat expected[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.02f, -1.0f,
        0.0f, 0.0f, -2.02f, 0.0f
    };
    perspective(result, 90.0f, 1.0f, 1.0f, 100.0f);
    ASSERT_EQUAL(expected, result, sizeof(expected), "test_perspective");
}

int main() {
    test_identity();
    test_translate();
    test_scale();
    test_rotatez();
    test_rotatex();
    test_rotatey();
    test_normalize();
    test_cross();
    test_lookAt();
    test_perspective();
    return 0;
}
