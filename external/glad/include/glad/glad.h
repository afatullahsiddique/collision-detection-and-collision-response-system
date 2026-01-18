#ifndef __glad_h_
#define __glad_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef void (*GLADloadproc)(void);

#ifndef GLAD_API
#define GLAD_API
#endif

GLAD_API int gladLoadGL(GLADloadproc load);
GLAD_API int gladLoadGLLoader(GLADloadproc load);

#include <GL/gl.h>

#ifdef __cplusplus
}
#endif

#endif
