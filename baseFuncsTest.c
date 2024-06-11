#include "baseFuncs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#define PI 3.14159265358979323846

void print_matrix(GLfloat* matrix);
void print_vector(GLfloat* vector, size_t size);
int assert_equal_matrix(GLfloat* expected, GLfloat* actual, size_t size, const char* testName);
int assert_equal_vector(GLfloat* expected, GLfloat* actual, size_t size, const char* testName);


int assert_equal(GLfloat* expected, GLfloat* actual, size_t size, const char* testName) {
    for (size_t i = 0; i < size / sizeof(GLfloat); i++) {
        if (fabs(expected[i] - actual[i]) > 0.01) {
            printf("%s: Nicht OK\n", testName);
            printf("Expected:\n");
            print_matrix(expected);
            printf("Actual:\n");
            print_matrix(actual);
            return 0;
        }
    }
    printf("%s: OK\n", testName);
    return 1;
}

int assert_equal_vector(GLfloat* expected, GLfloat* actual, size_t size, const char* testName) {
    int equal = 1;
    for (size_t i = 0; i < size / sizeof(GLfloat); i++) {
        if (fabs(expected[i] - actual[i]) > 0.01) {
            equal = 0;
        }
    }
    if (equal) {
        printf("%s: OK\n", testName);
    } else {
        printf("%s: Nicht OK\n", testName);
        printf("Expected:\n");
        print_vector(expected, size / sizeof(GLfloat));
        printf("Actual:\n");
        print_vector(actual, size / sizeof(GLfloat));
    }
    return equal;
}

void print_matrix(GLfloat* matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.2f ", matrix[i * 4 + j]);
        }
        printf("\n");
    }
}

void print_vector(GLfloat* vector, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%.2f ", vector[i]);
    }
    printf("\n");
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
    assert_equal(expected, result, sizeof(expected), "test_identity");
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
    assert_equal(expected, result, sizeof(expected), "test_translate");
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
    assert_equal(expected, result, sizeof(expected), "test_scale");
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
    assert_equal(expected, result, sizeof(expected), "test_rotatez");
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
    assert_equal(expected, result, sizeof(expected), "test_rotatex");
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
    assert_equal(expected, result, sizeof(expected), "test_rotatey");
}


void test_normalize() {
    GLfloat in[3] = {3.0f, 4.0f, 0.0f};
    GLfloat expected[3] = {0.6f, 0.8f, 0.0f};
    GLfloat result[3];
    normalize(result, in);
    assert_equal_vector(expected, result, sizeof(expected), "test_normalize");
}

void test_cross() {
    GLfloat a[3] = {1.0f, 0.0f, 0.0f};
    GLfloat b[3] = {0.0f, 1.0f, 0.0f};
    GLfloat expected[3] = {0.0f, 0.0f, 1.0f};
    GLfloat result[3];
    cross(result, a, b);
    assert_equal_vector(expected, result, sizeof(expected), "test_cross");
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
    assert_equal(expected, result, sizeof(expected), "test_lookAt");
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
    assert_equal(expected, result, sizeof(expected), "test_perspective");
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
