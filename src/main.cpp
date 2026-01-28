#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

struct Vec2 {

    float x, y;
    
    Vec2 operator+(Vec2 other) const {
         return {
            x + other.x, y + other.y
        }; 
    }

    Vec2 operator-(Vec2 other) const {
         return {
            x - other.x, y - other.y
        }; 
    }
    Vec2 operator*(float s) const {
         return {
            x * s, y * s
        }; 
    }
    
    float magSq() const {
         return x*x + y*y; 
    }

    float mag() const { 
        return sqrtf(magSq()); 
    }
    
    Vec2 unit() const {
        float m = mag();
        return (m > 0.0001f) ? Vec2{x/m, y/m} : Vec2{0, 0};
    }
};

float dot(Vec2 a, Vec2 b) {
     return a.x * b.x + a.y * b.y; 
}


enum ShapeType {
     CIRCLE, BOX 
};

class Entity {

public:

    Vec2 pos, vel, acc;
    float mass, invMass;
    float bounciness = 0.6f; 
    float friction = 0.99f;

    Entity(float x, float y, float m) : pos({x, y}), mass(m) {
        invMass = (m > 0) ? 1.0f / m : 0;
        vel = {0, 0};
        acc = {0, -15.0f}; 
    }

    virtual ~Entity() {}
    virtual void render() = 0;
    virtual ShapeType type() = 0;

    void step(float dt) {

        if (invMass == 0) return;
        
        vel = vel + acc * dt;
        vel = vel * friction;
        pos = pos + vel * dt;
    }

};


class Ball : public Entity {

public:

    float r;

    Ball(float x, float y, float radius, float m) : Entity(x, y, m), r(radius) {}

    ShapeType type() override { return CIRCLE; }

    void render() override {

        glColor3f(0.3f, 0.7f, 1.0f);
        glBegin(GL_LINE_LOOP); 

        for(int i=0; i<20; i++) {
        
            float theta = 2.0f * 3.14159f * i / 20.0f;
            glVertex2f(pos.x + cos(theta)*r, pos.y + sin(theta)*r);
        }

        glEnd();
    }
};


class Rect : public Entity {

public:

    float w, h;

    Rect(float x, float y, float _w, float _h, float m) : Entity(x, y, m), w(_w), h(_h) {}

    ShapeType type() override { return BOX; }

    void render() override {

        glColor3f(0.9f, 0.3f, 0.3f);

        glBegin(GL_QUADS);
            glVertex2f(pos.x - w/2, pos.y - h/2);
            glVertex2f(pos.x + w/2, pos.y - h/2);
            glVertex2f(pos.x + w/2, pos.y + h/2);
            glVertex2f(pos.x - w/2, pos.y + h/2);
        glEnd();
    }
};



std::vector<Entity*> world;


// physics logic

void applyImpulse(Entity* a, Entity* b, Vec2 normal) {

    Vec2 rv = b->vel - a->vel;

    float velAlongNormal = dot(rv, normal);

    if (velAlongNormal > 0) return;

    float e = std::min(a->bounciness, b->bounciness);
    float j = -(1 + e) * velAlongNormal;
    j /= (a->invMass + b->invMass);

    Vec2 impulse = normal * j;
    a->vel = a->vel - impulse * a->invMass;
    b->vel = b->vel + impulse * b->invMass;

}

void checkCollisions() {

    for (size_t i = 0; i < world.size(); i++) {

        for (size_t j = i + 1; j < world.size(); ++j) {

            Entity *A = world[i];
            Entity *B = world[j];

            if (A->type() == CIRCLE && B->type() == CIRCLE) {

                Ball *c1 = (Ball*)A;
                Ball *c2 = (Ball*)B;
                Vec2 dir = B->pos - A->pos;
                float dist = dir.mag();
                float sumR = c1->r + c2->r;

                if (dist < sumR) {

                    Vec2 normal = dir.unit();

                    float depth = sumR - dist;
                    A->pos = A->pos - normal * (depth * 0.5f);
                    B->pos = B->pos + normal * (depth * 0.5f);
                    applyImpulse(A, B, normal);
                }

            }

            else if (A->type() == CIRCLE || B->type() == CIRCLE) {

                Entity *c = (A->type() == CIRCLE) ? A : B;
                Entity *r = (c == A) ? B : A;
                Ball *bObj = (Ball*)c;
                Rect *rObj = (Rect*)r;

                float cx = std::max(r->pos.x - rObj->w/2, std::min(c->pos.x, r->pos.x + rObj->w/2));
                float cy = std::max(r->pos.y - rObj->h/2, std::min(c->pos.y, r->pos.y + rObj->h/2));

                Vec2 delta = c->pos - Vec2{cx, cy};

                if (delta.mag() < bObj->r) {
                    applyImpulse(c, r, delta.unit());
                }

            }
        }

        if (world[i]->pos.y < -90) {
            world[i]->pos.y = -90;
            world[i]->vel.y *= -0.5f;
        }

    }
}


void onFrame() {

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-100, 100, -100, 100);

    for (auto e : world) e->render();

    glutSwapBuffers();
}

void onTick(int value) {

    const float dt = 0.016f;
    for (auto e : world) e->step(dt);
    checkCollisions();

    glutPostRedisplay();
    glutTimerFunc(16, onTick, 0);
}

int main(int argc, char** argv) {

    world.push_back(new Ball(-30, 50, 8, 2.0f));
    world.push_back(new Ball(10, 80, 12, 5.0f));
    world.push_back(new Rect(0, 0, 150, 2, 2.0f));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Small Physics Engine");

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glutDisplayFunc(onFrame);
    glutTimerFunc(16, onTick, 0);
    
    glutMainLoop();
    
    return 0;
}
