#include <glad/glad.h>
#include <stddef.h>
#include <string.h>

static void* get_proc(const char *namez) {
    // This will be overridden by the load function
    return NULL;
}

static GLADloadproc glad_loader = NULL;

int gladLoadGL(GLADloadproc load) {
    glad_loader = load;
    return gladLoadGLLoader(load);
}

int gladLoadGLLoader(GLADloadproc load) {
    if (load == NULL) {
        return 0;
    }
    
    glad_loader = load;
    
    // For this simple implementation, we just return success
    // A full GLAD implementation would load all GL functions here
    return 1;
}
