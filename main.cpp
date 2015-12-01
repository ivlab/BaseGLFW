// This code was taken from the GLFW website for a quick example:
// http://www.glfw.org/docs/latest/quick.html#quick_example

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "event/VREvent.h"
#include <net/VRNetClient.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<VREvent> events;

static void buttoncb(GLFWwindow* window, int button, int action, int mods)
{
    VRDataIndex di;

	(void)window;
	(void)mods;
	if (button == GLFW_MOUSE_BUTTON_LEFT ) {
		if (action == GLFW_PRESS) {
		    VREvent e("Mouse_Down", di);
		    events.push_back(e);
			//input.mbut |= MG_MOUSE_PRESSED;
		}
		if (action == GLFW_RELEASE) {
		    VREvent e("Mouse_Up", di);
		    events.push_back(e);
			//input.mbut |= MG_MOUSE_RELEASED;
		}
	}
}


int main(void)
{
	//VRNetClient client("134.84.121.29", "3490");
	VRNetClient client("localhost", "3490");

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, buttoncb);
    while (!glfwWindowShouldClose(window))
    {
        client.synchronizeInputEventsAcrossAllNodes(events);
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        events.clear();
        client.synchronizeSwapBuffersAcrossAllNodes();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
